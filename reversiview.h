#pragma once

#include "case.h"

#include <QFrame>

class ReversiView : public QFrame
{
    Q_OBJECT
public:
    explicit ReversiView(QWidget *parent = nullptr);

    void resetTiles();
    void setStartCenter();
    void restartTiles();

    void restart();

signals:

private:
    Tile*** _tiles;

    const int _numberOfColumns = 8;
    const int _numberOfRows = 8;

    void setupUi();
};

