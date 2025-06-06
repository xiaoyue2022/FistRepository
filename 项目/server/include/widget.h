#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer> //监听套接字
#include <QTcpSocket> //通讯套接字
#include <QFile>
#include <QTimer>
#include <QDateTime>
#include "loginwidget.h"
#include "accesscontrol.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void sendData(); //发送文件数据
    void sendMes(); //发送消息
    void disconnectTcp();//断开连接

private slots:
    void on_pushButtonselect_clicked();
    
    void on_pushButtonsend_clicked();
    
    void on_pushButtondis_clicked();
    void onLoginSuccessful(const QString& username);
    void newConnection();
    void socketStateChanged(QAbstractSocket::SocketState socketState);
    void socketReadyRead();
    void handleTwoFactorAuthRequired(const QString& adminId);
    void handlePermissionChanged(const QString& username, const QString& resource);
    void on_assignRoleButton_clicked();

private:
    Ui::Widget *ui;
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    LoginWidget *loginWidget;

    QFile file; //文件对象
    QString fileName; //文件名字
    qint64 fileSize; //文件大小
    qint64 sendSize; //已发送文件的大小
    bool file_mes;  //发送消息还是发送文件 真为消息 假为文件
    QTimer timer; //定时器
    QDateTime *datetime;//当前时间

    QMap<QTcpSocket*, QString> clientUsernames;  // 保存socket对应的用户名
    QString currentLoggedInUser;  // 当前登录的用户名
    
    void initializeServer();
    void processFileRequest(QTcpSocket* socket, const QString& filename);
    void sendError(QTcpSocket* socket, const QString& error);
    void sendSuccess(QTcpSocket* socket, const QString& message);
    void updatePermissionDisplay();
    void showTwoFactorDialog(const QString& adminId);
    bool validateFileAccess(const QString& username, const QString& filename, Permission perm);
    bool checkUserRole(const QString& username, const QString& filepath);
};
#endif // WIDGET_H














