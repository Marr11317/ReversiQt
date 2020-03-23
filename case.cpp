#include "case.h"

#include <QPainter>

Tile::Tile(QWidget *parent, TileState s) : QWidget(parent), _state(s)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

QSize Tile::sizeHint() const
{
    return QSize(30, 30);
}

QSize Tile::minimumSizeHint() const
{
    return QSize(15, 15);
}

void Tile::paintEvent(QPaintEvent *event)
{
    constexpr int margin = 4;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    switch (_state) {
    case TileState::Empty:
        painter.setBrush(Qt::transparent);
        painter.setPen(Qt::DashLine);
        break;
    case TileState::Bot:
        painter.setBrush(Qt::darkGray);
        painter.setPen(Qt::NoPen);
        break;
    case TileState::User:
        painter.setBrush(Qt::darkBlue);
        painter.setPen(Qt::NoPen);
        break;
    default:
        qWarning("Invalid State");
    }
    painter.drawEllipse(margin, margin, width() - 2 * margin, height() - 2 * margin);
}

