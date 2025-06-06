#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include "authmanager.h"

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

signals:
    void loginSuccessful(const QString& username);

private slots:
    void on_loginButton_clicked();
    void on_registerButton_clicked();
    void updateMessage();

private:
    Ui::LoginWidget *ui;
    AuthManager *authManager;
    QTimer *messageTimer;
    QString currentUsername;

    void clearInputs();
    void showMessage(const QString &message, bool isError = true);
};

#endif // LOGINWIDGET_H