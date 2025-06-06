#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QHostAddress>
#include <QDebug>
#include <QFileInfo>
#include "authmanager.h"
#include <QInputDialog>
#include <QJsonObject>
#include <QJsonDocument>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , loginWidget(new LoginWidget)
{
    ui->setupUi(this);
    
    // 初始化变量
    file_mes = true;
    sendSize = 0;
    datetime = new QDateTime;
    tcpServer = nullptr;
    tcpSocket = nullptr;

    // 初始化UI状态
    ui->pushButtonselect->setEnabled(false);
    ui->pushButtonsend->setEnabled(false);

    // 隐藏主窗口，显示登录窗口
    this->hide();
    loginWidget->show();

    // 连接登录成功信号
    connect(loginWidget, &LoginWidget::loginSuccessful, this, &Widget::onLoginSuccessful);

    // 连接定时器信号
    connect(&timer, &QTimer::timeout, [=]() {
        timer.stop();
        file_mes ? sendMes() : sendData();
    });
}

Widget::~Widget()
{
    delete ui;
    delete datetime;
    delete loginWidget;
    if (tcpServer) {
        tcpServer->close();
        delete tcpServer;
    }
    if (tcpSocket) {
        tcpSocket->close();
        delete tcpSocket;
    }
}

void Widget::onLoginSuccessful(const QString& username)
{
    currentLoggedInUser = username;  // 存储当前登录的用户名
    loginWidget->hide();  // 隐藏登录窗口
    this->show();        // 显示主窗口
    initializeServer();  // 初始化服务器
}

void Widget::initializeServer()
{
    // 创建并初始化服务器
    tcpServer = new QTcpServer(this);
    
    // 监听端口
    if (!tcpServer->listen(QHostAddress::Any, 8888)) {
        QMessageBox::critical(this, "错误", "服务器启动失败！");
        return;
    }
    
    setWindowTitle("服务器端口为：8888");
    
    // 等待连接
    connect(tcpServer, &QTcpServer::newConnection, this, &Widget::newConnection);
}

void Widget::newConnection() {
    // 取出建立好连接的套接字
    tcpSocket = tcpServer->nextPendingConnection();
    
    // 获取对方的IP和端口
    QString ip = tcpSocket->peerAddress().toString();
    qint16 port = tcpSocket->peerPort();
    QString temp = QString("[%1:%2]:成功连接").arg(ip).arg(port);
    
    ui->textEdit->setText(temp);
    
    // 成功连接后，启用按钮
    ui->pushButtonsend->setEnabled(true);
    ui->pushButtonselect->setEnabled(true);
    
    // 处理接收的数据
    connect(tcpSocket, &QTcpSocket::readyRead, this, &Widget::socketReadyRead);
    
    // 处理断开连接
    connect(tcpSocket, &QTcpSocket::stateChanged, this, &Widget::socketStateChanged);
}

void Widget::socketStateChanged(QAbstractSocket::SocketState socketState) {
    if (socketState == QAbstractSocket::UnconnectedState) {
        tcpSocket->close();
        tcpSocket = nullptr;
        ui->textEdit->append("客户端已断开连接");
        ui->pushButtonselect->setEnabled(false);
        ui->pushButtonsend->setEnabled(false);
    }
}

void Widget::socketReadyRead() {
    QByteArray buf = tcpSocket->readAll();
    if (QString(buf) == "file done") {
        ui->textEdit->append("文件发送完毕");
        file.close();
        ui->pushButtonselect->setEnabled(true);
        ui->pushButtonsend->setEnabled(true);
        file_mes = true;
    } else {
        ui->textEdit->append("客户端：" + QString(buf));
    }
}

