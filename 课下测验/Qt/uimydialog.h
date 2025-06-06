#ifndef UI_MYDIALOG_H
#define UI_MYDIALOG_H
#include <QtCore/QVariant>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTextBrowser>

class Ui_MyDialog {
public:
    QGridLayout *gridLayout;
    QTextBrowser *textBrowser;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    void setupUi(QDialog *MyDialog) {
        // 自动生成的UI设置代码
    }
    void retranslateUi(QDialog *MyDialog) {
        // 自动生成的翻译代码
    }
};

namespace Ui {
    class MyDialog : public Ui_MyDialog {};
}
#endif