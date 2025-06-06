#include "widget.h"
#include <QApplication>
#include "authmanager.h"
#include "accesscontrol.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // 初始化认证管理器并创建默认管理员账户
    AuthManager* auth = AuthManager::getInstance();
    
    // 创建默认管理员账户（如果不存在）
    auth->registerUser("admin", "admin123", Role::ADMIN);
    
    // 创建一个测试普通用户（如果不存在）
    auth->registerUser("user1", "user123", Role::USER);
    
    Widget w;
    return a.exec();
}
