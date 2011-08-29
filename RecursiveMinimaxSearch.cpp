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
#include "RecursiveMinimaxSearch.h"

#include <QtConcurrentRun>
#include <QList>
#include <QCoreApplication>
#include <QtDebug>

quint16 guiRedrawCounter = 0;
const quint16 GUI_REDRAW_INTERVAL = 8000;

RecursiveMinimaxSearch::RecursiveMinimaxSearch(QSharedPointer<ReversiBoard> rootNodesBoard,quint8 maxDepth) :
    rootNodesBoard(rootNodesBoard), maxDepth(maxDepth)
{
    BoardPos bestMove = {0,0};
    this->bestMove = bestMove;
}

RecursiveMinimaxSearch::~RecursiveMinimaxSearch()
{
}

qint16 RecursiveMinimaxSearch::doSearch()
{
    const qint16 estimatedBestScore = this->visit(this->rootNodesBoard,0,-10000,10000);
    return estimatedBestScore;
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

        qint16 score = this->visit(simBoard,depth,alpha,beta);

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
        else if (whoseTurn == BLACK_CELL)
        {
            if (score < beta)
            {
                beta = score;
                bestMove = move;
            }
            else if (score == beta && qrand() % 2)
                bestMove = move;

            if (beta <= alpha)
                break;
        }

    }

    /*
      If we've returned from all of our recursion and are back at level 1 of the tree,
      then we can say that one move seems better than the others
    */
    if (depth == 1)
        this->bestMove = bestMove;

    /*
      Return the score of the board configuration created by optimal-play by both players if the
      so-called best move is taken
    */
    if (whoseTurn == WHITE_CELL)
        return alpha;
    else
        return beta;
}

BoardPos RecursiveMinimaxSearch::getBestMove() const
{
    return this->bestMove;
}
