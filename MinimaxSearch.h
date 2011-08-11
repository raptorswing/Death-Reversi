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
#ifndef MINIMAXSEARCH_H
#define MINIMAXSEARCH_H

#include <QSharedPointer>
#include <QStack>
#include <QHash>
#include <QMutex>

#include "ReversiBoard.h"
#include "MinimaxNode.h"

class MinimaxSearch
{
public:
    MinimaxSearch(QSharedPointer<ReversiBoard> rootNodeBoard,quint8 maxDepth = 1);

    qint16 doSearch();
    qint16 getFinalValue() const;

private:
    QSharedPointer<ReversiBoard> rootNodeBoard;
    quint8 maxDepth;
    QStack<QSharedPointer<MinimaxNode> > frontier;
    qint16 finalValue;

    static QHash<ReversiBoard,qint16> precomp;
    static bool precompsLoaded;
    static void loadPrecomps();
    static void storePrecomps();
};

#endif // MINIMAXSEARCH_H
