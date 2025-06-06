#ifndef AUDITLOG_H
#define AUDITLOG_H

#include <QObject>
#include <QDateTime>
#include <QString>
#include <QFile>
#include <QMutex>
#include <QVector>
#include <QHostAddress>

// 审计日志事件类型
enum class AuditEventType {
    USER_LOGIN,
    USER_LOGOUT,
    FILE_UPLOAD,
    FILE_DOWNLOAD,
    PERMISSION_CHANGE,
    ROLE_CHANGE,
    ACCESS_DENIED,
    SYSTEM_ERROR
};

// 审计日志条目
struct AuditLogEntry {
    QDateTime timestamp;
    QString userId;
    AuditEventType eventType;
    QString resourcePath;
    QHostAddress sourceIP;
    QString details;
    QByteArray previousHash;  // SM3哈希链的前一个哈希值
    QByteArray currentHash;   // 当前条目的SM3哈希值
};

class AuditLog : public QObject {
    Q_OBJECT

public:
    static AuditLog* getInstance();
    
    // 记录审计事件
    void logEvent(const QString& userId,
                 AuditEventType eventType,
                 const QString& resourcePath,
                 const QHostAddress& sourceIP,
                 const QString& details);
    
    // 验证日志完整性
    bool verifyIntegrity();
    
    // 搜索日志
    QVector<AuditLogEntry> searchLogs(const QDateTime& startTime,
                                    const QDateTime& endTime,
                                    const QString& userId = QString(),
                                    AuditEventType eventType = AuditEventType::USER_LOGIN);
    
    // 统计分析
    int countEvents(AuditEventType eventType,
                   const QDateTime& startTime,
                   const QDateTime& endTime);
    
    // 导出日志
    bool exportLogs(const QString& filePath,
                   const QDateTime& startTime,
                   const QDateTime& endTime);

signals:
    void securityAlert(const QString& message, const AuditLogEntry& entry);

private:
    explicit AuditLog(QObject *parent = nullptr);
    ~AuditLog();
    
    static AuditLog* instance;
    QMutex mutex;
    
    // 当前日志文件路径
    QString currentLogFile;
    // 备份日志文件路径列表
    QVector<QString> backupFiles;
    
    // 内部方法
    bool initializeLogFile();
    bool rotateLogFile();
    QByteArray calculateSM3Hash(const AuditLogEntry& entry);
    bool encryptAndSaveEntry(const AuditLogEntry& entry);
    bool decryptAndLoadEntry(QByteArray& encryptedData, AuditLogEntry& entry);
    void createBackup();
    bool isAnomalous(const AuditLogEntry& entry);
    
    // SM4密钥和IV管理
    QByteArray generateSM4Key() const;
    QByteArray generateIV() const;
    bool encryptWithSM4(const QByteArray& data, const QByteArray& key, 
                       const QByteArray& iv, QByteArray& encrypted) const;
    bool decryptWithSM4(const QByteArray& encrypted, const QByteArray& key,
                       const QByteArray& iv, QByteArray& decrypted) const;
};

#endif // AUDITLOG_H 