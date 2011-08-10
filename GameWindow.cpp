#include "GameWindow.h"
#include "ui_GameWindow.h"

#include "ReversiGame.h"
#include "AIReversiGame.h"

#include <QtDebug>

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
}

GameWindow::~GameWindow()
{
    delete ui;
}


//private slot
void GameWindow::handleCountChange(quint16 white, quint16 black)
{
    this->ui->statusBar->showMessage("White:" + QString::number(white) + " Black:" + QString::number(black));
}

//private slot
void GameWindow::handleGameOver(CELL_STATE)
{
    quint16 whiteScore = this->game->getBoard()->getWhiteCount();
    quint16 blackScore = this->game->getBoard()->getBlackCount();
    if (whiteScore > blackScore)
        this->ui->statusBar->showMessage("White wins " + QString::number(whiteScore) + " - " + QString::number(blackScore) + "!");
    else if (blackScore > whiteScore)
        this->ui->statusBar->showMessage("Black wins " + QString::number(blackScore) + " - " + QString::number(whiteScore) + "!");
    else
        this->ui->statusBar->showMessage("Tie! " + QString::number(blackScore) + " - " + QString::number(whiteScore) + "!");
}

//private slot
void GameWindow::on_toolButton_clicked()
{
    /*
    this->ui->widget->getBoard()->calculateBestMove(this->ui->widget->getBoard()->getWhoseTurn());
    this->ui->widget->update();
    */
}

//private slot
void GameWindow::on_actionVs_AI_triggered()
{
    this->setGame(QSharedPointer<ReversiGame>(new AIReversiGame()));
}

//private slot
void GameWindow::on_actionVs_human_triggered()
{
    this->setGame(QSharedPointer<ReversiGame>(new ReversiGame()));
}

//private
void GameWindow::setGame(QSharedPointer<ReversiGame> game)
{
    if (game.isNull())
        return;

    this->ui->widget->setBoard(game->getBoard());

    ReversiGame * raw = game.data();
    connect(this->ui->widget,
            SIGNAL(cellClicked(BoardPos)),
            raw,
            SLOT(handleCellClicked(BoardPos)));
    connect(raw,
            SIGNAL(gameOver(CELL_STATE)),
            this,
            SLOT(handleGameOver(CELL_STATE)));
            this->game = game;
    connect(raw,
            SIGNAL(scoreChanged(quint16,quint16)),
            this,
            SLOT(handleCountChange(quint16,quint16)));
}


