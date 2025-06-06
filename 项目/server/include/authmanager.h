#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QDateTime>
#include <QMap>
#include <QMutex>
#include "accesscontrol.h"

class AuthManager : public QObject
{
    Q_OBJECT
public:
    static AuthManager* getInstance();
    
    // 用户注册
    bool registerUser(const QString& username, const QString& password, Role role = Role::USER);
    
    // 用户登录
    bool login(const QString& username, const QString& password);
    
    // 检查用户是否被锁定
    bool isUserLocked(const QString& username);
    
    // 重置失败次数
    void resetFailedAttempts(const QString& username);
    
    // 获取剩余锁定时间（秒）
    int getRemainingLockTime(const QString& username);

    // 访问控制相关方法
    bool checkPermission(const QString& username, const QString& resource, Permission perm);
    bool grantPermission(const QString& adminId, const QString& username, 
                        const QString& resource, Permission perm);
    bool revokePermission(const QString& adminId, const QString& username, 
                         const QString& resource, Permission perm);
    bool assignRole(const QString& adminId, const QString& username, Role role);
    bool validateAccess(const QString& username, const QString& resource, 
                       Permission perm, const AccessContext& context);
    bool verifyTwoFactorAuth(const QString& adminId, const QString& code);
    
    // 获取访问控制实例
    AccessControl* getAccessControl() const;

signals:
    void twoFactorAuthRequired(const QString& adminId);
    void permissionChanged(const QString& username, const QString& resource);

private:
    explicit AuthManager(QObject *parent = nullptr);
    ~AuthManager();
    
    // 单例实例
    static AuthManager* instance;
    
    // 数据库连接
    QSqlDatabase db;
    
    // 访问控制管理器
    AccessControl* accessControl;
    
    // 失败尝试记录 <username, <failCount, lastFailTime>>
    QMap<QString, QPair<int, QDateTime>> failedAttempts;
    
    // 互斥锁
    QMutex mutex;
    
    // 常量定义
    const int MAX_FAILED_ATTEMPTS = 5;        // 最大失败次数
    const int LOCK_DURATION = 300;            // 锁定时长（秒）
    const int REPLAY_WINDOW = 30;             // 重放攻击窗口（秒）
    const int SALT_LENGTH = 16;               // 盐值长度（字节）
    
    // 内部方法
    bool initDatabase();                      // 初始化数据库
    QByteArray generateSalt();                // 生成随机盐值
    QByteArray calculateHash(const QString& password, const QByteArray& salt); // 计算哈希值
    bool verifyPassword(const QString& username, const QString& password);     // 验证密码
    void recordFailedAttempt(const QString& username);                        // 记录失败尝试
    bool checkReplayAttack(const QString& username, const QDateTime& loginTime); // 检查重放攻击
    bool verifyAdminAndTwoFactor(const QString& adminId);                     // 验证管理员权限和双因素认证
};

#endif // AUTHMANAGER_H