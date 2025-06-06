/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QTabWidget *tabWidget;
    QWidget *serverTab;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *portEdit;
    QPushButton *startButton;
    QTextEdit *textEdit;
    QTextEdit *textEdit_write;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButtonselect;
    QPushButton *pushButtonsend;
    QPushButton *pushButtondis;
    QWidget *permissionsTab;
    QVBoxLayout *verticalLayout_3;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLineEdit *adminUsernameEdit;
    QLabel *label_6;
    QLineEdit *adminPasswordEdit;
    QLabel *label_3;
    QLineEdit *usernameEdit;
    QLabel *label_5;
    QComboBox *roleComboBox;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *assignRoleButton;
    QListWidget *permissionList;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(800, 600);
        verticalLayout = new QVBoxLayout(Widget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        titleLabel = new QLabel(Widget);
        titleLabel->setObjectName(QStringLiteral("titleLabel"));
        titleLabel->setAlignment(Qt::AlignCenter);
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        font.setWeight(75);
        titleLabel->setFont(font);

        verticalLayout->addWidget(titleLabel);

        tabWidget = new QTabWidget(Widget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        serverTab = new QWidget();
        serverTab->setObjectName(QStringLiteral("serverTab"));
        verticalLayout_2 = new QVBoxLayout(serverTab);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(serverTab);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        portEdit = new QLineEdit(serverTab);
        portEdit->setObjectName(QStringLiteral("portEdit"));

        horizontalLayout->addWidget(portEdit);

        startButton = new QPushButton(serverTab);
        startButton->setObjectName(QStringLiteral("startButton"));

        horizontalLayout->addWidget(startButton);


        verticalLayout_2->addLayout(horizontalLayout);

        textEdit = new QTextEdit(serverTab);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setReadOnly(true);

        verticalLayout_2->addWidget(textEdit);

        textEdit_write = new QTextEdit(serverTab);
        textEdit_write->setObjectName(QStringLiteral("textEdit_write"));

        verticalLayout_2->addWidget(textEdit_write);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        pushButtonselect = new QPushButton(serverTab);
        pushButtonselect->setObjectName(QStringLiteral("pushButtonselect"));

        horizontalLayout_3->addWidget(pushButtonselect);

        pushButtonsend = new QPushButton(serverTab);
        pushButtonsend->setObjectName(QStringLiteral("pushButtonsend"));

        horizontalLayout_3->addWidget(pushButtonsend);

        pushButtondis = new QPushButton(serverTab);
        pushButtondis->setObjectName(QStringLiteral("pushButtondis"));

        horizontalLayout_3->addWidget(pushButtondis);


        verticalLayout_2->addLayout(horizontalLayout_3);

        tabWidget->addTab(serverTab, QString());
        permissionsTab = new QWidget();
        permissionsTab->setObjectName(QStringLiteral("permissionsTab"));
        verticalLayout_3 = new QVBoxLayout(permissionsTab);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_2 = new QLabel(permissionsTab);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        adminUsernameEdit = new QLineEdit(permissionsTab);
        adminUsernameEdit->setObjectName(QStringLiteral("adminUsernameEdit"));

        gridLayout->addWidget(adminUsernameEdit, 0, 1, 1, 1);

        label_6 = new QLabel(permissionsTab);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 1, 0, 1, 1);

        adminPasswordEdit = new QLineEdit(permissionsTab);
        adminPasswordEdit->setObjectName(QStringLiteral("adminPasswordEdit"));
        adminPasswordEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(adminPasswordEdit, 1, 1, 1, 1);

        label_3 = new QLabel(permissionsTab);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        usernameEdit = new QLineEdit(permissionsTab);
        usernameEdit->setObjectName(QStringLiteral("usernameEdit"));

        gridLayout->addWidget(usernameEdit, 2, 1, 1, 1);

        label_5 = new QLabel(permissionsTab);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 3, 0, 1, 1);

        roleComboBox = new QComboBox(permissionsTab);
        roleComboBox->setObjectName(QStringLiteral("roleComboBox"));

        gridLayout->addWidget(roleComboBox, 3, 1, 1, 1);


        verticalLayout_3->addLayout(gridLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        assignRoleButton = new QPushButton(permissionsTab);
        assignRoleButton->setObjectName(QStringLiteral("assignRoleButton"));

        horizontalLayout_2->addWidget(assignRoleButton);


        verticalLayout_3->addLayout(horizontalLayout_2);

        permissionList = new QListWidget(permissionsTab);
        permissionList->setObjectName(QStringLiteral("permissionList"));

        verticalLayout_3->addWidget(permissionList);

        tabWidget->addTab(permissionsTab, QString());

        verticalLayout->addWidget(tabWidget);


        retranslateUi(Widget);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "TCP\346\226\207\344\273\266\346\234\215\345\212\241\345\231\250", 0));
        titleLabel->setText(QApplication::translate("Widget", "\351\243\236\351\270\237", 0));
        label->setText(QApplication::translate("Widget", "\347\253\257\345\217\243\357\274\232", 0));
        portEdit->setText(QApplication::translate("Widget", "8080", 0));
        startButton->setText(QApplication::translate("Widget", "\345\220\257\345\212\250\346\234\215\345\212\241\345\231\250", 0));
        textEdit_write->setPlaceholderText(QApplication::translate("Widget", "\345\234\250\346\255\244\350\276\223\345\205\245\346\266\210\346\201\257...", 0));
        pushButtonselect->setText(QApplication::translate("Widget", "\351\200\211\346\213\251\346\226\207\344\273\266", 0));
        pushButtonsend->setText(QApplication::translate("Widget", "\345\217\221\351\200\201", 0));
        pushButtondis->setText(QApplication::translate("Widget", "\346\226\255\345\274\200\350\277\236\346\216\245", 0));
        tabWidget->setTabText(tabWidget->indexOf(serverTab), QApplication::translate("Widget", "\346\234\215\345\212\241\345\231\250", 0));
        label_2->setText(QApplication::translate("Widget", "\347\256\241\347\220\206\345\221\230\347\224\250\346\210\267\345\220\215\357\274\232", 0));
        label_6->setText(QApplication::translate("Widget", "\347\256\241\347\220\206\345\221\230\345\257\206\347\240\201\357\274\232", 0));
        label_3->setText(QApplication::translate("Widget", "\347\224\250\346\210\267\345\220\215\357\274\232", 0));
        label_5->setText(QApplication::translate("Widget", "\350\247\222\350\211\262\357\274\232", 0));
        roleComboBox->clear();
        roleComboBox->insertItems(0, QStringList()
         << QApplication::translate("Widget", "\347\256\241\347\220\206\345\221\230", 0)
         << QApplication::translate("Widget", "\346\231\256\351\200\232\347\224\250\346\210\267", 0)
        );
        assignRoleButton->setText(QApplication::translate("Widget", "\345\210\206\351\205\215\350\247\222\350\211\262", 0));
        tabWidget->setTabText(tabWidget->indexOf(permissionsTab), QApplication::translate("Widget", "\346\235\203\351\231\220\347\256\241\347\220\206", 0));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
