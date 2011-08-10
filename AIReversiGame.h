#ifndef AIREVERSIGAME_H
#define AIREVERSIGAME_H

#include "ReversiGame.h"

class AIReversiGame : public ReversiGame
{
    Q_OBJECT
public:
    explicit AIReversiGame();

signals:

public slots:
    virtual void handleCellClicked(BoardPos where);

private slots:
    virtual void handleTurnTaken(CELL_STATE byWhom, CELL_STATE nextTurn);
    virtual void handleGameOver(CELL_STATE winner);
    virtual void handleScoreChanged(quint16 white, quint16 black);
    virtual void makeAIMove();

};

#endif // AIREVERSIGAME_H
