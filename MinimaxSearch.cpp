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
#include "MinimaxSearch.h"

#include <QtDebug>

QMutex precompMutex;

//static
QHash<ReversiBoard,qint16> MinimaxSearch::precomp = QHash<ReversiBoard,qint16>();
bool MinimaxSearch::precompsLoaded = false;

MinimaxSearch::MinimaxSearch(QSharedPointer<ReversiBoard> rootNodeBoard,quint8 maxDepth) :
    rootNodeBoard(rootNodeBoard),maxDepth(maxDepth)
{

}

qint16 MinimaxSearch::doSearch()
{
    QMutexLocker lock(&precompMutex);
    if (this->precomp.contains(*rootNodeBoard))
    {
        //qDebug() << "Hit!";
        this->finalValue = this->precomp.value(*rootNodeBoard);
        return this->finalValue;
    }
    lock.unlock();


    QSharedPointer<MinimaxNode> rootNode(new MinimaxNode(rootNodeBoard));
    this->frontier.push(rootNode);

    while (!this->frontier.isEmpty())
    {
        QSharedPointer<MinimaxNode> current = this->frontier.pop();

        //calculate value
        qint16 value = current->calculateValue();

        //set it and propogate up the tree when applicable
        current->setValue(value);


        if (current->getMyDepth() >= maxDepth || current->getBoard()->isGameOver())
            continue;

        //This part is the alpha-beta pruning
        if (!current->getParent().isNull())
        {
            if (current->getBoard()->getWhoseTurn() == WHITE_CELL
                    && current->getParent()->getBoard()->getWhoseTurn() == BLACK_CELL
                    && current->getValue() > current->getParent()->getValue())
                continue;
            if (current->getBoard()->getWhoseTurn() == BLACK_CELL
                    && current->getParent()->getBoard()->getWhoseTurn() == WHITE_CELL
                    && current->getValue() < current->getParent()->getValue())
                continue;
        }

        const QList<BoardPos> moves = current->getBoard()->getValidMoves(current->getBoard()->getWhoseTurn());
        foreach(BoardPos move, moves)
        {
            QSharedPointer<ReversiBoard> boardCopy(new ReversiBoard(*current->getBoard()));
            boardCopy->makeMove(move,boardCopy->getWhoseTurn());
            QSharedPointer<MinimaxNode> childNode(new MinimaxNode(boardCopy,
                                                                  current,
                                                                  current->getMyDepth()+1));
            this->frontier.push(childNode);
        }
    }

    this->finalValue = rootNode->getValue();
    //qDebug() << "Final value" << this->getFinalValue();
    lock.relock();
    this->precomp.insert(*rootNodeBoard,this->finalValue);
    return this->finalValue;
}

qint16 MinimaxSearch::getFinalValue() const
{
    return this->finalValue;
}

//static
void MinimaxSearch::loadPrecomps()
{
    if (MinimaxSearch::precompsLoaded)
        return;
    MinimaxSearch::precompsLoaded = true;
}

//private
void MinimaxSearch::storePrecomps()
{
}
