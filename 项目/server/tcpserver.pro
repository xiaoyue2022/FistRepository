QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# GmSSL library
unix {
    INCLUDEPATH += /usr/local/include
    LIBS += -L/usr/local/lib -lgmssl
}

win32 {
    INCLUDEPATH += C:/GmSSL/include
    LIBS += -LC:/GmSSL/lib -lgmssl
}

SOURCES += \
    src/widget.cpp \
    src/loginwidget.cpp \
    src/authmanager.cpp \
    src/accesscontrol.cpp \
    src/auditlog.cpp \
    src/securefiletransfer.cpp \
    src/main.cpp

HEADERS += \
    include/widget.h \
    include/loginwidget.h \
    include/authmanager.h \
    include/accesscontrol.h \
    include/auditlog.h \
    include/securefiletransfer.h

FORMS += \
    ui/widget.ui \
    ui/loginwidget.ui

INCLUDEPATH += include

OBJECTS_DIR = build/obj
MOC_DIR = build/moc
UI_DIR = build/ui
RCC_DIR = build/qrc
DESTDIR = bin

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target 