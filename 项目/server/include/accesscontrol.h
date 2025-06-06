#ifndef ACCESSCONTROL_H
#define ACCESSCONTROL_H

#include <QObject>
#include <QMap>
#include <QSet>
#include <QString>
#include <QDateTime>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QHostAddress>

enum class Role {
    ADMIN,
    USER,
    GUEST
};

// Add hash function for Role
inline uint qHash(const Role &key, uint seed = 0) noexcept {
    return qHash(static_cast<uint>(key), seed);
}

enum class Permission {
    READ,
    WRITE,
    DELETE,
    MODIFY_PERMISSIONS
};

// Add hash function for Permission
inline uint qHash(const Permission &key, uint seed = 0) noexcept {
    return qHash(static_cast<uint>(key), seed);
}

struct AccessContext {
    QDateTime timestamp;
    QHostAddress ipAddress;
    QString userAgent;

    AccessContext() : 
        timestamp(QDateTime::currentDateTime()),
        ipAddress(QHostAddress::LocalHost),
        userAgent("Default") {}
        
    AccessContext(const QDateTime& ts, const QHostAddress& ip, const QString& agent) :
        timestamp(ts),
        ipAddress(ip),
        userAgent(agent) {}
};

class AccessControl : public QObject {
    Q_OBJECT

public:
    explicit AccessControl(QObject *parent = nullptr);
    
    // Role management
    bool assignRole(const QString& userId, Role role);
    bool hasRole(const QString& userId, Role role) const;
    
    // Permission management
    bool grantPermission(const QString& userId, const QString& resource, Permission perm);
    bool revokePermission(const QString& userId, const QString& resource, Permission perm);
    bool hasPermission(const QString& userId, const QString& resource, Permission perm) const;
    
    // Context-based access control
    bool validateAccess(const QString& userId, const QString& resource, 
                       Permission perm, const AccessContext& context);
    
    // Two-factor authentication
    bool requestTwoFactorAuth(const QString& adminId);
    bool verifyTwoFactorAuth(const QString& adminId, const QString& code);
    
    // ACL management
    bool saveACL();
    bool loadACL();
    
signals:
    void permissionChanged(const QString& userId, const QString& resource);
    void twoFactorAuthRequired(const QString& adminId);

private:
    struct ACLEntry {
        QSet<Role> roles;
        QMap<QString, QSet<Permission>> resourcePermissions;
    };
    
    QMap<QString, ACLEntry> userAcl;
    QMap<QString, QString> pendingTwoFactorAuth;
    
    QString encryptACL(const QJsonDocument& doc) const;
    QJsonDocument decryptACL(const QString& encrypted) const;
    bool validateTimeBasedAccess(const QString& resource, const QDateTime& timestamp) const;
    bool validateIPBasedAccess(const QString& resource, const QHostAddress& ip) const;
};

#endif // ACCESSCONTROL_H 