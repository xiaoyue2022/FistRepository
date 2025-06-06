#include "auditlog.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QDateTime>
#include <QDebug>
#include <gmssl/sm3.h>
#include <gmssl/sm4.h>
#include <gmssl/error.h>
#include <gmssl/rand.h>
#include <QDataStream>

// 初始化静态成员
AuditLog* AuditLog::instance = nullptr;

AuditLog* AuditLog::getInstance() {
    if (instance == nullptr) {
        instance = new AuditLog();
    }
    return instance;
}

AuditLog::AuditLog(QObject *parent) : QObject(parent) {
    currentLogFile = QDir::current().filePath("audit.log");
    backupFiles = {
        QDir::current().filePath("audit.log.1"),
        QDir::current().filePath("audit.log.2"),
        QDir::current().filePath("audit.log.3")
    };
    initializeLogFile();
}

AuditLog::~AuditLog() {
    // 确保最后的日志都写入文件
    mutex.lock();
    // 清理工作
    mutex.unlock();
}

void AuditLog::logEvent(const QString& userId,
                       AuditEventType eventType,
                       const QString& resourcePath,
                       const QHostAddress& sourceIP,
                       const QString& details) {
    QMutexLocker locker(&mutex);
    
    AuditLogEntry entry;
    entry.timestamp = QDateTime::currentDateTime();
    entry.userId = userId;
    entry.eventType = eventType;
    entry.resourcePath = resourcePath;
    entry.sourceIP = sourceIP;
    entry.details = details;
    
    // 计算SM3哈希链
    entry.previousHash = calculateSM3Hash(entry);
    
    // 检查是否需要轮转日志文件
    QFile file(currentLogFile);
    if (file.size() > 10 * 1024 * 1024) { // 10MB
        rotateLogFile();
    }
    
    // 加密并保存日志条目
    if (!encryptAndSaveEntry(entry)) {
        qDebug() << "Failed to save audit log entry";
        return;
    }
    
    // 检查异常行为
    if (isAnomalous(entry)) {
        emit securityAlert("Anomalous behavior detected", entry);
    }
    
    // 创建备份
    createBackup();
}

QByteArray AuditLog::calculateSM3Hash(const AuditLogEntry& entry) {
    SM3_CTX ctx;
    unsigned char hash[32];
    
    // 初始化SM3上下文
    sm3_init(&ctx);
    
    // 更新前一个哈希值
    if (!entry.previousHash.isEmpty()) {
        sm3_update(&ctx, (unsigned char*)entry.previousHash.data(), entry.previousHash.size());
    }
    
    // 添加当前条目的数据
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << entry.timestamp
           << entry.userId
           << static_cast<int>(entry.eventType)
           << entry.resourcePath
           << entry.sourceIP.toString()
           << entry.details;
    
    sm3_update(&ctx, (unsigned char*)data.data(), data.size());
    
    // 计算最终哈希值
    sm3_finish(&ctx, hash);
    
    return QByteArray((char*)hash, 32);
}

bool AuditLog::encryptAndSaveEntry(const AuditLogEntry& entry) {
    // 序列化日志条目
    QJsonObject json;
    json["timestamp"] = entry.timestamp.toString(Qt::ISODate);
    json["userId"] = entry.userId;
    json["eventType"] = static_cast<int>(entry.eventType);
    json["resourcePath"] = entry.resourcePath;
    json["sourceIP"] = entry.sourceIP.toString();
    json["details"] = entry.details;
    json["previousHash"] = QString(entry.previousHash.toHex());
    json["currentHash"] = QString(entry.currentHash.toHex());
    
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    
    // 生成加密密钥和IV
    QByteArray key = generateSM4Key();
    QByteArray iv = generateIV();
    
    // 加密数据
    QByteArray encrypted;
    if (!encryptWithSM4(data, key, iv, encrypted)) {
        return false;
    }
    
    // 写入文件
    QFile file(currentLogFile);
    if (!file.open(QIODevice::Append)) {
        return false;
    }
    
    // 写入IV和加密数据
    QDataStream stream(&file);
    stream << iv << encrypted;
    
    return true;
}

