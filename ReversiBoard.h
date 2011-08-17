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
#ifndef REVERSIBOARD_H
#define REVERSIBOARD_H

#include <QtGlobal>
#include <QList>
#include <QSharedPointer>
#include <QObject>

enum CELL_STATE
{
    EMPTY_CELL=0,
    WHITE_CELL=1,
    BLACK_CELL=2
};

struct BoardPos
{
    quint8 x;
    quint8 y;
};

class ReversiBoard : public QObject
{
    Q_OBJECT
public:
    ReversiBoard(quint8 size=8);
    ReversiBoard(const ReversiBoard&);
    ~ReversiBoard();

    quint16 getWhiteCount() const;
    quint16 getBlackCount() const;
    quint8 getWhiteCornerCount() const;
    quint8 getBlackCornerCount() const;
    quint16 getWhiteEdgeCount() const;
    quint16 getBlackEdgeCount() const;
    qint16 getScore() const;

    QList<BoardPos> getValidMoves(CELL_STATE forWhom) const;
    bool isValidMove(BoardPos pos,CELL_STATE forWhom,QList<BoardPos> * flips=0) const;
    bool makeMove(BoardPos pos, CELL_STATE forWhom);

    quint8 getBoardSize() const;

    bool isCellOccupied(BoardPos pos) const;

    CELL_STATE getCell(BoardPos pos) const;

    CELL_STATE getWhoseTurn() const;

    bool isGameOver() const;

    CELL_STATE getWinningColor() const;

    BoardPos getBestMove() const;

    void calculateBestMove(CELL_STATE forWhom, quint8 levels);


signals:
    void boardChanged();
    void moveMade(CELL_STATE byWhom, CELL_STATE nextTurn);
    void countChanged(quint16 white, quint16 black);
    void gameOver(CELL_STATE winner);


private:
    void initializeBoard();
    quint16 xy2index(BoardPos pos) const;
    quint16 xy2index(quint8 x, quint8 y) const;
    CELL_STATE getEnemyOf(CELL_STATE) const;
    QList<BoardPos> getCellsBetween(BoardPos p1, BoardPos p2) const;
    void setCell(BoardPos pos, CELL_STATE color);
    void incrementCount(CELL_STATE color);
    void decrementCount(CELL_STATE color);
    quint8 boardSize;
    CELL_STATE * board;
    CELL_STATE whoseTurn;

    quint16 whiteCount;
    quint16 blackCount;
    quint8 whiteCornerCount;
    quint8 blackCornerCount;
    quint16 whiteEdgeCount;
    quint16 blackEdgeCount;
    bool boolGameOver;

    BoardPos bestMove;

    //We restrict assignment operator for now.
    //ReversiBoard& operator=(ReversiBoard& other);


};

inline uint qHash(const ReversiBoard & b)
{
    const uint toRet = (b.getWhiteCount()*1000 - b.getBlackCount()*10)*(b.getWhiteCount() + b.getBlackCount());
    return toRet;
}

inline bool operator==(const ReversiBoard & b1, const ReversiBoard & b2)
{
    if (b1.getBoardSize() != b2.getBoardSize())
        return false;
    else if (b1.getWhoseTurn() != b2.getWhoseTurn())
        return false;
    else if (b1.getWhiteCount() != b2.getWhiteCount())
        return false;
    else if (b1.getBlackCount() != b2.getBlackCount())
        return false;

    for (quint8 x = 0; x < b1.getBoardSize(); x++)
    {
        for (quint8 y = 0; y < b1.getBoardSize(); y++)
        {
            const BoardPos temp = {x,y};
            if (b1.getCell(temp) != b2.getCell(temp))
                return false;
        }
    }
    return true;
}

#endif // REVERSIBOARD_H
