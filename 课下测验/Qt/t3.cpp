#include <QWidget>
#include <QtWidgets/QApplication>
#include <QFont>
#include <QtWidgets/QPushButton>
int main(int argc, char *argv[])
 {
 QApplication app(argc, argv);
 QWidget window;
 window.resize(200, 120);
 QPushButton quit("Quit", &window);
 quit.setFont(QFont("Times", 18, QFont::Bold));
 quit.setGeometry(10, 40, 180, 40);
 QObject::connect(&quit, SIGNAL(clicked()), &app, SLOT(quit()));
 window.show();
 return app.exec();
 }