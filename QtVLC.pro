TEMPLATE = app
QT += gui widgets
TARGET = qtvlc
DEPENDPATH += .
INCLUDEPATH += .
LIBS += -lvlc -lX11

# Input
HEADERS += player.h
SOURCES += main.cpp player.cpp
