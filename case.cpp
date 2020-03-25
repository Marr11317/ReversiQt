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
    _botColor(Qt::green),
    _userColor(Qt::blue),
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

    switch (_tileState) {
    case TileState::Empty:
        painter.setPen(Qt::DashLine);
        if (underMouse()) { // hovered
            painter.setBrush(emptyColor());
        }
        else
            painter.setBrush(Qt::transparent);
        break;
    case TileState::Bot:
    case TileState::User:
    {
        painter.setPen(Qt::NoPen);
        QGradientStops stops;

        stops << QGradientStop(0.00, QColor::fromRgba(0xffffffff));
        stops << QGradientStop(0.11, color());
//        stops << QGradientStop(0.13, QColor::fromRgba(0xfff9ff99));
//        stops << QGradientStop(0.14, QColor::fromRgba(0xfff3ff86));
//        stops << QGradientStop(0.49, QColor::fromRgba(0xff93b353));
//        stops << QGradientStop(0.87, QColor::fromRgba(0xff264619));
//        stops << QGradientStop(0.96, QColor::fromRgba(0xff0c1306));
        stops << QGradientStop(1.00, Qt::black);
        QRadialGradient grad(QPointF(width() / 2, height() / 2), height() / 2 - margin(), QPointF(0, 0));
        grad.setStops(stops);
        painter.setBrush(QBrush(grad));
        break;
    }
    default:
        Q_UNREACHABLE();
        return;
    }
    painter.drawEllipse(margin(), margin(), width() - 2 * margin(), height() - 2 * margin());
    event->accept();
}

