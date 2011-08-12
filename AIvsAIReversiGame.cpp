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
#include "AIvsAIReversiGame.h"

#include <QtDebug>
#include <QDateTime>

const quint16 turnDelayMS = 100;
const quint8 whiteSearchDepth = 9;
const quint8 blackSearchDepth = 9;

AIvsAIReversiGame::AIvsAIReversiGame()
{
    this->setBoard(QSharedPointer<ReversiBoard>(new ReversiBoard(8)));
    this->handleTurnTaken(WHITE_CELL,this->getBoard()->getWhoseTurn());
    qsrand(QDateTime::currentDateTime().toTime_t());
}

//public slot
void AIvsAIReversiGame::handleCellClicked(BoardPos where)
{
    Q_UNUSED(where);
    qDebug() << "Just watch the bots, pathetic human.";
}

//private slot
void AIvsAIReversiGame::handleTurnTaken(CELL_STATE byWhom, CELL_STATE nextTurn)
{
    Q_UNUSED(byWhom);
    if (this->getBoard()->isGameOver())
        return;

    if (nextTurn == WHITE_CELL)
        QTimer::singleShot(turnDelayMS,this,SLOT(makeWhiteMove()));
    else if (nextTurn == BLACK_CELL)
        QTimer::singleShot(turnDelayMS,this,SLOT(makeBlackMove()));
}

//private slot
void AIvsAIReversiGame::handleGameOver(CELL_STATE winner)
{
    Q_UNUSED(winner);
}

//private slot
void AIvsAIReversiGame::handleScoreChanged(quint16 white, quint16 black)
{
    Q_UNUSED(white);
    Q_UNUSED(black);
}

//private slot
void AIvsAIReversiGame::makeWhiteMove()
{
    this->getBoard()->calculateBestMove(WHITE_CELL,whiteSearchDepth);
    this->getBoard()->makeMove(this->getBoard()->getBestMove(),WHITE_CELL);
}

//private slot
void AIvsAIReversiGame::makeBlackMove()
{
    this->getBoard()->calculateBestMove(BLACK_CELL,blackSearchDepth);
    this->getBoard()->makeMove(this->getBoard()->getBestMove(),BLACK_CELL);
}
