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
#include "MinimaxNode.h"

MinimaxNode::MinimaxNode(QSharedPointer<ReversiBoard> board,
                         QSharedPointer<MinimaxNode> parent,
                         quint8 myDepth) :
    board(board), parent(parent), myDepth(myDepth), value(0)
{
    if (this->getBoard()->getWhoseTurn() == WHITE_CELL)
        this->value = -5000;
    else
        this->value = 5000;
}

QSharedPointer<ReversiBoard> MinimaxNode::getBoard() const
{
    return this->board;
}

QSharedPointer<MinimaxNode> MinimaxNode::getParent() const
{
    return this->parent;
}

qint16 MinimaxNode::getValue() const
{
    return this->value;
}

void MinimaxNode::setValue(qint16 nValue)
{
    if ((this->getBoard()->getWhoseTurn() == WHITE_CELL && nValue < this->value)
            || (this->getBoard()->getWhoseTurn() == BLACK_CELL && nValue > this->value))
        return;

    this->value = nValue;

    if (this->parent.isNull())
        return;

    if (this->parent->getBoard()->getWhoseTurn() == WHITE_CELL)
    {
        if (value > this->parent->getValue())
            this->parent->setValue(value);
    }
    else
    {
        if (value < this->parent->getValue())
            this->parent->setValue(value);
    }
}

qint16 MinimaxNode::calculateValue()
{
    //run some calulation on our board and give it a ranking
    qint16 value = this->board->getWhiteCount() - this->board->getBlackCount();

    const quint8 boardSize = this->board->getBoardSize();
    const BoardPos tl = {0,boardSize-1};
    const BoardPos tr = {boardSize-1,boardSize-1};
    const BoardPos bl = {0,0};
    const BoardPos br = {boardSize-1,0};

    CELL_STATE tlv = this->board->getCell(tl);
    CELL_STATE trv = this->board->getCell(tr);
    CELL_STATE blv = this->board->getCell(bl);
    CELL_STATE brv = this->board->getCell(br);

    const qint16 cornerReward = 5;

    if (tlv == WHITE_CELL)
        value += cornerReward;
    else if (tlv == BLACK_CELL)
        value -= cornerReward;

    if (trv == WHITE_CELL)
        value += cornerReward;
    else if (trv == BLACK_CELL)
        value -= cornerReward;

    if (blv == WHITE_CELL)
        value += cornerReward;
    else if (blv == BLACK_CELL)
        value -= cornerReward;

    if (brv == WHITE_CELL)
        value += cornerReward;
    else if (brv == BLACK_CELL)
        value -= cornerReward;

    return value;
}

quint8 MinimaxNode::getMyDepth() const
{
    return this->myDepth;
}
