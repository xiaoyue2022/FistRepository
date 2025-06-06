#include "accesscontrol.h"
#include <QFile>
#include <QSettings>
#include <QDir>
#include <QTime>
#include <QJsonArray>

AccessControl::AccessControl(QObject *parent) : QObject(parent) {
    loadACL();
    qsrand(QTime::currentTime().msec());  // 初始化随机数种子
}

bool AccessControl::assignRole(const QString& userId, Role role) {
    if (userAcl.contains(userId)) {
        userAcl[userId].roles.insert(role);
    } else {
        ACLEntry entry;
        entry.roles.insert(role);
        userAcl[userId] = entry;
    }
    return true;
}

bool AccessControl::hasRole(const QString& userId, Role role) const {
    return userAcl.contains(userId) && userAcl[userId].roles.contains(role);
}

bool AccessControl::grantPermission(const QString& userId, const QString& resource, Permission perm) {
    if (!userAcl.contains(userId)) {
        return false;
    }
    
    auto& entry = userAcl[userId];
    if (!entry.resourcePermissions.contains(resource)) {
        entry.resourcePermissions[resource] = QSet<Permission>();
    }
    entry.resourcePermissions[resource].insert(perm);
    
    emit permissionChanged(userId, resource);
    return true;
}

bool AccessControl::revokePermission(const QString& userId, const QString& resource, Permission perm) {
    if (!userAcl.contains(userId) || 
        !userAcl[userId].resourcePermissions.contains(resource)) {
        return false;
    }
    
    userAcl[userId].resourcePermissions[resource].remove(perm);
    emit permissionChanged(userId, resource);
    return true;
}

bool AccessControl::hasPermission(const QString& userId, const QString& resource, Permission perm) const {
    if (!userAcl.contains(userId)) {
        return false;
    }
    
    const auto& entry = userAcl[userId];
    
    // Admins have all permissions
    if (entry.roles.contains(Role::ADMIN)) {
        return true;
    }
    
    if (!entry.resourcePermissions.contains(resource)) {
        return false;
    }
    
    return entry.resourcePermissions[resource].contains(perm);
}

bool AccessControl::validateAccess(const QString& userId, const QString& resource, 
                                 Permission perm, const AccessContext& context) {
    // First check basic permission
    if (!hasPermission(userId, resource, perm)) {
        return false;
    }
    
    // Then validate time-based access
    if (!validateTimeBasedAccess(resource, context.timestamp)) {
        return false;
    }
    
    // Finally validate IP-based access
    return validateIPBasedAccess(resource, context.ipAddress);
}

bool AccessControl::requestTwoFactorAuth(const QString& adminId) {
    if (!hasRole(adminId, Role::ADMIN)) {
        return false;
    }
    
    // Generate a random 6-digit code using qrand()
    int code = qrand() % 900000 + 100000;  // 生成100000-999999之间的随机数
    pendingTwoFactorAuth[adminId] = QString::number(code);
    
    emit twoFactorAuthRequired(adminId);
    return true;
}

bool AccessControl::verifyTwoFactorAuth(const QString& adminId, const QString& code) {
    if (!pendingTwoFactorAuth.contains(adminId)) {
        return false;
    }
    
    bool result = (pendingTwoFactorAuth[adminId] == code);
    if (result) {
        pendingTwoFactorAuth.remove(adminId);
    }
    return result;
}

bool AccessControl::saveACL() {
    QJsonObject root;
    QJsonObject usersObj;
    
    for (auto it = userAcl.constBegin(); it != userAcl.constEnd(); ++it) {
        QJsonObject userObj;
        
        // Save roles
        QJsonArray rolesArray;
        for (const auto& role : it.value().roles) {
            rolesArray.append(static_cast<int>(role));
        }
        userObj["roles"] = rolesArray;
        
        // Save permissions
        QJsonObject permissionsObj;
        for (auto permIt = it.value().resourcePermissions.constBegin();
             permIt != it.value().resourcePermissions.constEnd(); ++permIt) {
            QJsonArray permArray;
            for (const auto& perm : permIt.value()) {
                permArray.append(static_cast<int>(perm));
            }
            permissionsObj[permIt.key()] = permArray;
        }
        userObj["permissions"] = permissionsObj;
        
        usersObj[it.key()] = userObj;
    }
    
    root["users"] = usersObj;
    QJsonDocument doc(root);
    
    // Encrypt and save to file
    QString encrypted = encryptACL(doc);
    QFile file(QDir::current().filePath("acl.dat"));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    
    file.write(encrypted.toUtf8());
    return true;
}

