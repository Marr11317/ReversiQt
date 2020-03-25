#include "playertile.h"

#include <QPainter>
#include <QtEvents>

PlayerTile::PlayerTile()
    : Tile(),
      m_score(QPair<int, int> (2, 2)),
      m_maxScore(64)
{

}

void PlayerTile::setScore(QPair<int, int> score)
{
    m_score = score;
    emit scoreChanged(m_score);
    update();
}

qreal PlayerTile::scoreFraction() const
{
    qreal botScore = score().first;
    qreal userScore = score().second;
    return botScore / (botScore + userScore);
}

void PlayerTile::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QLinearGradient grad(QPointF(width() / 2, 0.0), QPointF(width() / 2, height()));
    //    grad.setSpread(QGradient::PadSpread); // default
    qreal botPlace = qreal(score().first) / maxScore();
    qreal userPlace = 1.0 - qreal(score().second) / maxScore();
    grad.setColorAt(0, botColor());
    grad.setColorAt(1.0, userColor());
    if (totalScore() == maxScore()) {
        static constexpr qreal margin = 0.02;
        const qreal center = (botPlace + userPlace) / 2;
        grad.setColorAt(center, Qt::black);
        grad.setColorAt(center + margin, userColor());
        grad.setColorAt(center - margin, botColor());
    }
    else {
        grad.setColorAt(botPlace, Qt::black);
        grad.setColorAt(userPlace, Qt::black);
        grad.setColorAt((botPlace + userPlace) / 2, Qt::white); // middle white
    }
    painter.setBrush(QBrush(grad));
    painter.drawEllipse(margin(), margin(), width() - 2 * margin(), height() - 2 * margin());

    event->accept();

}

QSize PlayerTile::sizeHint() const
{
    return QSize(50, 50);
}

QSize PlayerTile::minimumSizeHint() const
{
    return QSize(30, 30);
}
