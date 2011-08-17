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
#include "ReversiBoard.h"

#include <cstring>
#include <QtDebug>
#include <QtConcurrentRun>
#include <QCoreApplication>

#include "MinimaxSearch.h"
#include "RecursiveMinimaxSearch.h"

const qint8 cornerCountScoreWorth = 00;
const qint8 edgeCountScoreWorth = 2;

ReversiBoard::ReversiBoard(quint8 size) :
    QObject(0),boardSize(size), board(0), whiteCount(0), blackCount(0), boolGameOver(false), whiteCornerCount(0),
    blackCornerCount(0), whiteEdgeCount(0),blackEdgeCount(0)
{
    this->initializeBoard();
}

ReversiBoard::ReversiBoard(const ReversiBoard & other) :
    QObject(0)
{
    this->boardSize = other.getBoardSize();
    this->whoseTurn = other.getWhoseTurn();
    this->whiteCount = other.getWhiteCount();
    this->blackCount = other.getBlackCount();
    this->boolGameOver = other.isGameOver();
    this->whiteCornerCount = other.getWhiteCornerCount();
    this->blackCornerCount = other.getBlackCornerCount();
    this->whiteEdgeCount = other.getWhiteEdgeCount();
    this->blackEdgeCount = other.getBlackEdgeCount();

    const quint16 numCells = this->boardSize*this->boardSize;
    this->board = new CELL_STATE[numCells];
    memcpy(this->board,other.board,numCells*sizeof(CELL_STATE));
}

ReversiBoard::~ReversiBoard()
{
    if (this->board != 0)
        delete this->board;
}

quint16 ReversiBoard::getWhiteCount() const
{
    return this->whiteCount;
}

quint16 ReversiBoard::getBlackCount() const
{
    return this->blackCount;
}

quint8 ReversiBoard::getWhiteCornerCount() const
{
    return this->whiteCornerCount;
}

quint8 ReversiBoard::getBlackCornerCount() const
{
    return this->blackCornerCount;
}

quint16 ReversiBoard::getWhiteEdgeCount() const
{
    return this->whiteEdgeCount;
}

quint16 ReversiBoard::getBlackEdgeCount() const
{
    return this->blackEdgeCount;
}

qint16 ReversiBoard::getScore() const
{
    const qint16 basic = this->getWhiteCount() - this->getBlackCount();
    const qint16 edges = this->getWhiteEdgeCount() * edgeCountScoreWorth
            - this->getBlackEdgeCount() * edgeCountScoreWorth;
    const qint16 score = basic
            + (cornerCountScoreWorth*this->getWhiteCornerCount())
            - (cornerCountScoreWorth*this->getBlackCornerCount())
            + edges;

    /*
    if ((score > 0 && basic < 0)
            || (score < 0 && basic > 0))
    {
        qDebug() << "Mismatch";
    }
    */

    return score;
}

QList<BoardPos> ReversiBoard::getValidMoves(CELL_STATE forWhom) const
{
    QList<BoardPos> toRet;
    for (quint8 x = 0; x < this->getBoardSize(); x++)
    {
        for (quint8 y = 0; y < this->getBoardSize(); y++)
        {
            BoardPos pos = {x,y};
            if (this->isValidMove(pos,forWhom))
                toRet.append(pos);
        }
    }

    return toRet;
}