bool AccessControl::loadACL() {
    QFile file(QDir::current().filePath("acl.dat"));
    if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    
    QString encrypted = QString::fromUtf8(file.readAll());
    QJsonDocument doc = decryptACL(encrypted);
    if (doc.isNull()) {
        return false;
    }
    
    userAcl.clear();
    QJsonObject root = doc.object();
    QJsonObject usersObj = root["users"].toObject();
    
    for (auto it = usersObj.constBegin(); it != usersObj.constEnd(); ++it) {
        QString userId = it.key();
        QJsonObject userObj = it.value().toObject();
        
        ACLEntry entry;
        
        // Load roles
        QJsonArray rolesArray = userObj["roles"].toArray();
        for (const auto& roleVal : rolesArray) {
            entry.roles.insert(static_cast<Role>(roleVal.toInt()));
        }
        
        // Load permissions
        QJsonObject permissionsObj = userObj["permissions"].toObject();
        for (auto permIt = permissionsObj.constBegin();
             permIt != permissionsObj.constEnd(); ++permIt) {
            QString resource = permIt.key();
            QJsonArray permArray = permIt.value().toArray();
            
            QSet<Permission> permissions;
            for (const auto& permVal : permArray) {
                permissions.insert(static_cast<Permission>(permVal.toInt()));
            }
            entry.resourcePermissions[resource] = permissions;
        }
        
        userAcl[userId] = entry;
    }
    
    return true;
}

QString AccessControl::encryptACL(const QJsonDocument& doc) const {
    // 使用简单的XOR加密，实际应用中应该使用更强的加密算法
    QByteArray data = doc.toJson();
    QByteArray key = QCryptographicHash::hash("tcpfile_secret_key", QCryptographicHash::Sha256);
    
    QByteArray encrypted;
    for (int i = 0; i < data.size(); ++i) {
        encrypted.append(data[i] ^ key[i % key.size()]);
    }
    
    return encrypted.toBase64();
}

QJsonDocument AccessControl::decryptACL(const QString& encrypted) const {
    QByteArray data = QByteArray::fromBase64(encrypted.toUtf8());
    QByteArray key = QCryptographicHash::hash("tcpfile_secret_key", QCryptographicHash::Sha256);
    
    QByteArray decrypted;
    for (int i = 0; i < data.size(); ++i) {
        decrypted.append(data[i] ^ key[i % key.size()]);
    }
    
    return QJsonDocument::fromJson(decrypted);
}

bool AccessControl::validateTimeBasedAccess(const QString& resource, const QDateTime& timestamp) const {
    // Default working hours (9:00-18:00)
    int hour = timestamp.time().hour();
    bool withinWorkHours = (hour >= 9 && hour < 18);
    
    // Special rules for specific resources
    if (resource.startsWith("confidential/")) {
        // Confidential files only accessible during core hours (10:00-16:00)
        return (hour >= 10 && hour < 16);
    } else if (resource.startsWith("public/")) {
        // Public resources accessible 24/7
        return true;
    } else if (resource.startsWith("backup/")) {
        // Backup operations only allowed during off-hours (18:00-9:00)
        return !withinWorkHours;
    }
    
    // Default to standard working hours for other resources
    return withinWorkHours;
}

bool AccessControl::validateIPBasedAccess(const QString& resource, const QHostAddress& ip) const {
    // Allow localhost access for all resources
    if (ip.isLoopback()) {
        return true;
    }
    
    // Special rules for specific resources
    if (resource.startsWith("confidential/")) {
        // Confidential files only accessible from internal network
        return ip.isInSubnet(QHostAddress("10.0.0.0"), 8) ||  // 10.0.0.0/8
               ip.isInSubnet(QHostAddress("192.168.0.0"), 16); // 192.168.0.0/16
    } else if (resource.startsWith("public/")) {
        // Public resources accessible from anywhere
        return true;
    } else if (resource.startsWith("backup/")) {
        // Backup operations only from specific backup servers
        QSet<QString> backupServers = {"192.168.1.10", "192.168.1.11", "10.0.0.5"};
        return backupServers.contains(ip.toString());
    }
    
    // Default to allowing local network access
    return ip.isInSubnet(QHostAddress("192.168.0.0"), 16) ||
           ip.isInSubnet(QHostAddress("10.0.0.0"), 8);
} 