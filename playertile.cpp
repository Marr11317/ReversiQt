#include "playertile.h"

#include <QPainter>

PlayerTile::PlayerTile()
    : Tile()
{

}

void PlayerTile::paintEvent(QPaintEvent *event)
{
    Tile::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QColor col;
    painter.setPen(col.darker());
    QString text = tileState() == TileState::Bot ? tr("The Enemy Bot") : tr("You");
    painter.drawText(QRect(margin(), margin(), width() - 2 * margin(), height() - 2 * margin()),
                     text,
                     Qt::AlignVCenter | Qt::AlignHCenter);
}

QSize PlayerTile::sizeHint() const
{
    return QSize(50, 50);
}

QSize PlayerTile::minimumSizeHint() const
{
    return QSize(30, 30);
}
