#include <QWidget>
#include <QtWidgets/QApplication>
#include <QFont>
#include <QtWidgets/QPushButton>
#include <QLCDNumber>
#include <QSlider>
#include <QVBoxLayout>
#include <QGridLayout>

class LCDRange : public QWidget
{
public:
    LCDRange(QWidget *parent = 0);
};

LCDRange::LCDRange(QWidget *parent) : QWidget(parent)
{
    QLCDNumber *lcd = new QLCDNumber(2);
    lcd->setSegmentStyle(QLCDNumber::Filled);
    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setRange(0,99);
    slider->setValue(0);
    connect(slider, SIGNAL(valueChanged(int)), lcd, SLOT(display(int)));
    // Bug fix: Add space between 'new' and 'QVBoxLayout'
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(lcd);
    layout->addWidget(slider);
    setLayout(layout);
}

// Bug fix: Add space between 'public' and 'QWidget'
class MyWidget : public QWidget
{
public:
    MyWidget(QWidget *parent = 0);
};

MyWidget::MyWidget(QWidget *parent) : QWidget(parent)
{
    // Bug fix: Add space between 'new' and 'QPushButton'
    QPushButton *quit = new QPushButton(tr("Quit"));
    quit->setFont(QFont("Times", 18, QFont::Bold));
    connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));
    // Bug fix: Add space between 'new' and 'QGridLayout'
    QGridLayout *grid = new QGridLayout;
    for (int row = 0; row < 3; ++row) {
        for (int column = 0; column < 3; ++column) {
            LCDRange *lcdRange = new LCDRange;
            grid->addWidget(lcdRange, row, column);
        }
    }
    // Bug fix: Add space between 'new' and 'QVBoxLayout'
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(quit);
    // Bug fix: Use correct method to add layout
    layout->addLayout(grid);
    setLayout(layout);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // Bug fix: Add space between 'MyWidget' and 'widget'
    MyWidget widget;
    widget.show();
    return app.exec();
}