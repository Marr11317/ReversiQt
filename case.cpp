#include "case.h"

#include <QAbstractButton>
#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QStyle>
#include <QStyleOptionSlider>
#include <QtDebug>


Tile::Tile(int xpos, int ypos, QWidget *parent, TileState s) :
    QAbstractButton(parent),
    _tileState(s),
    _xpos(xpos),
    _ypos(ypos),
    _emptyColor(Qt::darkGray),
    _botColor(QColor::fromRgb(255, 187, 0)),
    _userColor(QColor::fromRgb(255, 0, 47)),
    _color(colorFromState(s))
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setAttribute(Qt::WA_Hover, true); // Receive mouse enter and mouse leave event
//    QSizePolicy pol;
//    pol.setHeightForWidth(true);
//    setSizePolicy(pol);
}

void Tile::setTileState(const TileState &state)
{
    _tileState = state;

    QPropertyAnimation *animation = new QPropertyAnimation(this, "bestColor");
    animation->setDuration(350);
    //    animation->setStartValue(_color);
    animation->setEndValue(colorFromState(state));
    animation->start();
    connect(animation, &QVariantAnimation::valueChanged, [=]() { update(); });
}


QColor Tile::colorFromState(const TileState& state) const
{
    switch (state) {
    case TileState::Empty:
        return emptyColor();
    case TileState::Bot:
        return botColor();
    case TileState::User:
        return userColor();
    default:
        Q_UNREACHABLE(); // missing enumerant
        break;
    }
}

QSize Tile::sizeHint() const
{
    return QSize(30, 30);
}

QSize Tile::minimumSizeHint() const
{
    return QSize(15, 15);
}

void Tile::setPlayerTileState(TileState s)
{
    switch (tileState()) {
    case TileState::Empty:
        setTileState(s);
        break;
    default:
        Q_UNREACHABLE(); // in other cases, we should not get here
        break;
    }
}

void Tile::exchange()
{
    if (tileState() != TileState::Empty)
        setTileState(tileState() == TileState::Bot ? TileState::User : TileState::Bot);
}

QColor Tile::botColor() const
{
    return _botColor;
}

QColor Tile::userColor() const
{
    return _userColor;
}

int Tile::margin() const
{
    return _margin;
}

QColor Tile::emptyColor() const
{
    return _emptyColor;
}

void Tile::setEmptyColor(const QColor &emptyColor)
{
    _emptyColor = emptyColor;
}

void Tile::setColor(QColor c)
{
    if (_color == c)
        return;
    _color = c;
    emit colorChanged(c);
}

void Tile::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    if (tileState() == TileState::Empty && !playable()) {
//        QGradientStops stops;

//        stops << QGradientStop(0.00, Qt::black);
//        stops << QGradientStop(1.00, Qt::transparent);
//        QRadialGradient grad(QPointF(0, 0), height() / 2 - margin(), QPointF(width() / 2, height() / 2));
//        grad.setStops(stops);
        painter.setBrush(QBrush(QColor::fromRgbF(0, 0, 0, 0.2)));
    }
    else if (tileState() == TileState::Empty && !underMouse() && playable()) {
        QPen pen = painter.pen();
        pen.setWidthF(1.5);
        painter.setPen(pen);
    }
    else {
        painter.setPen(Qt::NoPen);
        QGradientStops stops;

        stops << QGradientStop(0.00, QColor::fromRgba(0xffffffff));
        stops << QGradientStop(0.11, empty()? emptyColor() : color());
        stops << QGradientStop(1.00, Qt::black);
        QRadialGradient grad(QPointF(width() / 2, height() / 2), height() / 2 - margin(), QPointF(0, 0));
        grad.setStops(stops);
        painter.setBrush(QBrush(grad));
    }
    painter.drawEllipse(margin(), margin(), width() - 2 * margin(), height() - 2 * margin());
    event->accept();
}

