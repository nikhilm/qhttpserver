TEMPLATE = app
QT += network
QT -= gui

SOURCES = test.cpp

LIBS += -L../lib/ -lqhttpserver
INCLUDEPATH += .\
 ../src
