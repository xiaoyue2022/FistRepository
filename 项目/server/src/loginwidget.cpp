#include "loginwidget.h"
#include "ui_loginwidget.h"
#include <QTimer>
#include <QDebug>

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget),
    authManager(AuthManager::getInstance()),
    messageTimer(new QTimer(this))
{
    ui->setupUi(this);
    
    // 设置消息自动清除定时器
    messageTimer->setInterval(3000);  // 3秒后清除消息
    connect(messageTimer, &QTimer::timeout, this, &LoginWidget::updateMessage);
    
    // 设置回车键触发登录
    connect(ui->passwordEdit, &QLineEdit::returnPressed, this, &LoginWidget::on_loginButton_clicked);
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::on_loginButton_clicked()
{
    QString username = ui->usernameEdit->text().trimmed();
    QString password = ui->passwordEdit->text();
    
    if (username.isEmpty() || password.isEmpty()) {
        showMessage("Please enter both username and password");
        return;
    }
    
    currentUsername = username;
    
    // 检查用户是否被锁定
    if (authManager->isUserLocked(username)) {
        int remainingTime = authManager->getRemainingLockTime(username);
        showMessage(QString("已锁定. 请 %1 秒后重试").arg(remainingTime));
        return;
    }
    
    // 尝试登录
    if (authManager->login(username, password)) {
        showMessage("登录成功!", false);
        currentUsername = username;  // 保存当前用户名
        QTimer::singleShot(1000, this, [this]() {
            emit loginSuccessful(currentUsername);  // 发送登录成功信号并传递用户名
        });
        clearInputs();
    } else {
        showMessage("Invalid username or password");
    }
}

void LoginWidget::on_registerButton_clicked()
{
    QString username = ui->usernameEdit->text().trimmed();
    QString password = ui->passwordEdit->text();
    
    if (username.isEmpty() || password.isEmpty()) {
        showMessage("Please enter both username and password");
        return;
    }
    
    // 尝试注册
    if (authManager->registerUser(username, password)) {
        showMessage("注册成功!", false);
        clearInputs();
    } else {
        showMessage("账号已经存在或注册失败");
    }
}

void LoginWidget::clearInputs()
{
    ui->usernameEdit->clear();
    ui->passwordEdit->clear();
    ui->usernameEdit->setFocus();
}

void LoginWidget::showMessage(const QString &message, bool isError)
{
    ui->messageLabel->setStyleSheet(isError ? "color: red;" : "color: green;");
    ui->messageLabel->setText(message);
    messageTimer->start();
}

void LoginWidget::updateMessage()
{
    // 如果用户被锁定，更新剩余锁定时间
    if (!currentUsername.isEmpty() && authManager->isUserLocked(currentUsername)) {
        int remainingTime = authManager->getRemainingLockTime(currentUsername);
        if (remainingTime > 0) {
            showMessage(QString("已锁定. 请 %1 秒后重试").arg(remainingTime));
            return;
        }
    }
    
    // 清除消息
    ui->messageLabel->clear();
    messageTimer->stop();
}