void Widget::on_pushButtonselect_clicked()
{
    // 使用当前登录的用户名
    QString currentUser = currentLoggedInUser;
    
    // 如果没有用户登录，提示登录
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, tr("错误"), tr("请先登录"));
        return;
    }
    
    // 检查用户角色，确定起始目录
    QString startDir = "../";
    AuthManager* auth = AuthManager::getInstance();
    AccessControl* accessControl = auth->getAccessControl();
    
    // 检查是否为普通用户（非管理员）
    if (!accessControl->hasRole(currentUser, Role::ADMIN)) {
        // 普通用户只能访问/home目录
        startDir = "/home";
    }
    
    QString filePath = QFileDialog::getOpenFileName(this, "open", startDir);

    if(filePath.isEmpty() == false){
        // 检查普通用户是否试图访问/home以外的目录
        if (!checkUserRole(currentUser, filePath)) {
            QMessageBox::warning(this, tr("错误"), tr("无权限访问此文件"));
            return;
        }
        
        fileName.clear();
        fileSize= 0;

        //获取文件信息
        QFileInfo info(filePath);
        fileName = info.fileName(); //获取文件名字
        fileSize = info.size(); //获取文件大小

        sendSize = 0; //发送文件的大小

       //只读方式打开文件
       //指定文件的名字
       file.setFileName(filePath);

       //打开文件
       bool isOk = file.open(QIODevice::ReadOnly);
       if(false == isOk)
       {
           qDebug() << "只读方式打开文件失败";
       }

       //提示打开文件的路径
       ui->textEdit->append(filePath);

       ui->pushButtonselect->setEnabled(false);//文件一次发一个
       file_mes = false;

    }else{
        qDebug() << "选择文件路径出错";
    }

}

void Widget::on_pushButtonsend_clicked()
{
    if(file_mes == false){ //发文件
        ui->pushButtonsend->setEnabled(false);
        //发文件先发头文件信息   格式化 文件名##文件大小
        QString head = QString("%1##%2##%3").arg("filemark").arg(fileName).arg(fileSize);//格式化
        quint64 len = tcpSocket->write(head.toUtf8());
        if(len >0) //头部信息发送完毕
        {
            //防止TCP黏包 需要通过定时器延时 20 ms
            timer.start(20);
        }else{ //头部发送失败
            file.close();
            ui->pushButtonselect->setEnabled(true);
            ui->pushButtonsend->setEnabled(true);
        }
    }else{
        //获取编辑框内容
        QString head = "mesmark##";//发送一个标记 发送的是mes
        quint64 len = tcpSocket->write(head.toUtf8());
        if(len >0){
            timer.start(10);
//            qDebug()<<"沉睡10毫秒";
        }
    }
}

void Widget::sendData(){
    ui->textEdit->append("正在发送");
    qint64 len = 0;
     do
     {
        //每次发送数据的大小
        char buf[4*1024] = {0};
        len = 0;
        //往文件中读数据
        len = file.read(buf, sizeof(buf));
        //发送数据，读多少，发多少
        len = tcpSocket->write(buf, len);
        //发送的数据需要累积
        sendSize += len;
     }while(len > 0);
}

void Widget::sendMes(){

    QString str = ui->textEdit_write->toPlainText();
    //发送数据
    if(str.isEmpty() == false){
        qDebug()<<str;
        tcpSocket->write(str.toUtf8().data());
    }
    ui->textEdit_write->clear();

}

void Widget::disconnectTcp(){

    QString str = QString("[%1:]断开连接").arg(datetime->toString("hh:mm:ss ddd"));
    ui->textEdit->append(str); //显示到编辑区
    ui->pushButtonselect->setEnabled(false);
    ui->pushButtonsend->setEnabled(false);
}

void Widget::on_pushButtondis_clicked()
{
    tcpSocket->disconnectFromHost();
    tcpSocket->close();

}

void Widget::processFileRequest(QTcpSocket* socket, const QString& filename) {
    QString username = clientUsernames.value(socket);
    if (username.isEmpty()) {
        sendError(socket, "Not authenticated");
        return;
    }
    
    // 创建访问上下文
    AccessContext context;
    context.timestamp = QDateTime::currentDateTime();
    context.ipAddress = socket->peerAddress();
    context.userAgent = "TCP File Client";
    
    // 验证访问权限
    if (!validateFileAccess(username, filename, Permission::READ)) {
        sendError(socket, "Access denied");
        return;
    }
    
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        sendError(socket, "Failed to open file");
        return;
    }
    
    // 发送文件内容
    QByteArray data = file.readAll();
    socket->write(data);
    socket->flush();
}

