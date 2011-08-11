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