bool ReversiBoard::isValidMove(BoardPos pos,CELL_STATE forWhom, QList<BoardPos> * flips) const
{
    Q_UNUSED(flips);
    if (forWhom == EMPTY_CELL)
        return false;
    if (this->isCellOccupied(pos))
        return false;

    const CELL_STATE enemy = this->getEnemyOf(forWhom);

    bool valid = false;

    //Check moving left
    {
        bool gotEnemy = false;
        for (qint16 x = pos.x-1; x >= 0; x--)
        {
            const BoardPos consider = {x,pos.y};
            const CELL_STATE considerCell = this->getCell(consider);
            //Found empty cell
            if (considerCell == EMPTY_CELL)
                break;
            //Found enemy cell
            else if (considerCell == enemy)
                gotEnemy = true;
            //Found friendly cell
            else
            {
                if (gotEnemy)
                {
                    valid = true;
                    if (flips)
                        flips->append(this->getCellsBetween(consider,pos));
                }
                break;
            }
        }
    } //End left-moving check

    //Check moving right
    {
        bool gotEnemy = false;
        for (quint8 x = pos.x+1; x < this->getBoardSize(); x++)
        {
            const BoardPos consider = {x,pos.y};
            const CELL_STATE considerCell = this->getCell(consider);
            if (considerCell == EMPTY_CELL)
                break;
            else if (considerCell == enemy)
                gotEnemy = true;
            else
            {
                if (gotEnemy)
                {
                    valid = true;
                    if (flips)
                        flips->append(this->getCellsBetween(consider,pos));
                }
                break;
            }
        }
    } //End right-moving check

    //Check moving up
    {
        bool gotEnemy = false;
        for (quint8 y = pos.y+1; y < this->getBoardSize(); y++)
        {
            const BoardPos consider = {pos.x,y};
            const CELL_STATE considerCell = this->getCell(consider);
            if (considerCell == EMPTY_CELL)
                break;
            else if (considerCell == enemy)
                gotEnemy = true;
            else
            {
                if (gotEnemy)
                {
                    valid = true;
                    if (flips)
                        flips->append(this->getCellsBetween(consider,pos));
                }
                break;
            }
        }
    }//End up-moving check

    //Check moving down
    {
        bool gotEnemy = false;
        for (qint16 y = pos.y-1; y >= 0; y--)
        {
            const BoardPos consider = {pos.x,y};
            const CELL_STATE considerCell = this->getCell(consider);
            if (considerCell == EMPTY_CELL)
                break;
            else if (considerCell == enemy)
                gotEnemy = true;
            else
            {
                if (gotEnemy)
                {
                    valid = true;
                    if (flips)
                        flips->append(this->getCellsBetween(consider,pos));
                }
                break;
            }
        }
    }//End up-moving check

    //Diagonal up-right check
    {
        bool gotEnemy = false;
        quint8 x = pos.x+1;
        quint8 y = pos.y+1;
        while (x < this->getBoardSize() && y < this->getBoardSize())
        {
            const BoardPos consider = {x,y};
            const CELL_STATE considerCell = this->getCell(consider);
            if (considerCell == EMPTY_CELL)
                break;
            else if (considerCell == enemy)
                gotEnemy = true;
            else
            {
                if (gotEnemy)
                {
                    valid = true;
                    if (flips)
                        flips->append(this->getCellsBetween(consider,pos));
                }
                break;
            }
            ++x;
            ++y;
        }
    } //End diagonal up-right check

    //Diagonal up-left check
    {
        bool gotEnemy = false;
        qint16 x = pos.x-1;
        quint8 y = pos.y+1;
        while (x >= 0 && y < this->getBoardSize())
        {
            const BoardPos consider = {x,y};
            const CELL_STATE considerCell = this->getCell(consider);
            if (considerCell == EMPTY_CELL)
                break;
            else if (considerCell == enemy)
                gotEnemy = true;
            else
            {
                if (gotEnemy)
                {
                    valid = true;
                    if (flips)
                        flips->append(this->getCellsBetween(consider,pos));
                }
                break;
            }
            --x;
            ++y;
        }
    }//End diagonal up-left check

    //Diagonal down-right check
    {
        bool gotEnemy = false;
        quint8 x = pos.x+1;
        qint16 y = pos.y-1;
        while (x < this->getBoardSize()&& y >= 0)
        {
            const BoardPos consider = {x,y};
            const CELL_STATE considerCell = this->getCell(consider);
            if (considerCell == EMPTY_CELL)
                break;
            else if (considerCell == enemy)
                gotEnemy = true;
            else
            {
                if (gotEnemy)
                {
                    valid = true;
                    if (flips)
                        flips->append(this->getCellsBetween(consider,pos));
                }
                break;
            }
            ++x;
            --y;
        }
    }//End diagonal down-right check

    //Diagonal down-left check
    {
        bool gotEnemy = false;
        qint16 x = pos.x-1;
        qint16 y = pos.y-1;
        while (x >= 0 && y >= 0)
        {
            const BoardPos consider = {x,y};
            const CELL_STATE considerCell = this->getCell(consider);
            if (considerCell == EMPTY_CELL)
                break;
            else if (considerCell == enemy)
                gotEnemy = true;
            else
            {
                if (gotEnemy)
                {
                    valid = true;
                    if (flips)
                        flips->append(this->getCellsBetween(consider,pos));
                }
                break;
            }
            --x;
            --y;
        }
    }//End diagonal down-left check

    return valid;
}

