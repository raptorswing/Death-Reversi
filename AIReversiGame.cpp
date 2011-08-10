#include "AIReversiGame.h"

#include <QTimer>

const CELL_STATE humanPlayer = BLACK_CELL;
const CELL_STATE aiPlayer = WHITE_CELL;

AIReversiGame::AIReversiGame()
{
}

//public slot
void AIReversiGame::handleCellClicked(BoardPos where)
{
    if (this->getBoard()->getWhoseTurn() != humanPlayer)
        return;
    ReversiGame::handleCellClicked(where);
}

//private slot
void AIReversiGame::handleTurnTaken(CELL_STATE byWhom, CELL_STATE nextTurn)
{
    Q_UNUSED(byWhom);
    //If it's the computers turn, make that happen
    if (nextTurn == aiPlayer)
    {
        QTimer::singleShot(500,this,SLOT(makeAIMove()));
    }
}

//private slot
void AIReversiGame::handleGameOver(CELL_STATE winner)
{
    Q_UNUSED(winner);
}

//private slot
void AIReversiGame::handleScoreChanged(quint16 white, quint16 black)
{
    Q_UNUSED(white);
    Q_UNUSED(black);
}

//private slot
void AIReversiGame::makeAIMove()
{
    this->getBoard()->calculateBestMove(aiPlayer);
    //this->handleCellClicked(this->getBoard()->getBestMove());
    this->getBoard()->makeMove(this->getBoard()->getBestMove(),aiPlayer);
}
