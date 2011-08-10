#ifndef REVERSIBOARDWIDGET_H
#define REVERSIBOARDWIDGET_H

#include <QWidget>
#include "ReversiBoard.h"

class ReversiBoardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ReversiBoardWidget(QSharedPointer<ReversiBoard> board,
                                QWidget *parent = 0);

    ReversiBoardWidget(QWidget * parent = 0);

    //virtual from QWidget
    QSize sizeHint() const;

    void setBoard(QSharedPointer<ReversiBoard> board);
    QSharedPointer<ReversiBoard> getBoard() const;

protected:
    //virtual from QWidget
    void paintEvent(QPaintEvent *);

    //virtual from QWidget
    void mouseReleaseEvent(QMouseEvent *);

    //virtual from QWidget
    void mousePressEvent(QMouseEvent *);


signals:
    void cellClicked(const BoardPos cell);

public slots:

private:
    QSharedPointer<ReversiBoard> board;
    QPoint lastMousePressPos;

    bool showPossibleWhite;
    bool showPossibleBlack;

};

#endif // REVERSIBOARDWIDGET_H
