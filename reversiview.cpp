#include "reversiview.h"

#include <QGridLayout>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QtDebug>

void ReversiView::resetTiles()
{
    // Reset the tiles
    for (int i = 0; i < _numberOfColumns; ++i) {
        for (int j = 0; j < _numberOfRows; ++j) {
            _tiles[i][j]->setTileState(TileState::Empty);
        }
    }
}

void ReversiView::setStartCenter()
{
    // Set the start position
    const int hcenter = _numberOfColumns * .5;
    const int vcenter = _numberOfRows * .5;
    _tiles[hcenter][vcenter]->setTileState(TileState::Bot);
    _tiles[hcenter - 1][vcenter - 1]->setTileState(TileState::Bot);
    _tiles[hcenter][vcenter - 1]->setTileState(TileState::User);
    _tiles[hcenter - 1][vcenter]->setTileState(TileState::User);
}

void ReversiView::restartTiles()
{
    resetTiles();
    setStartCenter();
}

void ReversiView::restart()
{
    QMessageBox::StandardButton n = QMessageBox::warning(this, tr("Restart?"), tr("The game will restart. This cannot be undone. \nProceed?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (n == QMessageBox::Yes) {
        restartTiles();
        update();
    }
}

void ReversiView::userPlay(Tile* tile)
{
    if (tile->tileState() == TileState::Empty) {
        QVector<Tile*> returned = getReturnedTilesForMove(tile);
        if (returned.isEmpty()) {
            QMessageBox::information(this, tr("Invalid move"), tr("This move is invalid because it doesn't turn any tile."), QMessageBox::Ok, QMessageBox::Ok);
            return;
        }
        for (Tile* t : returned) {
            t->exchange();
        }
        switch (turn()) {
        case Turn::User:
            tile->setTileState(TileState::User);
            break;
        case Turn::Bot:
            tile->setTileState(TileState::Bot);
            break;
        }
        nextTurn();
    }

    if (!hasPossibleMove()) {
        setEmptyColor(Qt::transparent);

        QString s;
        if (true)
                s= tr("Haha! I beat you again! You mere human...");
        QMessageBox::information(this, tr("The End"), s, QMessageBox::Ok, QMessageBox::Ok);
    }
}

Tile ***ReversiView::tiles() const
{
    return _tiles;
}

void ReversiView::setTiles(Tile ***tiles)
{
    _tiles = tiles;
}

QColor ReversiView::emptyColorForTurn(ReversiView::Turn t)
{
    return t == Turn::Bot ? _tiles[0][0]->botColor() : _tiles[0][0]->userColor();
}

QPair<int, int> ReversiView::countBotAndUser()
{
    int botCount = 0;
    int userCount = 0;
    for (int i = 0; i < numberOfColumns(); ++i) {
        for (int j = 0; j < numberOfRows(); ++j) {
            switch (tiles()[i][j]->tileState()) {
            case TileState::Bot :
                ++botCount;
                break;
            case TileState::User:
                ++userCount;
                break;
            // Nothing to do when empty
            }
        }
    }
    return QPair<int, int> (botCount, userCount);
}

void ReversiView::setEmptyColor(QColor color)
{
    int numCol = numberOfColumns();
    int numRows = numberOfRows();
    Tile *** tls = tiles();
    for (int i = 0; i < numCol; ++i) {
        for (int j = 0; j < numRows; ++j) {
            tls[i][j]->setEmptyColor(color);
        }
    }
}

int ReversiView::numberOfColumns() const
{
    return _numberOfColumns;
}

int ReversiView::numberOfRows() const
{
    return _numberOfRows;
}

void ReversiView::setupUi()
{
    QGridLayout* grid = new QGridLayout;

    for (int i = 0; i < _numberOfColumns; ++i) {
        for (int j = 0; j < _numberOfRows; ++j) {
            grid->addWidget(_tiles[i][j], i, j);
        }
    }

    setLayout(grid);
}

QVector<Tile *> ReversiView::getReturnedTilesForMove(Tile* tile) const
{
    if (tile->tileState() != TileState::Empty)
        return QVector<Tile *>(); // you cannot do this move.
    QVector<Tile *> result;
    TileState t = tileStateForTurn();
    result.append(emprisonsInDir(tile, t, Direction::Up));
    result.append(emprisonsInDir(tile, t, Direction::Down));
    result.append(emprisonsInDir(tile, t, Direction::Right));
    result.append(emprisonsInDir(tile, t, Direction::Left));

    result.append(emprisonsInDir(tile, t, Direction::UpRight));
    result.append(emprisonsInDir(tile, t, Direction::UpLeft));
    result.append(emprisonsInDir(tile, t, Direction::DownRight));
    result.append(emprisonsInDir(tile, t, Direction::DownLeft));
    return result;
}

Tile *ReversiView::adjacentTile(Tile* tile, ReversiView::Direction dir) const
{
    switch (dir) {
    case Direction::Up:
        if (tile->xpos() == 0)
            return nullptr;
        return _tiles[tile->xpos() - 1][tile->ypos()];
    case Direction::Down:
        if (tile->xpos() == _numberOfRows - 1)
            return nullptr;
        return _tiles[tile->xpos() + 1][tile->ypos()];
    case Direction::Right:
        if (tile->ypos() == _numberOfColumns - 1)
            return nullptr;
        return _tiles[tile->xpos()][tile->ypos() + 1];
    case Direction::Left:
        if (tile->ypos() == 0)
            return nullptr;
        return _tiles[tile->xpos()][tile->ypos() - 1];

    case Direction::UpRight:
        if (tile->xpos() == 0 || tile->ypos() == _numberOfColumns - 1)
            return nullptr;
        return _tiles[tile->xpos() - 1][tile->ypos() + 1];
    case Direction::DownRight:
        if (tile->xpos() == _numberOfRows - 1 || tile->ypos() == _numberOfColumns - 1)
            return nullptr;
        return _tiles[tile->xpos() + 1][tile->ypos() + 1];
    case Direction::UpLeft:
        if (tile->xpos() == 0 || tile->ypos() == 0)
            return nullptr;
        return _tiles[tile->xpos() - 1][tile->ypos() - 1];
    case Direction::DownLeft:
        if (tile->xpos() == _numberOfRows - 1 || tile->ypos() == 0)
            return nullptr;
        return _tiles[tile->xpos() + 1][tile->ypos() - 1];
    default:
        Q_ASSERT(false);
        return nullptr;
    }
}

QVector<Tile*> ReversiView::emprisonsInDir(Tile* tile, TileState state, ReversiView::Direction dir) const
{
    QVector<Tile*> result;
    Tile *current;
    for (current = adjacentTile(tile, dir); current && current->tileState() != TileState::Empty && current->tileState() == enemyState(state); current = adjacentTile(current, dir)) {
        result << current;
    }
    if (!current || current->tileState() == TileState::Empty)
        return QVector<Tile*>(); // return an empty vector
    return result;
}

const TileState ReversiView::enemyState(TileState state) const
{
    Q_ASSERT (state != TileState::Empty);
    return state == TileState::User ? TileState::Bot : TileState::User;
}

QPair<Tile *, int> ReversiView::optionTurningMost()
{
    int count = 0;
    Tile* tile = nullptr;
    for (int i = 0; i < numberOfColumns(); ++i) {
        for (int j = 0; j < numberOfRows(); ++j) {
            int c = getReturnedTilesForMove(tiles()[i][j]).count();
            if (c > count) {
                tile = tiles()[i][j];
                count = c;
            }
        }
    }
    return QPair<Tile* , int> (tile, count);
}

bool ReversiView::hasPossibleMove()
{
    for (int i = 0; i < numberOfColumns(); ++i) {
        for (int j = 0; j < numberOfRows(); ++j) {
            if (getReturnedTilesForMove(tiles()[i][j]).count())
                return true;
        }
    }
    return false;
}

bool ReversiView::occupiedAdjacentTile(Tile* tile, ReversiView::Direction dir) const
{
    return adjacentTile(tile, dir)->tileState() != TileState::Empty;
}

ReversiView::ReversiView(QWidget *parent) : QFrame(parent)
{
    // Init the tiles
    _tiles = new Tile**[_numberOfRows];
    for(int i = 0; i < _numberOfRows; ++i) {
        _tiles[i] = new Tile*[_numberOfColumns];
        for (int j = 0; j < _numberOfColumns; ++j) {
            // Do not set the parent, as it is added automatically with the grid layout
            _tiles[i][j] = new Tile (i, j);
            _tiles[i][j]->setXpos(i);
            _tiles[i][j]->setYpos(j);
            connect(_tiles[i][j], &QAbstractButton::clicked, [=]() { userPlay(_tiles[i][j]); });
            _tiles[i][j]->setTileState(qrand() % 2 ? TileState::Bot : TileState::User);
        }
    }

//    restartTiles();

    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(2);

    setMinimumSize(600, 600);

    setupUi();
}
