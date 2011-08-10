#include "ReversiBoardWidget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QtDebug>
#include "MinimaxSearch.h"

ReversiBoardWidget::ReversiBoardWidget(QSharedPointer<ReversiBoard> board,
                                       QWidget *parent) :
    QWidget(parent), showPossibleWhite(false), showPossibleBlack(false)
{
    this->setBoard(board);
}

ReversiBoardWidget::ReversiBoardWidget(QWidget *parent) :
    QWidget(parent), showPossibleWhite(false), showPossibleBlack(false)
{

}

//virtual from QWidget
QSize ReversiBoardWidget::sizeHint() const
{
    return QSize(800,800);
}

void ReversiBoardWidget::setBoard(QSharedPointer<ReversiBoard> board)
{
    this->board = board;
    ReversiBoard * raw = board.data();
    connect(raw,
            SIGNAL(moveMade(CELL_STATE,CELL_STATE)),
            this,
            SLOT(update()));

    this->update();
}

QSharedPointer<ReversiBoard> ReversiBoardWidget::getBoard() const
{
    return this->board;
}

//protected
//virtual from QWidget
void ReversiBoardWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (this->board.isNull())
    {
        painter.fillRect(this->rect(),Qt::gray);
        painter.drawText(this->rect(),"No Game.",QTextOption(Qt::AlignCenter));
        return;
    }
    const QColor bgColor = QColor(0,0,0,255);
    const QColor boardColor = Qt::gray;
    const QColor gridColor = Qt::black;
    const QColor whiteChipColor = Qt::white;
    const QColor blackChipColor = Qt::black;

    const quint16 size = qMin(this->width(),this->height())-1;
    const quint8 numCells = this->board->getBoardSize();
    const quint16 cellSize = size / numCells;

    //Draw background
    //painter.fillRect(this->rect(),bgColor);
    if (this->board->getWhoseTurn() == WHITE_CELL)
        painter.fillRect(this->rect(),Qt::white);
    else
        painter.fillRect(this->rect(),Qt::black);

    //Flip the y-axis
    painter.scale(1,-1.0);
    painter.translate(0,-1*size);

    //Center the board
    painter.translate((this->width() - numCells*cellSize)/2,
                      -1*(this->height() - numCells*cellSize)/2);

    //draw board
    painter.fillRect(0,0,numCells*cellSize,numCells*cellSize,boardColor);


    painter.setPen(gridColor);
    //Draw Vertical Grid Lines
    for (quint8 x = 0; x <= numCells; x++)
    {
        painter.drawLine(x*cellSize,0,x*cellSize,cellSize*numCells);
    }

    //Draw Horizontal Grid Lines
    for (quint8 y = 0; y <= numCells; y++)
    {
        painter.drawLine(0,y*cellSize,cellSize*numCells,y*cellSize);
    }

    //Draw pieces
    for (quint8 x = 0; x < numCells; x++)
    {
        for (quint8 y = 0; y < numCells; y++)
        {
            const BoardPos pos = {x,y};
            CELL_STATE cell = this->board->getCell(pos);

            if (cell == WHITE_CELL)
            {
                painter.setPen(whiteChipColor);
                painter.setBrush(QBrush(whiteChipColor));
                painter.drawEllipse(QPoint(cellSize*x + cellSize/2,cellSize*y + cellSize/2),cellSize/2-2,cellSize/2-2);
            }
            else if (cell == BLACK_CELL)
            {
                painter.setPen(blackChipColor);
                painter.setBrush(QBrush(blackChipColor));
                painter.drawEllipse(QPoint(cellSize*x + cellSize/2,cellSize*y + cellSize/2),cellSize/2-2,cellSize/2-2);
            }
        }
    }

    if (this->getBoard()->getWhoseTurn() == WHITE_CELL && !this->showPossibleWhite)
        return;
    else if (this->getBoard()->getWhoseTurn() == BLACK_CELL && !this->showPossibleBlack)
        return;
    else if (this->getBoard()->getWhoseTurn() == EMPTY_CELL)
        return;
    //draw valid moves for the player whose turn it is
    painter.setPen(Qt::green);
    painter.setBrush(QBrush(whiteChipColor));
    if (this->board->getWhoseTurn() == BLACK_CELL)
        painter.setBrush(blackChipColor);
    for (quint8 x = 0; x < numCells; x++)
    {
        for (quint8 y = 0; y < numCells; y++)
        {
            BoardPos pos = {x,y};
            if (!this->board->isValidMove(pos,this->board->getWhoseTurn()))
                continue;
            painter.drawEllipse(QPoint(cellSize*x + cellSize/2, cellSize*y + cellSize/2),5,5);
        }
    }


    /*
    if (this->getBoard()->getWhoseTurn() == BLACK_CELL && !this->getBoard()->isGameOver())
        this->board->makeMove(bestMove,this->board->getWhoseTurn());
        */
    //qDebug() << "Best Move" << bestMove.x << bestMove.y << "Score" << currentValue;
    BoardPos bestMove = this->getBoard()->getBestMove();
    painter.setBrush(QBrush(Qt::red));
    painter.drawEllipse(QPoint(cellSize*bestMove.x + cellSize/2, cellSize*bestMove.y + cellSize/2),5,5);
}


//protected
//virtual from QWidget
void ReversiBoardWidget::mouseReleaseEvent(QMouseEvent * event)
{
    //If the press and release points are very different, assume a drag
    if ((event->pos() - this->lastMousePressPos).manhattanLength() > 4)
        return;
    QPoint pos = event->pos();

    //We have to do the same transformation for mouse clicks that we use for drawing.
    //"Essential geometry"
    const quint16 size = qMin(this->width(),this->height())-1;
    const quint8 numCells = this->board->getBoardSize();
    const quint16 cellSize = size / numCells;
    QPoint transformedPos = pos;
    transformedPos.setY(transformedPos.y()*-1);
    transformedPos.setY(transformedPos.y() + size);
    transformedPos.setX(transformedPos.x() - (this->width() - numCells*cellSize)/2);
    transformedPos.setY(transformedPos.y() + (this->height() - numCells*cellSize)/2);


    //If they didn't click the board, we don't care
    if (transformedPos.x() < 0 || transformedPos.y() < 0 || transformedPos.x() > numCells*cellSize || transformedPos.y() > numCells*cellSize)
        return;

    //Now, what cell did they click?
    quint8 xCell = transformedPos.x() / cellSize;
    quint8 yCell = transformedPos.y() / cellSize;
    const BoardPos boardPos = {xCell,yCell};

    //Emit the "cell has been clicked" signal
    this->cellClicked(boardPos);
}

//protected
//virtual from QWidget
void ReversiBoardWidget::mousePressEvent(QMouseEvent * event)
{
    this->lastMousePressPos = event->pos();
}
