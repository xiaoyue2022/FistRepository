#include "securefiletransfer.h"
#include <QDateTime>
#include <QCryptographicHash>
#include <QDebug>
#include <gmssl/sm2.h>
#include <gmssl/sm3.h>
#include <gmssl/sm4.h>
#include <gmssl/rand.h>

// Define SM2 point size if not defined
#ifndef SM2_DEFAULT_POINT_SIZE
#define SM2_DEFAULT_POINT_SIZE 33  // Compressed point format (1 byte prefix + 32 bytes for x-coordinate)
#endif

// 初始化静态成员
SecureFileTransfer* SecureFileTransfer::instance = nullptr;

SecureFileTransfer* SecureFileTransfer::getInstance() {
    if (instance == nullptr) {
        instance = new SecureFileTransfer();
    }
    return instance;
}

SecureFileTransfer::SecureFileTransfer(QObject *parent) : QObject(parent) {
}

SecureFileTransfer::~SecureFileTransfer() {
    // 清理所有活动会话
    QMutexLocker locker(&mutex);
    activeSessions.clear();
}

QString SecureFileTransfer::initializeSession(const QString& filePath,
                                           const QByteArray& clientPublicKey) {
    QMutexLocker locker(&mutex);
    
    QFile file(filePath);
    if (!file.exists()) {
        return QString();
    }
    
    // 创建新会话
    TransferSession session;
    session.sessionId = generateSessionId();
    session.sm2PublicKey = clientPublicKey;
    session.filePath = filePath;
    session.fileSize = file.size();
    session.transferredSize = 0;
    
    // 生成SM4密钥和IV
    session.sm4Key = generateSessionKey();
    session.sm4IV = generateIV();
    
    // 计算文件的SM3哈希值
    session.sm3Hash = calculateSM3Hash(filePath);
    
    // 保存会话
    activeSessions[session.sessionId] = session;
    
    return session.sessionId;
}

bool SecureFileTransfer::resumeTransfer(const QString& sessionId, qint64 offset) {
    QMutexLocker locker(&mutex);
    
    if (!activeSessions.contains(sessionId)) {
        return false;
    }
    
    TransferSession& session = activeSessions[sessionId];
    
    // 验证偏移量
    if (offset < 0 || offset >= session.fileSize) {
        return false;
    }
    
    session.transferredSize = offset;
    return true;
}

double SecureFileTransfer::getProgress(const QString& sessionId) const {
    QMutexLocker locker(&mutex);
    
    if (!activeSessions.contains(sessionId)) {
        return -1.0;
    }
    
    const TransferSession& session = activeSessions[sessionId];
    return static_cast<double>(session.transferredSize) / session.fileSize;
}

bool SecureFileTransfer::verifyFileIntegrity(const QString& sessionId,
                                           const QByteArray& clientHash) {
    QMutexLocker locker(&mutex);
    
    if (!activeSessions.contains(sessionId)) {
        return false;
    }
    
    const TransferSession& session = activeSessions[sessionId];
    return (session.sm3Hash == clientHash);
}

QByteArray SecureFileTransfer::encryptFileBlock(const QString& sessionId,
                                              const QByteArray& data) {
    QMutexLocker locker(&mutex);
    
    if (!activeSessions.contains(sessionId)) {
        return QByteArray();
    }
    
    TransferSession& session = activeSessions[sessionId];
    
    // 使用SM4-CBC加密数据块
    QByteArray encrypted = encryptWithSM4CBC(data, session.sm4Key, session.sm4IV);
    if (encrypted.isEmpty()) {
        return QByteArray();
    }
    
    // 更新传输进度
    session.transferredSize += data.size();
    emit transferProgress(sessionId, getProgress(sessionId));
    
    // 检查是否传输完成
    if (session.transferredSize >= session.fileSize) {
        emit transferCompleted(sessionId);
    }
    
    return encrypted;
}

QByteArray SecureFileTransfer::decryptFileBlock(const QString& sessionId,
                                              const QByteArray& encryptedData) {
    QMutexLocker locker(&mutex);
    
    if (!activeSessions.contains(sessionId)) {
        return QByteArray();
    }
    
    const TransferSession& session = activeSessions[sessionId];
    return decryptWithSM4CBC(encryptedData, session.sm4Key, session.sm4IV);
}

QByteArray SecureFileTransfer::generateSessionKey() const {
    QByteArray key(16, 0);  // SM4使用128位密钥
    rand_bytes((unsigned char*)key.data(), key.size());
    return key;
}

QByteArray SecureFileTransfer::generateIV() const {
    QByteArray iv(16, 0);  // SM4-CBC模式需要128位IV
    rand_bytes((unsigned char*)iv.data(), iv.size());
    return iv;
}

