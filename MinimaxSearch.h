#ifndef MINIMAXSEARCH_H
#define MINIMAXSEARCH_H

#include <QSharedPointer>
#include <QStack>
#include <QHash>

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
};

#endif // MINIMAXSEARCH_H
