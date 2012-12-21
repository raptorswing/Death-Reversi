//Copyright 2011 Spencer Clark
/*
---------------------------------------------------------------------
This file is part of DeathReversi.

DeathReversi is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

DeathReversi is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with DeathReversi.  If not, see <http://www.gnu.org/licenses/>.
---------------------------------------------------------------------
*/
#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>

#include "ReversiBoard.h"
#include "ReversiGame.h"
#include "AIOptionsWidget.h"

namespace Ui {
    class GameWindow;
}

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();

private slots:
    void handleCountChange(quint16 white, quint16 black);

    void handleGameOver(CELL_STATE);

    void on_actionVs_human_triggered();

    void on_actionAI_vs_AI_triggered();

    void on_actionPlay_as_White_triggered();

    void on_actionPlay_as_Black_triggered();

    void on_actionAI_Settings_triggered();

    void handleAIOptionChange(AIOptions options);

    void on_actionExit_triggered();

private:
    void setGame(QSharedPointer<ReversiGame> game);
    Ui::GameWindow *ui;
    QSharedPointer<ReversiGame> game;
    AIOptions options;
};

#endif // GAMEWINDOW_H
