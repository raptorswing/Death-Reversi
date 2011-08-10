#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>

#include "ReversiBoard.h"
#include "ReversiGame.h"

namespace Ui {
    class GameWindow;
}

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();

private slots:
    void handleCountChange(quint16 white, quint16 black);

    void handleGameOver(CELL_STATE);

    void on_toolButton_clicked();

    void on_actionVs_AI_triggered();

    void on_actionVs_human_triggered();

private:
    void setGame(QSharedPointer<ReversiGame> game);
    Ui::GameWindow *ui;
    QSharedPointer<ReversiGame> game;
};

#endif // GAMEWINDOW_H
