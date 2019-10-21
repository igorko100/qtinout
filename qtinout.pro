QT += widgets core sql

HEADERS       = mainwindow.h \
    nfc.h \
    digitalclock.h \
    addnewuserdialog.h \
    showlasteventdialog.h
SOURCES       = mainwindow.cpp \
                main.cpp \
    nfc.cpp \
    digitalclock.cpp \
    addnewuserdialog.cpp \
    showlasteventdialog.cpp

# install
target.path = .
INSTALLS += target
LIBS += -lnfc
DEFINES += QT_NO_OPENGL

DISTFILES += \
    home.png \
    wait.png \
    work.png

FORMS += \
    addnewuserdialog.ui \
    showlasteventdialog.ui
