#include "reversiview.h"

#include <QMouseEvent>
#include <QPainter>

void ReversiView::resetTiles()
{
    // Reset the tiles
    for (int i = 0; i < _numberOfColumns; ++i) {
        for (int j = 0; j < _numberOfRows; ++j) {
            _tiles[i][j] = State::Empty;
        }
    }
}

void ReversiView::setStartCenter()
{
    // Set the start position
    const int hcenter = _numberOfColumns * .5;
    const int vcenter = _numberOfRows * .5;
    _tiles[hcenter][vcenter]         = State::Bot;
    _tiles[hcenter - 1][vcenter - 1] = State::Bot;
    _tiles[hcenter]    [vcenter - 1] = State::User;
    _tiles[hcenter - 1][vcenter]     = State::User;
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

ReversiView::ReversiView(QWidget *parent) : QFrame(parent)
{
    // Init the tiles
    _tiles = new State*[_numberOfRows];
    for(int i = 0; i < _numberOfRows; ++i)
        _tiles[i] = new State[_numberOfColumns];

    restartTiles();
    _tiles[2][2]     = State::User;

    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(2);

    setMinimumSize(600, 600);
    setStyleSheet("background-color: rgb(251, 255, 206);");
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void ReversiView::paintEvent(QPaintEvent *event)
{
    constexpr int margin = 4;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

//    QRadialGradient radialGradient(50, 50, 50, 70, 70);
//    radialGradient.setColorAt(0.0, Qt::white);
//    radialGradient.setColorAt(0.2, Qt::green);
//    radialGradient.setColorAt(1.0, Qt::black);
//    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin));

    const qreal w = (width()  - (_numberOfColumns + 1) * margin) / _numberOfColumns;
    const qreal h = (height() - (_numberOfRows    + 1) * margin) / _numberOfRows;
    qreal x = margin;
    qreal y = margin;
    for (int i = 0; i < _numberOfColumns; ++i) {
        for (int j = 0; j < _numberOfRows; ++j) {
            switch (_tiles[i][j]) {
            case State::Empty:
                painter.setBrush(Qt::transparent);
                painter.setPen(Qt::DashLine);
                break;
            case State::Bot:
                painter.setBrush(Qt::darkGray);
                painter.setPen(Qt::NoPen);
                break;
            case State::User:
                painter.setBrush(Qt::darkBlue);
                painter.setPen(Qt::NoPen);
                break;
            default:
                qWarning("Invalid State");
            }

            painter.drawEllipse(x, y, w, h);
            x += w + margin;
        }
        y += h + margin;
        x = margin;
    }

    QFrame::paintEvent(event);
}

void ReversiView::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();
}
