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