QByteArray SecureFileTransfer::calculateSM3Hash(const QString& filePath) const {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return QByteArray();
    }
    
    SM3_CTX ctx;
    unsigned char hash[32];
    
    sm3_init(&ctx);
    
    // 分块读取文件并更新哈希
    const int blockSize = 8192;
    QByteArray block;
    while (!(block = file.read(blockSize)).isEmpty()) {
        sm3_update(&ctx, (unsigned char*)block.data(), block.size());
    }
    
    sm3_finish(&ctx, hash);
    return QByteArray((char*)hash, 32);
}

QByteArray SecureFileTransfer::encryptWithSM2(const QByteArray& data,
                                            const QByteArray& publicKey) const {
    SM2_KEY key;
    
    // 导入公钥
    if (sm2_key_set_public_key(&key, reinterpret_cast<const SM2_Z256_POINT*>(publicKey.constData())) != 1) {
        return QByteArray();
    }
    
    // 计算加密后的长度
    size_t cipherLen;
    if (sm2_encrypt_fixlen(&key, (unsigned char*)data.data(), data.size(),
                          SM2_DEFAULT_POINT_SIZE, nullptr, &cipherLen) != 1) {
        return QByteArray();
    }
    
    // 分配空间
    QByteArray encrypted(cipherLen, 0);
    
    // 加密数据
    if (sm2_encrypt(&key, (unsigned char*)data.data(), data.size(),
                   (unsigned char*)encrypted.data(), &cipherLen) != 1) {
        return QByteArray();
    }
    
    encrypted.resize(cipherLen);
    return encrypted;
}

QByteArray SecureFileTransfer::encryptWithSM4CBC(const QByteArray& data,
                                               const QByteArray& key,
                                               const QByteArray& iv) const {
    SM4_KEY sm4_key;
    
    // 设置加密密钥
    sm4_set_encrypt_key(&sm4_key, (unsigned char*)key.data());
    
    // 计算填充后的长度
    int paddedLen = (data.size() + 15) & ~15;  // 向上取整到16的倍数
    QByteArray padded = data;
    padded.resize(paddedLen);
    
    // PKCS7填充
    int paddingLen = paddedLen - data.size();
    for (int i = data.size(); i < paddedLen; ++i) {
        padded[i] = paddingLen;
    }
    
    QByteArray encrypted(paddedLen, 0);
    unsigned char prev[16];
    memcpy(prev, iv.data(), 16);
    
    // CBC模式加密
    for (int i = 0; i < paddedLen; i += 16) {
        unsigned char block[16];
        memcpy(block, padded.data() + i, 16);
        
        // XOR with previous block
        for (int j = 0; j < 16; j++) {
            block[j] ^= prev[j];
        }
        
        // Encrypt block
        sm4_encrypt(&sm4_key, block, (unsigned char*)encrypted.data() + i);
        memcpy(prev, encrypted.data() + i, 16);
    }
    
    return encrypted;
}

QByteArray SecureFileTransfer::decryptWithSM4CBC(const QByteArray& encryptedData,
                                               const QByteArray& key,
                                               const QByteArray& iv) const {
    if (encryptedData.size() % 16 != 0) {
        return QByteArray();
    }
    
    SM4_KEY sm4_key;
    
    // 设置解密密钥
    sm4_set_decrypt_key(&sm4_key, (unsigned char*)key.data());
    
    QByteArray decrypted(encryptedData.size(), 0);
    unsigned char prev[16];
    memcpy(prev, iv.data(), 16);
    
    // CBC模式解密
    for (int i = 0; i < encryptedData.size(); i += 16) {
        unsigned char block[16];
        sm4_encrypt(&sm4_key, (unsigned char*)encryptedData.data() + i, block);
        
        // XOR with previous block
        for (int j = 0; j < 16; j++) {
            block[j] ^= prev[j];
        }
        
        memcpy(decrypted.data() + i, block, 16);
        memcpy(prev, encryptedData.data() + i, 16);
    }
    
    // 移除PKCS7填充
    if (decrypted.isEmpty()) {
        return QByteArray();
    }
    
    int paddingLen = decrypted.at(decrypted.size() - 1);
    if (paddingLen <= 0 || paddingLen > 16) {
        return QByteArray();
    }
    
    // 验证填充
    for (int i = decrypted.size() - paddingLen; i < decrypted.size(); ++i) {
        if (decrypted.at(i) != paddingLen) {
            return QByteArray();
        }
    }
    
    decrypted.chop(paddingLen);
    return decrypted;
}

QString SecureFileTransfer::generateSessionId() const {
    QByteArray randomData(16, 0);
    rand_bytes((unsigned char*)randomData.data(), randomData.size());
    return randomData.toHex();
}

void SecureFileTransfer::cleanupSession(const QString& sessionId) {
    QMutexLocker locker(&mutex);
    activeSessions.remove(sessionId);
} 