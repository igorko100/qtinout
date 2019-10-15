QT += widgets

HEADERS       = mainwindow.h
SOURCES       = mainwindow.cpp \
                main.cpp

# install
target.path = .
INSTALLS += target
DEFINES += QT_NO_OPENGL
