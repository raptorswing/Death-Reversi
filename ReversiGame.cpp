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
#include "ReversiGame.h"

#include <QtDebug>

ReversiGame::ReversiGame()
{
    this->setBoard(QSharedPointer<ReversiBoard>(new ReversiBoard(8)));
}

ReversiGame::~ReversiGame()
{

}

QSharedPointer<ReversiBoard> ReversiGame::getBoard() const
{
    return this->board;
}

//public slot
void ReversiGame::handleCellClicked(BoardPos where)
{
    this->board->makeMove(where,
                          this->board->getWhoseTurn());
}

//public slot
void ReversiGame::setBlackAIDepth(quint8)
{
}

//public slot
void ReversiGame::setWhiteAIDepth(quint8)
{
}


//private slot
void ReversiGame::handleTurnTaken(CELL_STATE byWhom, CELL_STATE nextTurn)
{
    Q_UNUSED(byWhom);
    Q_UNUSED(nextTurn);
    //this->turnTaken(byWhom,nextTurn);
    qDebug() << this->board->getScore();
}

//private slot
void ReversiGame::handleGameOver(CELL_STATE winner)
{
    Q_UNUSED(winner);
    //this->gameOver(winner);
}

//private slot
void ReversiGame::handleScoreChanged(quint16 white, quint16 black)
{
    Q_UNUSED(white);
    Q_UNUSED(black);
    //this->scoreChanged(white,black);
}

//private
void ReversiGame::setBoard(QSharedPointer<ReversiBoard> nBoard)
{
    if (nBoard.isNull())
        return;

    ReversiBoard * raw = nBoard.data();
    connect(raw,
            SIGNAL(moveMade(CELL_STATE,CELL_STATE)),
            this,
            SLOT(handleTurnTaken(CELL_STATE,CELL_STATE)));
    connect(raw,
            SIGNAL(moveMade(CELL_STATE,CELL_STATE)),
            this,
            SIGNAL(turnTaken(CELL_STATE,CELL_STATE)));
    connect(raw,
            SIGNAL(gameOver(CELL_STATE)),
            this,
            SLOT(handleGameOver(CELL_STATE)));
    connect(raw,
            SIGNAL(gameOver(CELL_STATE)),
            this,
            SIGNAL(gameOver(CELL_STATE)));
    connect(raw,
            SIGNAL(countChanged(quint16,quint16)),
            this,
            SLOT(handleScoreChanged(quint16,quint16)));
    connect(raw,
            SIGNAL(countChanged(quint16,quint16)),
            this,
            SIGNAL(scoreChanged(quint16,quint16)));

    this->board = nBoard;
}



