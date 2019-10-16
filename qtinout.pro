QT += widgets

HEADERS       = mainwindow.h \
    nfc.h
SOURCES       = mainwindow.cpp \
                main.cpp \
    nfc.cpp

# install
target.path = .
INSTALLS += target
LIBS += -lnfc
DEFINES += QT_NO_OPENGL

DISTFILES += \
    home.png \
    wait.png \
    work.png
