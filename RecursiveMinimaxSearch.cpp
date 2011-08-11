#include "RecursiveMinimaxSearch.h"

#include <QtConcurrentRun>
#include <QList>
#include <QCoreApplication>

quint16 guiRedrawCounter = 0;
const quint16 GUI_REDRAW_INTERVAL = 1000;

RecursiveMinimaxSearch::RecursiveMinimaxSearch(QSharedPointer<ReversiBoard> rootNodesBoard,quint8 maxDepth) :
    rootNodesBoard(rootNodesBoard), maxDepth(maxDepth)
{
}

RecursiveMinimaxSearch::~RecursiveMinimaxSearch()
{
}

qint16 RecursiveMinimaxSearch::doSearch()
{
    return this->visit(this->rootNodesBoard,0,-10000,10000);
}

//private
qint16 RecursiveMinimaxSearch::visit(QSharedPointer<ReversiBoard> board, quint8 depth, qint16 alpha, qint16 beta)
{
    //If we're at maximum depth or an end state, return
    if (++depth > this->maxDepth || board->isGameOver())
        return board->getScore();

    //Hack to let Qt event loop process every once in a while.
    //We should move this someplace gui-related.
    if (++guiRedrawCounter >= GUI_REDRAW_INTERVAL)
    {
        guiRedrawCounter = 0;
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    }

    const CELL_STATE whoseTurn = board->getWhoseTurn();
    const QList<BoardPos> moves = board->getValidMoves(whoseTurn);

    BoardPos bestMove = {0,0};
    foreach(const BoardPos move, moves)
    {
        QSharedPointer<ReversiBoard> simBoard(new ReversiBoard(*board));
        simBoard->makeMove(move,whoseTurn);

        qint16 score = this->visit(simBoard,depth+1,alpha,beta);

        if (whoseTurn == WHITE_CELL)
        {
            if (score > alpha)
            {
                alpha = score;
                bestMove = move;
            }
            else if (score == alpha && qrand() % 2)
                bestMove = move;
            if (beta <= alpha)
                break;
        }
        else
        {
            if (score <= beta)
            {
                beta = score;
                bestMove = move;
            }
            else if (score == beta && qrand() % 2)
                bestMove = move;
            if (beta >= alpha)
                break;
        }

    }
    if (depth == 1)
        this->bestMove = bestMove;
    if (whoseTurn == WHITE_CELL)
        return alpha;
    else
        return beta;

}

BoardPos RecursiveMinimaxSearch::getBestMove() const
{
    return this->bestMove;
}
