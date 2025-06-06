#ifndef SECUREFILETRANSFER_H
#define SECUREFILETRANSFER_H

#include <QObject>
#include <QFile>
#include <QByteArray>
#include <QString>
#include <QMap>
#include <QMutex>

struct TransferSession {
    QString sessionId;
    QByteArray sm4Key;        // SM4会话密钥
    QByteArray sm4IV;         // SM4-CBC的IV
    QByteArray sm2PublicKey;  // 客户端的SM2公钥
    qint64 fileSize;          // 文件总大小
    qint64 transferredSize;   // 已传输大小
    QString filePath;         // 文件路径
    QByteArray sm3Hash;       // 文件的SM3哈希值
};

class SecureFileTransfer : public QObject {
    Q_OBJECT

public:
    static SecureFileTransfer* getInstance();
    
    // 初始化传输会话
    QString initializeSession(const QString& filePath, 
                            const QByteArray& clientPublicKey);
    
    // 继续传输
    bool resumeTransfer(const QString& sessionId, qint64 offset);
    
    // 获取传输进度
    double getProgress(const QString& sessionId) const;
    
    // 验证文件完整性
    bool verifyFileIntegrity(const QString& sessionId, const QByteArray& clientHash);
    
    // 加密文件块
    QByteArray encryptFileBlock(const QString& sessionId, 
                               const QByteArray& data);
    
    // 解密文件块
    QByteArray decryptFileBlock(const QString& sessionId, 
                               const QByteArray& encryptedData);

signals:
    void transferProgress(const QString& sessionId, double progress);
    void transferCompleted(const QString& sessionId);
    void transferError(const QString& sessionId, const QString& error);

private:
    explicit SecureFileTransfer(QObject *parent = nullptr);
    ~SecureFileTransfer();
    
    static SecureFileTransfer* instance;
    QMutex mutex;
    
    // 活动的传输会话
    QMap<QString, TransferSession> activeSessions;
    
    // 内部方法
    QByteArray generateSessionKey() const;
    QByteArray generateIV() const;
    QByteArray calculateSM3Hash(const QString& filePath) const;
    QByteArray encryptWithSM2(const QByteArray& data, 
                             const QByteArray& publicKey) const;
    QByteArray encryptWithSM4CBC(const QByteArray& data,
                                const QByteArray& key,
                                const QByteArray& iv) const;
    QByteArray decryptWithSM4CBC(const QByteArray& encryptedData,
                                const QByteArray& key,
                                const QByteArray& iv) const;
    QString generateSessionId() const;
    void cleanupSession(const QString& sessionId);
};

#endif // SECUREFILETRANSFER_H 