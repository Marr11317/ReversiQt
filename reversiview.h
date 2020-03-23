#pragma once

#include <QFrame>

class ReversiView : public QFrame
{
    enum class State { User, Bot, Empty};

    Q_OBJECT
public:
    explicit ReversiView(QWidget *parent = nullptr);

    void paintEvent (QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

    void resetTiles();
    void setStartCenter();
    void restartTiles();

    void restart();

signals:

private:
    State** _tiles;

    const int _numberOfColumns = 8;
    const int _numberOfRows = 8;
};

