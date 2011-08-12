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
#ifndef RECURSIVEMINIMAXSEARCH_H
#define RECURSIVEMINIMAXSEARCH_H

#include "ReversiBoard.h"

#include <QSharedPointer>
#include <QtGlobal>

class RecursiveMinimaxSearch
{
public:
    RecursiveMinimaxSearch(QSharedPointer<ReversiBoard> rootNodeBoard,quint8 maxDepth = 1);
    virtual ~RecursiveMinimaxSearch();

    qint16 doSearch();

    BoardPos getBestMove() const;

private:
    qint16 visit(QSharedPointer<ReversiBoard> board, quint8 depth, qint16 alpha, qint16 beta);
    QSharedPointer<ReversiBoard> rootNodesBoard;
    const quint8 maxDepth;
    BoardPos bestMove;
};

#endif // RECURSIVEMINIMAXSEARCH_H
