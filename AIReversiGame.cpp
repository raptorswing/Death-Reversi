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
#include "AIReversiGame.h"

#include <QTimer>
#include <QDateTime>
#include <QtGlobal>

const CELL_STATE humanPlayer = BLACK_CELL;
const CELL_STATE aiPlayer = WHITE_CELL;

AIReversiGame::AIReversiGame()
{
    this->setBoard(QSharedPointer<ReversiBoard>(new ReversiBoard(8)));
    this->handleTurnTaken(WHITE_CELL,BLACK_CELL);
    qsrand(QDateTime::currentDateTime().toTime_t());
}

//public slot
void AIReversiGame::handleCellClicked(BoardPos where)
{
    if (this->getBoard()->getWhoseTurn() != humanPlayer)
        return;
    ReversiGame::handleCellClicked(where);
}

//private slot
void AIReversiGame::handleTurnTaken(CELL_STATE byWhom, CELL_STATE nextTurn)
{
    Q_UNUSED(byWhom);
    if (this->getBoard()->isGameOver())
        return;
    //If it's the computers turn, make that happen
    if (nextTurn == aiPlayer)
    {
        QTimer::singleShot(50,this,SLOT(makeAIMove()));
    }
}

//private slot
void AIReversiGame::handleGameOver(CELL_STATE winner)
{
    Q_UNUSED(winner);
}

//private slot
void AIReversiGame::handleScoreChanged(quint16 white, quint16 black)
{
    Q_UNUSED(white);
    Q_UNUSED(black);
}

//private slot
void AIReversiGame::makeAIMove()
{
    this->getBoard()->calculateBestMove(aiPlayer,18);
    this->getBoard()->makeMove(this->getBoard()->getBestMove(),aiPlayer);
}
