#include "ReversiBoard.h"

#include <cstring>
#include <QtDebug>
#include <QtConcurrentRun>
#include <QCoreApplication>

#include "MinimaxSearch.h"

ReversiBoard::ReversiBoard(quint8 size) :
    QObject(0),boardSize(size), board(0), whiteCount(0), blackCount(0), boolGameOver(false)
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

qint16 ReversiBoard::getScore() const
{
    return this->getWhiteCount() - this->getBlackCount();
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

void ReversiBoard::calculateBestMove(CELL_STATE forWhom)
{
    BoardPos bestMove = {0,0};
    qint16 currentValue;
    if (forWhom == WHITE_CELL)
        currentValue = -5000;
    else
        currentValue = 5000;
    QList<BoardPos> moves = this->getValidMoves(forWhom);

    const bool useThreads = true;
    if (useThreads)
    {
        QList<MinimaxSearch *> tempSearches;
        QList<QFuture<qint16> > results;
        foreach(BoardPos move, moves)
        {
            QSharedPointer<ReversiBoard> simBoard(new ReversiBoard(*this));
            simBoard->makeMove(move,forWhom);
            MinimaxSearch * tempSearch = new MinimaxSearch(simBoard,9);
            tempSearches.append(tempSearch);
            QFuture<qint16> result = QtConcurrent::run(tempSearch,&MinimaxSearch::doSearch);
            results.append(result);
        }
        foreach(QFuture<qint16> result, results)
        {
            while (result.isRunning())
                QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        }

        for (int i = 0; i < results.size(); i++)
        {
            QFuture<qint16> result = results[i];
            result.waitForFinished();
            qint16 value = result.result();
            const BoardPos move = moves[i];
            if ((forWhom == WHITE_CELL && value > currentValue)
                    || (forWhom == BLACK_CELL && value < currentValue))
            {
                bestMove = move;
                currentValue = value;
            }
        }
    }
    else
    {
        foreach(BoardPos move, moves)
        {
            QSharedPointer<ReversiBoard> simBoard(new ReversiBoard(*this));
            simBoard->makeMove(move,forWhom);
            MinimaxSearch search(simBoard,8);
            search.doSearch();
            qint16 value = search.getFinalValue();
            if ((forWhom == WHITE_CELL && value > currentValue)
                    || (forWhom == BLACK_CELL && value < currentValue))
            {
                bestMove = move;
                currentValue = value;
            }
        }
    }
    this->bestMove = bestMove;
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
