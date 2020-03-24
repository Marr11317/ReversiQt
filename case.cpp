#include "case.h"

#include <QAbstractButton>
#include <QMouseEvent>
#include <QPainter>
#include <QStyle>
#include <QStyleOptionSlider>

Tile::Tile(int xpos, int ypos, QWidget *parent, TileState s) :
    QAbstractButton(parent),
    _tileState(s),
    _xpos(xpos),
    _ypos(ypos),
    _emptyColor(Qt::darkGray)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setAttribute(Qt::WA_Hover, true); // Receive mouse enter and mouse leave event
//    QSizePolicy pol;
//    pol.setHeightForWidth(true);
//    setSizePolicy(pol);
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
    default: // in other cases, we should not get here
        Q_ASSERT(false);
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
        painter.setBrush(_botColor);
        painter.setPen(Qt::NoPen);
        break;
    case TileState::User:
        painter.setBrush(_userColor);
        painter.setPen(Qt::NoPen);
        break;
    default:
        qWarning("Invalid State");
    }
    painter.drawEllipse(margin(), margin(), width() - 2 * margin(), height() - 2 * margin());
    event->accept();
}