bool ReversiBoard::makeMove(BoardPos pos, CELL_STATE forWhom)
{
    if (forWhom != this->getWhoseTurn())
    {
        qDebug() << "It is not that player's turn";
        return false;
    }

    QList<BoardPos> flips;
    if (!this->isValidMove(pos,forWhom,&flips))
        return false;

    const quint8 boardSize = this->getBoardSize();
    //Update corner counts
    if ((pos.x == 0 && pos.y == 0) || (pos.x == 0 && pos.y == boardSize-1)
            || (pos.x == boardSize-1 && pos.y == 0) || (pos.x == boardSize-1 && pos.y == boardSize-1))
    {
        if (forWhom == WHITE_CELL)
            this->whiteCornerCount++;
        else
            this->blackCornerCount++;
    }


    //do the move with flips
    foreach(BoardPos theirPos, flips)
    {
        this->setCell(theirPos,forWhom);
        this->incrementCount(forWhom);
        this->decrementCount(this->getEnemyOf(forWhom));
    }
    this->setCell(pos,forWhom);
    this->incrementCount(forWhom);

    this->whoseTurn = this->getEnemyOf(this->getWhoseTurn());
    if (this->getValidMoves(this->getWhoseTurn()).isEmpty())
    {
        this->whoseTurn = this->getEnemyOf(this->getWhoseTurn());
        if (this->getValidMoves(this->getWhoseTurn()).isEmpty())
        {
            this->whoseTurn = EMPTY_CELL;
            this->boolGameOver = true;
            this->gameOver(this->getWinningColor());
        }
    }

    //Update edge counts
    quint16 wCount = 0;
    quint16 bCount = 0;
    for (quint8 x = 0; x < boardSize; x++)
    {
        for (quint8 y = 0; y < boardSize; y++)
        {
            BoardPos pos = {x,y};
            if (pos.x == 0 || pos.y == 0 || pos.x == boardSize-1 || pos.y == boardSize-1)
            {
                const CELL_STATE owner = this->getCell(pos);
                if (owner == WHITE_CELL)
                {
                    ++wCount;
                    //qDebug() << pos.x << pos.y << "is white";
                }
                else if (owner == BLACK_CELL)
                {
                    ++bCount;
                    //qDebug() << pos.x << pos.y << "is black";
                }
            }
        }
    }
    this->whiteEdgeCount = wCount;
    this->blackEdgeCount = bCount;

    this->moveMade(forWhom,this->getWhoseTurn());
    return true;
}

quint8 ReversiBoard::getBoardSize() const
{
    return this->boardSize;
}

bool ReversiBoard::isCellOccupied(BoardPos pos) const
{
    return (this->getCell(pos) != EMPTY_CELL);
}

CELL_STATE ReversiBoard::getCell(BoardPos pos) const
{
    return this->board[this->xy2index(pos)];
}

CELL_STATE ReversiBoard::getWhoseTurn() const
{
    return this->whoseTurn;
}

bool ReversiBoard::isGameOver() const
{
    return this->boolGameOver;
}

CELL_STATE ReversiBoard::getWinningColor() const
{
    if (this->getWhiteCount() > this->getBlackCount())
        return BLACK_CELL;
    else if (this->getWhiteCount() < this->getBlackCount())
        return WHITE_CELL;
    else
        return EMPTY_CELL;

}

BoardPos ReversiBoard::getBestMove() const
{
    return this->bestMove;
}

