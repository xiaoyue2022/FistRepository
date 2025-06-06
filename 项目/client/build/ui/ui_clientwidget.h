/********************************************************************************
** Form generated from reading UI file 'clientwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENTWIDGET_H
#define UI_CLIENTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_clientwidget
{
public:
    QGridLayout *gridLayout_3;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEditip;
    QPushButton *pushButtonconnect;
    QLabel *label_3;
    QLineEdit *lineEditport;
    QProgressBar *progress_01;
    QWidget *widget;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer;
    QLabel *label_4;
    QTextEdit *textEdit;
    QWidget *widget_2;
    QGridLayout *gridLayout_2;
    QTextEdit *textEdit_write;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_5;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButtonsend;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *pushButton_2;

    void setupUi(QWidget *clientwidget)
    {
        if (clientwidget->objectName().isEmpty())
            clientwidget->setObjectName(QStringLiteral("clientwidget"));
        clientwidget->resize(686, 658);
        gridLayout_3 = new QGridLayout(clientwidget);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        label = new QLabel(clientwidget);
        label->setObjectName(QStringLiteral("label"));
        QFont font;
        font.setFamily(QStringLiteral("Agency FB"));
        font.setPointSize(14);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label, 0, 1, 1, 1);

        label_2 = new QLabel(clientwidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_3->addWidget(label_2, 1, 0, 1, 1);

        lineEditip = new QLineEdit(clientwidget);
        lineEditip->setObjectName(QStringLiteral("lineEditip"));

        gridLayout_3->addWidget(lineEditip, 1, 1, 1, 1);

        pushButtonconnect = new QPushButton(clientwidget);
        pushButtonconnect->setObjectName(QStringLiteral("pushButtonconnect"));

        gridLayout_3->addWidget(pushButtonconnect, 1, 2, 2, 1);

        label_3 = new QLabel(clientwidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_3->addWidget(label_3, 2, 0, 1, 1);

        lineEditport = new QLineEdit(clientwidget);
        lineEditport->setObjectName(QStringLiteral("lineEditport"));

        gridLayout_3->addWidget(lineEditport, 2, 1, 1, 1);

        progress_01 = new QProgressBar(clientwidget);
        progress_01->setObjectName(QStringLiteral("progress_01"));
        progress_01->setValue(24);

        gridLayout_3->addWidget(progress_01, 3, 0, 1, 4);

        widget = new QWidget(clientwidget);
        widget->setObjectName(QStringLiteral("widget"));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalSpacer_2 = new QSpacerItem(286, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(286, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 0, 1, 1);

        label_4 = new QLabel(widget);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 0, 1, 1, 1);

        textEdit = new QTextEdit(widget);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setReadOnly(true);

        gridLayout->addWidget(textEdit, 1, 0, 1, 3);


        gridLayout_3->addWidget(widget, 4, 0, 1, 4);

        widget_2 = new QWidget(clientwidget);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        gridLayout_2 = new QGridLayout(widget_2);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        textEdit_write = new QTextEdit(widget_2);
        textEdit_write->setObjectName(QStringLiteral("textEdit_write"));

        gridLayout_2->addWidget(textEdit_write, 1, 0, 1, 3);

        horizontalSpacer_4 = new QSpacerItem(89, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_4, 0, 2, 1, 1);

        label_5 = new QLabel(widget_2);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_2->addWidget(label_5, 0, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(89, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_3, 0, 0, 1, 1);


        gridLayout_3->addWidget(widget_2, 5, 0, 1, 4);

        pushButtonsend = new QPushButton(clientwidget);
        pushButtonsend->setObjectName(QStringLiteral("pushButtonsend"));

        gridLayout_3->addWidget(pushButtonsend, 6, 0, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(244, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_5, 6, 1, 1, 1);

        pushButton_2 = new QPushButton(clientwidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        gridLayout_3->addWidget(pushButton_2, 6, 2, 1, 1);


        retranslateUi(clientwidget);

        QMetaObject::connectSlotsByName(clientwidget);
    } // setupUi

    void retranslateUi(QWidget *clientwidget)
    {
        clientwidget->setWindowTitle(QApplication::translate("clientwidget", "Form", 0));
        label->setText(QApplication::translate("clientwidget", "   \351\243\236\351\270\237\346\226\207\344\273\266\344\274\240\351\200\201\345\256\242\346\210\267\347\253\257", 0));
        label_2->setText(QApplication::translate("clientwidget", "\346\234\215\345\212\241\345\231\250IP\357\274\232", 0));
        lineEditip->setText(QApplication::translate("clientwidget", "127.0.0.1", 0));
        pushButtonconnect->setText(QApplication::translate("clientwidget", "\350\277\236\346\216\245", 0));
        label_3->setText(QApplication::translate("clientwidget", "\347\253\257\345\217\243\357\274\232", 0));
        lineEditport->setText(QApplication::translate("clientwidget", "8888", 0));
        label_4->setText(QApplication::translate("clientwidget", "\345\256\236\346\227\266\347\212\266\346\200\201\347\252\227\345\217\243", 0));
        label_5->setText(QApplication::translate("clientwidget", "\350\201\212\345\244\251\346\266\210\346\201\257\346\241\206", 0));
        pushButtonsend->setText(QApplication::translate("clientwidget", "\345\217\221\351\200\201\346\266\210\346\201\257", 0));
        pushButton_2->setText(QApplication::translate("clientwidget", "\346\226\255\345\274\200\350\277\236\346\216\245", 0));
    } // retranslateUi

};

namespace Ui {
    class clientwidget: public Ui_clientwidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENTWIDGET_H