bool Widget::validateFileAccess(const QString& username, const QString& filename, Permission perm) {
    AuthManager* auth = AuthManager::getInstance();
    
    // 获取文件信息
    QFileInfo fileInfo(filename);
    QString resource = fileInfo.absoluteFilePath();
    
    // 创建访问上下文
    AccessContext context;
    context.timestamp = QDateTime::currentDateTime();
    context.ipAddress = QHostAddress::LocalHost; // 对于本地操作使用本地地址
    context.userAgent = "TCP File Server";
    
    return auth->validateAccess(username, resource, perm, context);
}

void Widget::handleTwoFactorAuthRequired(const QString& adminId) {
    showTwoFactorDialog(adminId);
}

void Widget::showTwoFactorDialog(const QString& adminId) {
    bool ok;
    QString code = QInputDialog::getText(this, tr("Two-Factor Authentication"),
                                       tr("Please enter the 2FA code sent to your device:"),
                                       QLineEdit::Normal, QString(), &ok);
    
    if (ok && !code.isEmpty()) {
        AuthManager* auth = AuthManager::getInstance();
        if (!auth->verifyTwoFactorAuth(adminId, code)) {
            QMessageBox::warning(this, tr("Error"), tr("Invalid 2FA code"));
        }
    }
}

void Widget::handlePermissionChanged(const QString& username, const QString& resource) {
    Q_UNUSED(username);
    Q_UNUSED(resource);
    updatePermissionDisplay();
}

void Widget::updatePermissionDisplay() {
    // 更新权限显示列表
    ui->permissionList->clear();
    
    // TODO: 获取并显示所有用户的权限
    // 这里需要添加一个方法来获取所有用户的权限列表
}



void Widget::on_assignRoleButton_clicked() {
    QString adminUsername = ui->adminUsernameEdit->text();
    QString adminPassword = ui->adminPasswordEdit->text();
    QString username = ui->usernameEdit->text();
    QString roleStr = ui->roleComboBox->currentText();
    
    if (adminUsername.isEmpty() || adminPassword.isEmpty() || username.isEmpty()) {
        QMessageBox::warning(this, tr("错误"), tr("请填写所有字段"));
        return;
    }
    
    // 验证管理员身份
    AuthManager* auth = AuthManager::getInstance();
    if (!auth->login(adminUsername, adminPassword)) {
        QMessageBox::warning(this, tr("错误"), tr("管理员用户名或密码错误"));
        return;
    }
    
    // 检查是否为管理员角色
    if (!auth->checkPermission(adminUsername, "/", Permission::MODIFY_PERMISSIONS)) {
        QMessageBox::warning(this, tr("错误"), tr("只有管理员才能分配角色"));
        return;
    }
    
    Role role;
    if (roleStr == "管理员") {
        role = Role::ADMIN;
    } else {
        role = Role::USER;
    }
    
    if (auth->assignRole(adminUsername, username, role)) {
        QMessageBox::information(this, tr("成功"), tr("角色分配成功"));
        updatePermissionDisplay();
        // 清空输入框
        ui->adminUsernameEdit->clear();
        ui->adminPasswordEdit->clear();
        ui->usernameEdit->clear();
    } else {
        QMessageBox::warning(this, tr("错误"), tr("角色分配失败"));
    }
}

void Widget::sendError(QTcpSocket* socket, const QString& error) {
    QJsonObject response;
    response["status"] = "error";
    response["message"] = error;
    
    QJsonDocument doc(response);
    socket->write(doc.toJson());
    socket->flush();
}

void Widget::sendSuccess(QTcpSocket* socket, const QString& message) {
    QJsonObject response;
    response["status"] = "success";
    response["message"] = message;
    
    QJsonDocument doc(response);
    socket->write(doc.toJson());
    socket->flush();
}

bool Widget::checkUserRole(const QString& username, const QString& filepath) {
    AuthManager* auth = AuthManager::getInstance();
    AccessControl* accessControl = auth->getAccessControl();
    
    // 检查是否为管理员
    if (accessControl->hasRole(username, Role::ADMIN)) {
        // 管理员可以访问所有文件
        return true;
    }
    
    // 普通用户只能访问/home目录下的文件
    QFileInfo fileInfo(filepath);
    QString absolutePath = fileInfo.absoluteFilePath();
    
    // 在Ubuntu系统下，检查路径是否以/home开头
    if (absolutePath.startsWith("/home")) {
        return true;
    }
    
    // 如果不是/home目录下的文件，拒绝访问
    return false;
}
