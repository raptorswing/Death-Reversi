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
#include <QtDebug>

quint8 aiSearchDepth = 10;

AIReversiGame::AIReversiGame(CELL_STATE humanPlayer) :
    humanPlayer(humanPlayer)
{
    if (humanPlayer == WHITE_CELL)
        this->aiPlayer = BLACK_CELL;
    else
        this->aiPlayer = WHITE_CELL;

    this->setBoard(QSharedPointer<ReversiBoard>(new ReversiBoard(8)));
    this->handleTurnTaken(WHITE_CELL,this->getBoard()->getWhoseTurn());
    qsrand(QDateTime::currentDateTime().toTime_t());
}

//public slot
void AIReversiGame::handleCellClicked(BoardPos where)
{
    if (this->getBoard()->getWhoseTurn() != this->humanPlayer)
        return;
    ReversiGame::handleCellClicked(where);
}

//public slot
void AIReversiGame::setBlackAIDepth(quint8 depth)
{
    if (this->aiPlayer == BLACK_CELL)
        aiSearchDepth = depth;
}

//public slot
void AIReversiGame::setWhiteAIDepth(quint8 depth)
{
    if (this->aiPlayer == WHITE_CELL)
        aiSearchDepth = depth;
}

//private slot
void AIReversiGame::handleTurnTaken(CELL_STATE byWhom, CELL_STATE nextTurn)
{
    qDebug() << this->getBoard()->getScore();
    Q_UNUSED(byWhom);
    if (this->getBoard()->isGameOver())
        return;
    //If it's the computers turn, make that happen
    if (nextTurn == this->aiPlayer)
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
    this->getBoard()->calculateBestMove(this->aiPlayer,aiSearchDepth);
    this->getBoard()->makeMove(this->getBoard()->getBestMove(),this->aiPlayer);
    //qDebug() << "Current score:" << this->getBoard()->getScore();
}
