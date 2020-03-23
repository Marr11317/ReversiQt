#include "case.h"

#include <QPainter>

Case::Case(QWidget *parent, State s) : QWidget(parent), _state(s)
{

}

void Case::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    switch (_state) {
    case State::User:
        painter.setBrush(Qt::NoBrush);
        painter.setPen(Qt::blue);
        break;
    case State::Bot:
        painter.setBrush(Qt::NoBrush);
        painter.setPen(Qt::blue);

        break;
    case State::Empty:
        break;
    case State::Invalid:
        break;
    default:
        break;
    }
}
