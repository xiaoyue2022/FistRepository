#include "authmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCryptographicHash>
#include <QTime>
#include <gmssl/sm3.h>
#include <gmssl/error.h>

// 初始化静态成员
AuthManager* AuthManager::instance = nullptr;

AuthManager* AuthManager::getInstance()
{
    if (instance == nullptr) {
        instance = new AuthManager();
        // 初始化随机数种子
        qsrand(QTime::currentTime().msec());
    }
    return instance;
}

AuthManager::AuthManager(QObject *parent) : QObject(parent)
{
    // 初始化数据库
    if (!initDatabase()) {
        qDebug() << "Failed to initialize database";
        return;
    }
    
    // 初始化访问控制
    accessControl = new AccessControl(this);
    connect(accessControl, &AccessControl::twoFactorAuthRequired,
            this, &AuthManager::twoFactorAuthRequired);
    connect(accessControl, &AccessControl::permissionChanged,
            this, &AuthManager::permissionChanged);
}

AuthManager::~AuthManager()
{
    if (db.isOpen()) {
        db.close();
    }
}

bool AuthManager::initDatabase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("auth.db");
    
    if (!db.open()) {
        qDebug() << "Failed to open database:" << db.lastError().text();
        return false;
    }
    
    // 创建用户表
    QSqlQuery query;
    bool success = query.exec(
        "CREATE TABLE IF NOT EXISTS users ("
        "username TEXT PRIMARY KEY,"
        "password_hash BLOB NOT NULL,"
        "salt BLOB NOT NULL,"
        "last_login DATETIME,"
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ")"
    );
    
    if (!success) {
        qDebug() << "Failed to create users table:" << query.lastError().text();
        return false;
    }
    
    return true;
}

QByteArray AuthManager::generateSalt()
{
    QByteArray salt;
    salt.resize(SALT_LENGTH);
    
    // 使用基础的随机数生成器生成随机盐值
    for (int i = 0; i < SALT_LENGTH; ++i) {
        salt[i] = static_cast<char>(qrand() % 256);
    }
    
    return salt;
}

QByteArray AuthManager::calculateHash(const QString& password, const QByteArray& salt)
{
    // 将密码和盐值组合
    QByteArray combined = password.toUtf8() + salt;
    
    // 使用SM3计算哈希
    unsigned char hash[32];  // SM3 输出固定为 32 字节
    SM3_CTX ctx;
    
    sm3_init(&ctx);
    sm3_update(&ctx, (unsigned char*)combined.data(), combined.size());
    sm3_finish(&ctx, hash);
    
    return QByteArray((char*)hash, 32);
}

bool AuthManager::registerUser(const QString& username, const QString& password, Role role)
{
    QMutexLocker locker(&mutex);
    
    QSqlQuery query(db);
    query.prepare("SELECT username FROM users WHERE username = ?");
    query.addBindValue(username);
    
    if (!query.exec()) {
        qDebug() << "Failed to query user:" << query.lastError().text();
        return false;
    }
    
    if (query.next()) {
        qDebug() << "User already exists";
        return false;
    }
    
    QByteArray salt = generateSalt();
    QByteArray hash = calculateHash(password, salt);
    
    query.prepare("INSERT INTO users (username, password_hash, salt) VALUES (?, ?, ?)");
    query.addBindValue(username);
    query.addBindValue(hash);
    query.addBindValue(salt);
    
    if (!query.exec()) {
        qDebug() << "Failed to insert user:" << query.lastError().text();
        return false;
    }
    
    // 分配初始角色
    return accessControl->assignRole(username, role);
}

bool AuthManager::checkPermission(const QString& username, const QString& resource, Permission perm)
{
    return accessControl->hasPermission(username, resource, perm);
}

bool AuthManager::grantPermission(const QString& adminId, const QString& username, 
                                const QString& resource, Permission perm)
{
    if (!verifyAdminAndTwoFactor(adminId)) {
        return false;
    }
    return accessControl->grantPermission(username, resource, perm);
}

bool AuthManager::revokePermission(const QString& adminId, const QString& username, 
                                 const QString& resource, Permission perm)
{
    if (!verifyAdminAndTwoFactor(adminId)) {
        return false;
    }
    return accessControl->revokePermission(username, resource, perm);
}

bool AuthManager::assignRole(const QString& adminId, const QString& username, Role role)
{
    // 检查管理员权限（不需要双因素认证，因为已经通过密码验证）
    if (!accessControl->hasRole(adminId, Role::ADMIN)) {
        qDebug() << "User is not an admin:" << adminId;
        return false;
    }
    return accessControl->assignRole(username, role);
}

