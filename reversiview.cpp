#include "reversiview.h"

#include <QGridLayout>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QSound>
#include <QTimer>
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
    QMessageBox::StandardButton n = QMessageBox::Yes;
    if (!end())
         n = QMessageBox::warning(this, tr("Restart?"), tr("The game will restart. This cannot be undone. \nProceed?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (n == QMessageBox::Yes) {
        restartTiles();
        setEnd(false);
        update();
        setTurn(TileState::User);
    }
}

void ReversiView::endGameTalk()
{
    setEnd(true);
    setEmptyColor(Qt::transparent);

    QPair<int, int> score = countBotAndUser();
    QString s;
    if (score.first > score.second)
        s= tr("Haha! I beat you again! You mere human...\n"
              "Because I am the greatest, I offer you a rematch.");
    else if (score.first == score.second)
        s= tr("Wow! Look at how good I am!\n"
              "I wanted to tie the game, and it's exactly what happened!\n"
              "Ok. That's enough, next time I will be you fair and square.\n"
              "Let's play again.");
    else
        s= tr("Beeep -- Booop -- Buuup!\n"
              "... RECOVERING...\n"
              "...\n"
              "I crashed. And you cheated. That's why you won!!!\n"
              "I want a revenge.");
    QMessageBox::StandardButton n = QMessageBox::information(this, tr("The End"), s, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    if (n == QMessageBox::Yes)
        restart();
    // else if (n== QMessageBox::No) nothing to do.
    // else Unreachable
}

void ReversiView::userPlay(Tile* tile)
{
    if (end()) // Do nothing if game has ended
        return;
    if (!play(tile)) // do nothing if game is invalid
        return;

    if (!hasPossibleMove(turn())) { // if there is no place to place a tile, skip a turn;
        nextTurn();
        if (!hasPossibleMove(turn())) { // if you still can't place a tile, then no one can, and it's the end of the game
            endGameTalk();
        }
    }
    else {
        if (turn() == TileState::Bot) {
            QTimer::singleShot(750, [=]() { userPlay(botCalculateBestOption()); });
        }
    }
}

bool ReversiView::play(Tile *tile)
{
    if (tile->tileState() != TileState::Empty)
        return false;

    QVector<Tile *> returned = getReturnedTilesForMove(tile, turn());
    if (returned.isEmpty()) {
        QSound::play(":/images/audio/invalid.wav");
        return false;
    }

    switch (turn()) {
    case TileState::User:
        tile->setTileState(TileState::User);
        break;
    case TileState::Bot:
        tile->setTileState(TileState::Bot);
        break;
    }
    for (Tile* t : returned) {
        t->exchange();
    }
    nextTurn();
    return true;
}

Tile ***ReversiView::tiles() const
{
    return _tiles;
}

void ReversiView::setTiles(Tile ***tiles)
{
    _tiles = tiles;
}

QColor ReversiView::emptyColorForTurn(TileState t)
{
    return t == TileState::Bot ? _tiles[0][0]->botColor() : _tiles[0][0]->userColor();
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

Tile *ReversiView::botCalculateBestOption() const
{
    // place in corner if possible
    if (getReturnedTilesForMove(tiles()[0][0], turn()).count())
        return tiles()[0][0];
    if (getReturnedTilesForMove(tiles()[numberOfColumns() - 1][0], turn()).count())
        return tiles()[numberOfColumns() - 1][0];
    if (getReturnedTilesForMove(tiles()[0][numberOfRows() - 1], turn()).count())
        return tiles()[0][numberOfRows() - 1];
    if (getReturnedTilesForMove(tiles()[numberOfColumns() - 1][numberOfRows() - 1], turn()).count())
        return tiles()[numberOfColumns() - 1][numberOfRows() - 1];



    return optionTurningMost(turn()).first;
}

Tile ***ReversiView::getBoardAfterMove(Tile* move) const
{
    Tile ***board = getCopyBoard();
    return board;

}

Tile ***ReversiView::getCopyBoard() const
{
    Tile *** result = new Tile**[_numberOfRows];
    for(int i = 0; i < _numberOfRows; ++i) {
        result[i] = new Tile*[_numberOfColumns];
        for (int j = 0; j < _numberOfColumns; ++j) {
            // Do not set the parent, as it is added automatically with the grid layout
            result[i][j] = new Tile (i, j);
            result[i][j]->setPos(QPoint(i, j));
            result[i][j]->setTileState(_tiles[i][j]->tileState());
        }
    }
    return result;
}

void ReversiView::AsciiPrintBoard(Tile *** board)
{
    for(int i = 0; i < _numberOfRows; ++i) {
        for (int j = 0; j < _numberOfColumns; ++j) {
            switch (board[i][j]->tileState()) {
            case TileState::Empty:
                printf("*");
                break;
            case TileState::User:
                printf("X");
                break;
            case TileState::Bot:
                printf("O");
                break;
            default:
                Q_UNREACHABLE();
                break;
            }
        }
    }
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

QVector<Tile *> ReversiView::getReturnedTilesForMove(Tile* tile, TileState player) const
{
    if (tile->tileState() != TileState::Empty)
        return QVector<Tile *>(); // you cannot do this move.
    QVector<Tile *> result;
    result.append(emprisonsInDir(tile, player, Direction::Up));
    result.append(emprisonsInDir(tile, player, Direction::Down));
    result.append(emprisonsInDir(tile, player, Direction::Right));
    result.append(emprisonsInDir(tile, player, Direction::Left));

    result.append(emprisonsInDir(tile, player, Direction::UpRight));
    result.append(emprisonsInDir(tile, player, Direction::UpLeft));
    result.append(emprisonsInDir(tile, player, Direction::DownRight));
    result.append(emprisonsInDir(tile, player, Direction::DownLeft));
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
        Q_UNREACHABLE(); // in other cases, we should not get here
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

QPair<Tile *, int> ReversiView::optionTurningMost(TileState player) const
{
    int count = 0;
    Tile* tile = nullptr;
    for (int i = 0; i < numberOfColumns(); ++i) {
        for (int j = 0; j < numberOfRows(); ++j) {
            int c = getReturnedTilesForMove(tiles()[i][j], player).count();
            if (c > count) {
                tile = tiles()[i][j];
                count = c;
            }
        }
    }
    return QPair<Tile* , int> (tile, count);
}

bool ReversiView::hasPossibleMove(TileState player)
{
    for (int i = 0; i < numberOfColumns(); ++i) {
        for (int j = 0; j < numberOfRows(); ++j) {
            if (getReturnedTilesForMove(tiles()[i][j], player).count())
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
            _tiles[i][j]->setPos(QPoint(i, j));
            connect(_tiles[i][j], &QAbstractButton::clicked, [=]() { userPlay(_tiles[i][j]); });
//            _tiles[i][j]->setTileState(qrand() % 2 ? TileState::Bot : TileState::User);
        }
    }

    restartTiles();

    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(2);

    setMinimumSize(600, 600);

    setupUi();
}