bool AuditLog::encryptWithSM4(const QByteArray& data, const QByteArray& key,
                             const QByteArray& iv, QByteArray& encrypted) const {
    SM4_KEY sm4_key;
    
    // 设置SM4密钥
    sm4_set_encrypt_key(&sm4_key, (unsigned char*)key.data());
    
    // 计算填充后的长度
    int paddedLen = (data.size() + 15) & ~15;
    encrypted.resize(paddedLen);
    
    // CBC模式加密
    unsigned char prev[16];
    memcpy(prev, iv.data(), 16);
    
    for (int i = 0; i < data.size(); i += 16) {
        unsigned char block[16] = {0};
        int blockSize = qMin(16, data.size() - i);
        memcpy(block, data.data() + i, blockSize);
        
        // XOR with previous block
        for (int j = 0; j < 16; j++) {
            block[j] ^= prev[j];
        }
        
        // Encrypt block
        sm4_encrypt(&sm4_key, block, (unsigned char*)encrypted.data() + i);
        memcpy(prev, encrypted.data() + i, 16);
    }
    
    return true;
}

QByteArray AuditLog::generateSM4Key() const {
    QByteArray key(16, 0);
    rand_bytes((unsigned char*)key.data(), 16);
    return key;
}

QByteArray AuditLog::generateIV() const {
    QByteArray iv(16, 0);
    rand_bytes((unsigned char*)iv.data(), 16);
    return iv;
}

bool AuditLog::verifyIntegrity() {
    QMutexLocker locker(&mutex);
    
    QFile file(currentLogFile);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    
    QByteArray previousHash;
    QDataStream stream(&file);
    
    while (!stream.atEnd()) {
        QByteArray iv, encrypted;
        stream >> iv >> encrypted;
        
        AuditLogEntry entry;
        if (!decryptAndLoadEntry(encrypted, entry)) {
            return false;
        }
        
        // 验证哈希链
        if (entry.previousHash != previousHash) {
            return false;
        }
        
        // 验证当前条目的哈希值
        if (entry.currentHash != calculateSM3Hash(entry)) {
            return false;
        }
        
        previousHash = entry.currentHash;
    }
    
    return true;
}

QVector<AuditLogEntry> AuditLog::searchLogs(const QDateTime& startTime,
                                          const QDateTime& endTime,
                                          const QString& userId,
                                          AuditEventType eventType) {
    QMutexLocker locker(&mutex);
    QVector<AuditLogEntry> results;
    
    QFile file(currentLogFile);
    if (!file.open(QIODevice::ReadOnly)) {
        return results;
    }
    
    QDataStream stream(&file);
    while (!stream.atEnd()) {
        QByteArray iv, encrypted;
        stream >> iv >> encrypted;
        
        AuditLogEntry entry;
        if (!decryptAndLoadEntry(encrypted, entry)) {
            continue;
        }
        
        // 应用过滤条件
        if (entry.timestamp < startTime || entry.timestamp > endTime) {
            continue;
        }
        
        if (!userId.isEmpty() && entry.userId != userId) {
            continue;
        }
        
        if (entry.eventType != eventType) {
            continue;
        }
        
        results.append(entry);
    }
    
    return results;
}

bool AuditLog::isAnomalous(const AuditLogEntry& entry) {
    // 实现异常检测逻辑
    // 1. 检查短时间内的失败登录次数
    if (entry.eventType == AuditEventType::ACCESS_DENIED) {
        auto recentFailures = searchLogs(
            entry.timestamp.addSecs(-300),  // 5分钟内
            entry.timestamp,
            entry.userId,
            AuditEventType::ACCESS_DENIED
        );
        
        if (recentFailures.size() >= 5) {  // 5次失败尝试
            return true;
        }
    }
    
    // 2. 检查非工作时间的访问
    int hour = entry.timestamp.time().hour();
    if (hour < 9 || hour >= 18) {
        return true;
    }
    
    // 3. 检查异常IP地址
    if (!entry.sourceIP.isInSubnet(QHostAddress("192.168.0.0"), 16) &&
        !entry.sourceIP.isInSubnet(QHostAddress("10.0.0.0"), 8)) {
        return true;
    }
    
    return false;
}

