#include "RecursiveMinimaxSearch.h"

#include <QtConcurrentRun>
#include <QList>

RecursiveMinimaxSearch::RecursiveMinimaxSearch(QSharedPointer<ReversiBoard> rootNodesBoard,quint8 maxDepth) :
    rootNodesBoard(rootNodesBoard), maxDepth(maxDepth)
{
}

RecursiveMinimaxSearch::~RecursiveMinimaxSearch()
{
}

qint16 RecursiveMinimaxSearch::doSearch()
{
    return this->visit(this->rootNodesBoard,0);
}

//private
qint16 RecursiveMinimaxSearch::visit(QSharedPointer<ReversiBoard> board, quint8 depth)
{
    if (++depth > this->maxDepth)
        return board->getScore();

    const CELL_STATE whoseTurn = board->getWhoseTurn();
    const QList<BoardPos> moves = board->getValidMoves(whoseTurn);

    BoardPos bestMove = {0,0};
    qint16 bestScore;
    if (whoseTurn == WHITE_CELL)
        bestScore = -10000;
    else
        bestScore = 10000;

    //Start as many threads as we can
    QList<QFuture<qint16> > scoreResults;
    foreach(const BoardPos move, moves)
    {
        QSharedPointer<ReversiBoard> simBoard(new ReversiBoard(*board));
        simBoard->makeMove(move,whoseTurn);

        QFuture<qint16> scoreResult = QtConcurrent::run(this,&RecursiveMinimaxSearch::visit,simBoard,depth+1);
        scoreResults.append(scoreResult);
    }

    //Wait for all threads to finish, judge moves by score
    for (int i = 0; i < scoreResults.size(); i++)
    {
        QFuture<qint16> scoreResult = scoreResults[i];
        scoreResult.waitForFinished();
        const qint16 score = scoreResult.result();
        if ((whoseTurn == WHITE_CELL && score > bestScore)
                || (whoseTurn == BLACK_CELL && score < bestScore))
        {
            //qDebug() << score << "is better than" << bestScore;
            bestScore = score;
            bestMove = moves[i];
        }
    }
    if (depth == 1)
        this->bestMove = bestMove;
    return bestScore;
}

BoardPos RecursiveMinimaxSearch::getBestMove() const
{
    return this->bestMove;
}
