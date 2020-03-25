#pragma once

#include "case.h"

#include <QFrame>

class ReversiView : public QFrame
{
    Q_OBJECT

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

    Q_PROPERTY(bool end READ end WRITE setEnd NOTIFY endChanged)

public:
    explicit ReversiView(QWidget *parent = nullptr);

    void restart();
    void userPlay(Tile *tile);
    bool play(Tile* tile);

    TileState turn() const { return _turn; }
    void setTurn(const TileState &turn) { _turn = turn; }
    void nextTurn() { setTurn((turn() == TileState::Bot) ? TileState::User : TileState::Bot); emit turnChanged(turn()); }

    int numberOfColumns() const;
    int numberOfRows() const;

    Tile *botCalculateBestOption() const;

    Tile ***tiles() const;
    void setTiles(Tile ***tiles);

    QColor emptyColorForTurn (TileState t);
    QColor emptyColorForTurn () { return emptyColorForTurn(turn()); }

    QPair<int, int> countBotAndUser();

    void setEmptyColor(QColor);

    bool end() const { return m_end; }

public slots:
    void setEnd(bool end) { m_end = end; emit endChanged(m_end); }

signals:
    void turnChanged(TileState);
    void endChanged(bool end);

private:
    /// variables
    Tile*** _tiles;

    TileState _turn = TileState::User;

    const int _numberOfColumns = 8;
    const int _numberOfRows = 8;

    bool m_end = false;

    /// functions
    void setupUi();

    void resetTiles();
    void setStartCenter();
    void restartTiles();

    QVector<Tile*> getReturnedTilesForMove(Tile* tile, TileState player) const;

    Tile* adjacentTile(Tile *tile, Direction dir) const;
    bool occupiedAdjacentTile(Tile *tile, ReversiView::Direction dir) const;
    QVector<Tile *> emprisonsInDir(Tile *tile, TileState state, ReversiView::Direction dir) const;

    const TileState enemyState(TileState state) const;

    QPair<Tile*, int> optionTurningMost(TileState player) const;
    bool hasPossibleMove(TileState player);

};
