#pragma once

#include <QAbstractButton>
#include <QWidget>

enum class TileState { User, Bot, Empty};

class Tile : public QAbstractButton
{
    Q_OBJECT

public:
    explicit Tile(int xpos = -1, int ypos = -1, QWidget *parent = nullptr, TileState s = TileState::Empty);


    TileState tileState() const { return _tileState; }
    void setTileState(const TileState &state) { _tileState = state; update(); }

    void setPlayerTileState(TileState s);

    int xpos() const { return _xpos; }
    void setXpos(int xpos) { _xpos = xpos; }
    int ypos() const {return _ypos; }
    void setYpos(int ypos) { _ypos = ypos; }

    QPoint pos() const { return QPoint(xpos(), ypos()); } // helper function
    void setPos(QPoint p) { setXpos(p.x()); setYpos(p.y()); } // helper function

    void exchange();

    QColor botColor() const;
    QColor userColor() const;

    int margin() const;

    QColor emptyColor() const;
    void setEmptyColor(const QColor &emptyColor);

signals:

private:
    int _xpos;
    int _ypos;
    TileState _tileState = TileState::Empty;

    const QColor _botColor = QColor(Qt::green);
    const QColor _userColor = QColor(Qt::blue);
    QColor _emptyColor;

    const int _margin = 4;

protected:
    virtual void paintEvent (QPaintEvent *) override;
    virtual QSize sizeHint() const override;
    virtual QSize minimumSizeHint() const override;
//    virtual int heightForWidth(int w) { return w; }
};

