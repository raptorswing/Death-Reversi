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
#include "GameWindow.h"
#include "ui_GameWindow.h"

#include "ReversiGame.h"
#include "AIReversiGame.h"
#include "AIvsAIReversiGame.h"

#include <QtDebug>

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
}

GameWindow::~GameWindow()
{
    delete ui;
}


//private slot
void GameWindow::handleCountChange(quint16 white, quint16 black)
{
    this->ui->statusBar->showMessage("White:" + QString::number(white) + " Black:" + QString::number(black));
}

//private slot
void GameWindow::handleGameOver(CELL_STATE)
{
    quint16 whiteScore = this->game->getBoard()->getWhiteCount();
    quint16 blackScore = this->game->getBoard()->getBlackCount();
    if (whiteScore > blackScore)
        this->ui->statusBar->showMessage("White wins " + QString::number(whiteScore) + " - " + QString::number(blackScore) + "!");
    else if (blackScore > whiteScore)
        this->ui->statusBar->showMessage("Black wins " + QString::number(blackScore) + " - " + QString::number(whiteScore) + "!");
    else
        this->ui->statusBar->showMessage("Tie! " + QString::number(blackScore) + " - " + QString::number(whiteScore) + "!");
}

//private slot
void GameWindow::on_actionVs_human_triggered()
{
    this->setGame(QSharedPointer<ReversiGame>(new ReversiGame()));
}

//private slot
void GameWindow::on_actionAI_vs_AI_triggered()
{
    this->setGame(QSharedPointer<ReversiGame>(new AIvsAIReversiGame()));
}

//private slot
void GameWindow::on_actionPlay_as_White_triggered()
{
    this->setGame(QSharedPointer<ReversiGame>(new AIReversiGame(WHITE_CELL)));
}

//private slot
void GameWindow::on_actionPlay_as_Black_triggered()
{
    this->setGame(QSharedPointer<ReversiGame>(new AIReversiGame(BLACK_CELL)));
}

//private
void GameWindow::setGame(QSharedPointer<ReversiGame> game)
{
    if (game.isNull())
        return;

    this->ui->widget->setBoard(game->getBoard());

    ReversiGame * raw = game.data();
    connect(this->ui->widget,
            SIGNAL(cellClicked(BoardPos)),
            raw,
            SLOT(handleCellClicked(BoardPos)));
    connect(raw,
            SIGNAL(gameOver(CELL_STATE)),
            this,
            SLOT(handleGameOver(CELL_STATE)));
            this->game = game;
    connect(raw,
            SIGNAL(scoreChanged(quint16,quint16)),
            this,
            SLOT(handleCountChange(quint16,quint16)));
}