bool AuthManager::validateAccess(const QString& username, const QString& resource, 
                               Permission perm, const AccessContext& context)
{
    return accessControl->validateAccess(username, resource, perm, context);
}

bool AuthManager::verifyAdminAndTwoFactor(const QString& adminId)
{
    if (!accessControl->hasRole(adminId, Role::ADMIN)) {
        qDebug() << "User is not an admin:" << adminId;
        return false;
    }
    
    // 请求双因素认证
    if (!accessControl->requestTwoFactorAuth(adminId)) {
        qDebug() << "Failed to request 2FA for admin:" << adminId;
        return false;
    }
    
    return true;
}

bool AuthManager::login(const QString& username, const QString& password)
{
    QMutexLocker locker(&mutex);
    
    // 检查用户是否被锁定
    if (isUserLocked(username)) {
        qDebug() << "User is locked";
        return false;
    }
    
    // 验证密码
    if (!verifyPassword(username, password)) {
        recordFailedAttempt(username);
        return false;
    }
    
    // 检查重放攻击
    QDateTime currentTime = QDateTime::currentDateTime();
    if (!checkReplayAttack(username, currentTime)) {
        qDebug() << "Possible replay attack detected";
        return false;
    }
    
    // 更新最后登录时间
    QSqlQuery query;
    query.prepare("UPDATE users SET last_login = ? WHERE username = ?");
    query.addBindValue(currentTime);
    query.addBindValue(username);
    
    if (!query.exec()) {
        qDebug() << "Failed to update last login time:" << query.lastError().text();
    }
    
    // 重置失败尝试次数
    resetFailedAttempts(username);
    
    return true;
}

bool AuthManager::verifyPassword(const QString& username, const QString& password)
{
    QSqlQuery query;
    query.prepare("SELECT password_hash, salt FROM users WHERE username = ?");
    query.addBindValue(username);
    
    if (!query.exec() || !query.next()) {
        return false;
    }
    
    QByteArray storedHash = query.value(0).toByteArray();
    QByteArray salt = query.value(1).toByteArray();
    
    QByteArray calculatedHash = calculateHash(password, salt);
    
    return (storedHash == calculatedHash);
}

void AuthManager::recordFailedAttempt(const QString& username)
{
    auto it = failedAttempts.find(username);
    if (it == failedAttempts.end()) {
        failedAttempts.insert(username, qMakePair(1, QDateTime::currentDateTime()));
    } else {
        it.value().first++;
        it.value().second = QDateTime::currentDateTime();
    }
}

bool AuthManager::isUserLocked(const QString& username)
{
    auto it = failedAttempts.find(username);
    if (it == failedAttempts.end()) {
        return false;
    }
    
    if (it.value().first >= MAX_FAILED_ATTEMPTS) {
        QDateTime lockTime = it.value().second;
        if (lockTime.secsTo(QDateTime::currentDateTime()) < LOCK_DURATION) {
            return true;
        } else {
            resetFailedAttempts(username);
        }
    }
    
    return false;
}

void AuthManager::resetFailedAttempts(const QString& username)
{
    failedAttempts.remove(username);
}

int AuthManager::getRemainingLockTime(const QString& username)
{
    auto it = failedAttempts.find(username);
    if (it == failedAttempts.end() || it.value().first < MAX_FAILED_ATTEMPTS) {
        return 0;
    }
    
    int secsElapsed = it.value().second.secsTo(QDateTime::currentDateTime());
    int remainingTime = LOCK_DURATION - secsElapsed;
    
    return (remainingTime > 0) ? remainingTime : 0;
}

bool AuthManager::checkReplayAttack(const QString& username, const QDateTime& loginTime)
{
    QSqlQuery query;
    query.prepare("SELECT last_login FROM users WHERE username = ?");
    query.addBindValue(username);
    
    if (!query.exec() || !query.next()) {
        return false;
    }
    
    QDateTime lastLogin = query.value(0).toDateTime();
    if (lastLogin.isValid()) {
        int secsSinceLastLogin = lastLogin.secsTo(loginTime);
        if (secsSinceLastLogin < REPLAY_WINDOW) {
            return false;
        }
    }
    
    return true;
}

bool AuthManager::verifyTwoFactorAuth(const QString& adminId, const QString& code) {
    if (!accessControl->hasRole(adminId, Role::ADMIN)) {
        return false;
    }
    return accessControl->verifyTwoFactorAuth(adminId, code);
}

AccessControl* AuthManager::getAccessControl() const
{
    return accessControl;
}