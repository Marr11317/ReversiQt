#pragma once

#include "case.h"

#include <QFrame>

class ReversiView : public QFrame
{
    Q_OBJECT

    enum class Turn { Bot, User };
    enum class Direction {
        Up,
        Down,
        Right,
        Left,
        UpRight,
        DownRight,
        UpLeft,
        DownLeft
    };

public:
    explicit ReversiView(QWidget *parent = nullptr);

    void restart();
    void userPlay(Tile *tile);

    Turn turn() const { return _turn; }
    void setTurn(const Turn &turn) { _turn = turn; }
    void nextTurn() { setTurn((turn() == Turn::Bot) ? Turn::User : Turn::Bot); emit turnChanged(turn()); }

    const TileState tileStateForTurn() const { return (turn() == Turn::Bot) ? TileState::Bot : TileState::User; }

    int numberOfColumns() const;
    int numberOfRows() const;

    Tile ***tiles() const;
    void setTiles(Tile ***tiles);

    QColor emptyColorForTurn (Turn t);
    QColor emptyColorForTurn () { return emptyColorForTurn(turn()); }

    QPair<int, int> countBotAndUser();

    void setEmptyColor(QColor);

signals:
    void turnChanged(Turn);

private:
    // variables
    Tile*** _tiles;

    Turn _turn = Turn::User;

    const int _numberOfColumns = 8;
    const int _numberOfRows = 8;

    // functions
    void setupUi();

    void resetTiles();
    void setStartCenter();
    void restartTiles();

    QVector<Tile*> getReturnedTilesForMove(Tile* tile) const;

    Tile* adjacentTile(Tile *tile, Direction dir) const;
    bool occupiedAdjacentTile(Tile *tile, ReversiView::Direction dir) const;
    QVector<Tile *> emprisonsInDir(Tile *tile, TileState state, ReversiView::Direction dir) const;

    const TileState enemyState(TileState state) const;

    QPair<Tile*, int> optionTurningMost();
    bool hasPossibleMove();

};
