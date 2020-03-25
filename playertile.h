#ifndef PLAYERTILE_H
#define PLAYERTILE_H

#include "case.h"

class PlayerTile : public Tile
{
    Q_OBJECT
    // The score    <bot, usr>
    Q_PROPERTY(QPair<int, int> score READ score WRITE setScore NOTIFY scoreChanged)
    Q_PROPERTY(int maxScore READ maxScore WRITE setMaxScore NOTIFY maxScoreChanged)
public:
    PlayerTile();

    QPair<int, int> score() const { return m_score; }

    int maxScore() const { return m_maxScore; }

public slots:
    void setScore(QPair<int, int> score);
    void setMaxScore(int maxScore) { m_maxScore = maxScore; emit maxScoreChanged(m_maxScore); update(); }

signals:
    void scoreChanged(QPair<int, int> score);
    void maxScoreChanged(int maxScore);

private:
    qreal scoreFraction() const;
    int totalScore() const { return score().first + score().second; }
    QPair<int, int> m_score;

    int m_maxScore;

protected:
    virtual void paintEvent (QPaintEvent *event) override;
    virtual QSize sizeHint() const override;
    virtual QSize minimumSizeHint() const override;
};

#endif // PLAYERTILE_H