void AuditLog::createBackup() {
    // 创建日志文件的备份
    for (const QString& backupFile : backupFiles) {
        QFile::copy(currentLogFile, backupFile);
    }
}

bool AuditLog::rotateLogFile() {
    // 轮转备份文件
    for (int i = backupFiles.size() - 1; i > 0; --i) {
        QFile::rename(backupFiles[i-1], backupFiles[i]);
    }
    
    // 将当前日志文件移动到第一个备份
    return QFile::rename(currentLogFile, backupFiles[0]);
}

bool AuditLog::decryptAndLoadEntry(QByteArray& encryptedData, AuditLogEntry& entry) {
    // 解密数据
    QByteArray decrypted;
    QByteArray key = generateSM4Key(); // 在实际应用中应该从安全存储中获取密钥
    QByteArray iv(16, 0); // 在实际应用中应该从文件中读取IV
    
    if (!decryptWithSM4(encryptedData, key, iv, decrypted)) {
        return false;
    }
    
    // 解析JSON数据
    QJsonDocument doc = QJsonDocument::fromJson(decrypted);
    if (doc.isNull()) {
        return false;
    }
    
    QJsonObject json = doc.object();
    entry.timestamp = QDateTime::fromString(json["timestamp"].toString(), Qt::ISODate);
    entry.userId = json["userId"].toString();
    entry.eventType = static_cast<AuditEventType>(json["eventType"].toInt());
    entry.resourcePath = json["resourcePath"].toString();
    entry.sourceIP = QHostAddress(json["sourceIP"].toString());
    entry.details = json["details"].toString();
    entry.previousHash = QByteArray::fromHex(json["previousHash"].toString().toLatin1());
    entry.currentHash = QByteArray::fromHex(json["currentHash"].toString().toLatin1());
    
    return true;
}

bool AuditLog::decryptWithSM4(const QByteArray& encrypted, const QByteArray& key,
                             const QByteArray& iv, QByteArray& decrypted) const {
    SM4_KEY sm4_key;
    
    // 设置SM4密钥
    sm4_set_decrypt_key(&sm4_key, (unsigned char*)key.data());
    
    decrypted.resize(encrypted.size());
    
    // CBC模式解密
    unsigned char prev[16];
    memcpy(prev, iv.data(), 16);
    
    for (int i = 0; i < encrypted.size(); i += 16) {
        unsigned char block[16];
        sm4_encrypt(&sm4_key, (unsigned char*)encrypted.data() + i, block);
        
        // XOR with previous block
        for (int j = 0; j < 16; j++) {
            block[j] ^= prev[j];
        }
        
        memcpy(decrypted.data() + i, block, 16);
        memcpy(prev, encrypted.data() + i, 16);
    }
    
    return true;
}

bool AuditLog::initializeLogFile() {
    QMutexLocker locker(&mutex);
    
    // 检查日志文件是否存在
    QFile file(currentLogFile);
    if (!file.exists()) {
        // 创建新的日志文件
        if (!file.open(QIODevice::WriteOnly)) {
            qDebug() << "Failed to create audit log file:" << currentLogFile;
            return false;
        }
        file.close();
    }
    
    // 验证备份文件
    bool allBackupsOk = true;
    for (const QString& backupFile : backupFiles) {
        QFile backup(backupFile);
        if (!backup.exists()) {
            // 创建空的备份文件
            if (!backup.open(QIODevice::WriteOnly)) {
                qDebug() << "Failed to create backup file:" << backupFile;
                allBackupsOk = false;
                continue;
            }
            backup.close();
        }
    }
    
    // 验证日志文件完整性
    if (!verifyIntegrity()) {
        qDebug() << "Warning: Audit log integrity check failed";
        // 可以在这里添加额外的恢复或通知机制
        return false;
    }
    
    return allBackupsOk;
} 