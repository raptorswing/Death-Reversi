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
#ifndef AIREVERSIGAME_H
#define AIREVERSIGAME_H

#include "ReversiGame.h"

class AIReversiGame : public ReversiGame
{
    Q_OBJECT
public:
    explicit AIReversiGame(CELL_STATE humanPlayer=WHITE_CELL);

signals:

public slots:
    virtual void handleCellClicked(BoardPos where);
    virtual void setBlackAIDepth(quint8 depth);
    virtual void setWhiteAIDepth(quint8 depth);

private slots:
    virtual void handleTurnTaken(CELL_STATE byWhom, CELL_STATE nextTurn);
    virtual void handleGameOver(CELL_STATE winner);
    virtual void handleScoreChanged(quint16 white, quint16 black);
    virtual void makeAIMove();

private:
    CELL_STATE aiPlayer;
    CELL_STATE humanPlayer;

};

#endif // AIREVERSIGAME_H
