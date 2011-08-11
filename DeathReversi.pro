#Copyright 2011 Spencer Clark
#----------------------------------------------------------------------
#This file is part of DeathReversi.
#
#DeathReversi is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.
#
#DeathReversi is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with DeathReversi.  If not, see <http://www.gnu.org/licenses/>.
#------------------------------------------------------------------------

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
    AIReversiGame.cpp \
    RecursiveMinimaxSearch.cpp

HEADERS  += GameWindow.h \
    ReversiBoard.h \
    ReversiBoardWidget.h \
    MinimaxSearch.h \
    MinimaxNode.h \
    ReversiGame.h \
    AIReversiGame.h \
    RecursiveMinimaxSearch.h

FORMS    += GameWindow.ui
