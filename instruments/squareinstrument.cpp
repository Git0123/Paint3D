#include "squareinstrument.h"
#include "../drawingboard.h"
#include "core/painter.h"
#include <QPen>
#include <QPainter>

SquareInstrument::SquareInstrument(QObject *parent) :
    AbstractInstrument(parent)
{

}


void SquareInstrument::mousePressEvent(QMouseEvent *event, DrawingBoard &board)
{
    if(event->button() == Qt::LeftButton) {
        mStartPoint = mEndPoint = event->pos();
        board.setIsInPaint(true);
        mImageCopy = QImage(*board.getImage());
        makeUndoCommand(board);
    }
}

void SquareInstrument::mouseMoveEvent(QMouseEvent *event, DrawingBoard &board)
{
    if(board.isInPaint()) {
        mEndPoint = event->pos();
        draw(board);
    }
}

void SquareInstrument::mouseReleaseEvent(QMouseEvent *event, DrawingBoard &board)
{
    if(board.isInPaint()) {
        mEndPoint = event->pos();
        draw(board);
        board.setIsInPaint(false);
    }
}


void SquareInstrument::draw(DrawingBoard &board)
{
    board.setImage(mImageCopy);
    QPainter painter(board.getImage());

    QPen pen(board.borderColor(), board.thickness());
    if (board.borderStyle() == "None") {
        pen.setStyle(Qt::NoPen);
    }
    else if (board.borderStyle() == "Dash") {
        pen.setStyle(Qt::DashLine);
    }
    else if (board.borderStyle() == "Dot") {
        pen.setStyle(Qt::DotLine);
    }
    else if (board.borderStyle() == "DashDot") {
        pen.setStyle(Qt::DashDotLine);
    }
    else {
        pen.setStyle(Qt::SolidLine);
    }
    painter.setPen(pen);

    QBrush brush(board.fillColor());
    if (board.fillStyle() == "None") {
        brush.setStyle(Qt::NoBrush);
    }
    else {
        brush.setStyle(Qt::SolidPattern);
    }
    painter.setBrush(brush);

    painter.setOpacity(board.opaqueness() / 100.0);

    if(mStartPoint != mEndPoint)
    {
        CorePainter::Painter mypainter(board.getImage());
        int x1 = mStartPoint.x() < mEndPoint.x() ? mStartPoint.x() : mEndPoint.x();
        int y1 = mStartPoint.y() < mEndPoint.y() ? mStartPoint.y() : mEndPoint.y();
        int width = abs(mStartPoint.x() - mEndPoint.x());
        int height = abs(mStartPoint.y() - mEndPoint.y());
        mypainter.drawSquare(x1, y1, width, height, board.thickness(), board.opaqueness() / 100.0, board.borderColor(), board.fillColor(),
                           board.borderStyle() != "None", board.fillStyle() != "None");
//        painter.drawRect(QRect(mStartPoint, mEndPoint));
    }

    painter.end();

    board.update();
}
