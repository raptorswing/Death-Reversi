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
