#include "reversiview.h"

#include <QGridLayout>
#include <QMouseEvent>
#include <QPainter>

void ReversiView::resetTiles()
{
    // Reset the tiles
    for (int i = 0; i < _numberOfColumns; ++i) {
        for (int j = 0; j < _numberOfRows; ++j) {
            _tiles[i][j]->setState(TileState::Empty);
        }
    }
}

void ReversiView::setStartCenter()
{
    // Set the start position
    const int hcenter = _numberOfColumns * .5;
    const int vcenter = _numberOfRows * .5;
    _tiles[hcenter][vcenter]->setState(TileState::Bot);
    _tiles[hcenter - 1][vcenter - 1]->setState(TileState::Bot);
    _tiles[hcenter][vcenter - 1]->setState(TileState::User);
    _tiles[hcenter - 1][vcenter]->setState(TileState::User);
}

void ReversiView::restartTiles()
{
    resetTiles();
    setStartCenter();
}

void ReversiView::restart()
{
    restartTiles();
    update();
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

ReversiView::ReversiView(QWidget *parent) : QFrame(parent)
{
    // Init the tiles
    _tiles = new Tile**[_numberOfRows];
    for(int i = 0; i < _numberOfRows; ++i) {
        _tiles[i] = new Tile*[_numberOfColumns];
        for (int j = 0; j < _numberOfColumns; ++j) {
            // Do not set the parent, as it is added automatically with the grid layout
            _tiles[i][j] = new Tile;
        }
    }

    restartTiles();
    _tiles[2][2]->setState(TileState::User);

    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(2);

    setMinimumSize(600, 600);
    setStyleSheet("background-color: rgb(251, 255, 206);");

    setupUi();
}
