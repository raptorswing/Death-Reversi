#-------------------------------------------------
#
# Project created by QtCreator 2011-08-08T14:21:19
#
#-------------------------------------------------

QT       += core gui

TARGET = DeathReversi
TEMPLATE = app


SOURCES += main.cpp\
        GameWindow.cpp \
    ReversiBoard.cpp \
    ReversiBoardWidget.cpp \
    MinimaxSearch.cpp \
    MinimaxNode.cpp \
    ReversiGame.cpp \
    AIReversiGame.cpp

HEADERS  += GameWindow.h \
    ReversiBoard.h \
    ReversiBoardWidget.h \
    MinimaxSearch.h \
    MinimaxNode.h \
    ReversiGame.h \
    AIReversiGame.h

FORMS    += GameWindow.ui
