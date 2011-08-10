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
#ifndef MINIMAXNODE_H
#define MINIMAXNODE_H

#include <QSharedPointer>

#include "ReversiBoard.h"

class MinimaxNode
{
public:
    MinimaxNode(QSharedPointer<ReversiBoard> board,
                QSharedPointer<MinimaxNode> parent = QSharedPointer<MinimaxNode>(),
                quint8 myDepth=0);

    QSharedPointer<ReversiBoard> getBoard() const;
    QSharedPointer<MinimaxNode> getParent() const;

    qint16 getValue() const;
    void setValue(qint16 nValue);
    qint16 calculateValue();

    quint8 getMyDepth() const;

private:
    QSharedPointer<ReversiBoard> board;
    QSharedPointer<MinimaxNode> parent;
    quint8 myDepth;
    qint16 value;
};

#endif // MINIMAXNODE_H
