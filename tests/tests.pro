TEMPLATE = app

QT += network
QT -= gui

SOURCES = test.cpp

LIBS += -L../lib/

win32 {
    debug: LIBS += -lqhttpserverd
    else: LIBS += -lqhttpserver
} else {
    LIBS += -lqhttpserver
}

INCLUDEPATH += ../src
