#include "MinimaxSearch.h"

#include <QtDebug>

//static
QHash<ReversiBoard,qint16> MinimaxSearch::precomp = QHash<ReversiBoard,qint16>();

MinimaxSearch::MinimaxSearch(QSharedPointer<ReversiBoard> rootNodeBoard,quint8 maxDepth) :
    rootNodeBoard(rootNodeBoard),maxDepth(maxDepth)
{

}

qint16 MinimaxSearch::doSearch()
{
    if (this->precomp.contains(*rootNodeBoard))
    {
        //qDebug() << "Hit!";
        this->finalValue = this->precomp.value(*rootNodeBoard);
        return this->finalValue;
    }


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
    this->precomp.insert(*rootNodeBoard,this->finalValue);
    return this->finalValue;
}

qint16 MinimaxSearch::getFinalValue() const
{
    return this->finalValue;
}