void ReversiBoard::calculateBestMove(CELL_STATE forWhom,quint8 levels)
{
    Q_UNUSED(forWhom);

    QSharedPointer<ReversiBoard> board(new ReversiBoard(*this));
    RecursiveMinimaxSearch search(board,levels);
    search.doSearch();
    //qDebug() << "For player" << forWhom;
    //qDebug() << "Best move has score" << bestScore;
    this->bestMove = search.getBestMove();
    //qDebug() << this->bestMove.x << this->bestMove.y;
}

//private
void ReversiBoard::initializeBoard()
{
    if (this->board != 0)
    {
        delete this->board;
        this->board = 0;
    }

    const quint16 numCells = this->boardSize*this->boardSize;
    this->board = new CELL_STATE[numCells];

    for (quint8 x = 0; x < this->boardSize; x++)
    {
        for (quint8 y = 0; y < this->boardSize; y++)
        {
            const BoardPos pos = {x,y};
            this->board[this->xy2index(pos)] = EMPTY_CELL;
        }
    }

    quint8 size = this->getBoardSize();

    //Initialize board positions
    const BoardPos ul = {size/2-1,size/2};
    const BoardPos ur = {size/2,size/2};
    const BoardPos bl = {size/2-1,size/2-1};
    const BoardPos br = {size/2,size/2-1};
    //const BoardPos temp = {5,3};


    this->board[this->xy2index(ul)] = WHITE_CELL;
    this->board[this->xy2index(ur)] = BLACK_CELL;
    this->board[this->xy2index(bl)] = BLACK_CELL;
    this->board[this->xy2index(br)] = WHITE_CELL;


    //this->board[this->xy2index(temp)] = WHITE_CELL;

    this->whoseTurn = BLACK_CELL;
    this->whiteCount = 2;
    this->blackCount = 2;
    this->boolGameOver = false;

    this->boardChanged();
    this->countChanged(this->getWhiteCount(),this->getBlackCount());
    //this->calculateBestMove(this->getWhoseTurn());
}

//private
quint16 ReversiBoard::xy2index(BoardPos pos) const
{
    return pos.y*this->getBoardSize() + pos.x;
}

//private
quint16 ReversiBoard::xy2index(quint8 x, quint8 y) const
{
    return y*this->getBoardSize() + x;
}


//private
CELL_STATE ReversiBoard::getEnemyOf(CELL_STATE color) const
{
    if (color == WHITE_CELL)
        return BLACK_CELL;
    else if (color == BLACK_CELL)
        return WHITE_CELL;
    return EMPTY_CELL;
}

//private
QList<BoardPos> ReversiBoard::getCellsBetween(BoardPos p1, BoardPos p2) const
{
    QList<BoardPos> toRet;
    qint8 dx;
    qint8 dy;
    if (p2.x < p1.x)
        dx = 1;
    else if (p2.x > p1.x)
        dx = -1;
    else
        dx = 0;

    if (p2.y < p1.y)
        dy = 1;
    else if (p2.y > p1.y)
        dy = -1;
    else
        dy = 0;

    BoardPos start = {p2.x + dx, p2.y + dy};
    while (start.x != p1.x || start.y != p1.y)
    {
        toRet.append(start);
        start.x += dx;
        start.y += dy;
    }

    return toRet;
}

//private
void ReversiBoard::setCell(BoardPos pos, CELL_STATE color)
{
    this->board[this->xy2index(pos)] = color;
    this->boardChanged();
}

//private
void ReversiBoard::incrementCount(CELL_STATE color)
{
    if (color == EMPTY_CELL)
        return;
    else if (color == BLACK_CELL)
        this->blackCount++;
    else
        this->whiteCount++;
    this->countChanged(this->getWhiteCount(),this->getBlackCount());
}

//private
void ReversiBoard::decrementCount(CELL_STATE color)
{
    if (color == EMPTY_CELL)
        return;
    else if (color == BLACK_CELL)
        this->blackCount--;
    else
        this->whiteCount--;
    this->countChanged(this->getWhiteCount(),this->getBlackCount());
}


