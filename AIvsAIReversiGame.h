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
#ifndef AIVSAIREVERSIGAME_H
#define AIVSAIREVERSIGAME_H

#include "ReversiGame.h"

#include <QTimer>

class AIvsAIReversiGame : public ReversiGame
{
    Q_OBJECT
public:
    explicit AIvsAIReversiGame();

signals:

public slots:
    virtual void handleCellClicked(BoardPos where);

private slots:
    virtual void handleTurnTaken(CELL_STATE byWhom, CELL_STATE nextTurn);
    virtual void handleGameOver(CELL_STATE winner);
    virtual void handleScoreChanged(quint16 white, quint16 black);
    void makeWhiteMove();
    void makeBlackMove();

};

#endif // AIVSAIREVERSIGAME_H
