#ifndef REVERSIGAME_H
#define REVERSIGAME_H

#include <QSharedPointer>
#include <QObject>

#include "ReversiBoard.h"

class ReversiGame : public QObject
{
    Q_OBJECT
public:
    ReversiGame();
    virtual ~ReversiGame();

    QSharedPointer<ReversiBoard> getBoard() const;

signals:
    void turnTaken(CELL_STATE byWhom, CELL_STATE nextTurn);
    void scoreChanged(quint16 white, quint16 black);
    void gameOver(CELL_STATE winner);

public slots:
    virtual void handleCellClicked(BoardPos where);

private slots:
    virtual void handleTurnTaken(CELL_STATE byWhom, CELL_STATE nextTurn);
    virtual void handleGameOver(CELL_STATE winner);
    virtual void handleScoreChanged(quint16 white, quint16 black);


protected:
    void setBoard(QSharedPointer<ReversiBoard> nBoard);

private:
    QSharedPointer<ReversiBoard> board;
};

#endif // REVERSIGAME_H
