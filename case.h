#pragma once

#include <QAbstractButton>
#include <QWidget>

enum class TileState { User, Bot, Empty};

class Tile : public QAbstractButton
{
    Q_OBJECT
    Q_PROPERTY(QColor bestColor MEMBER _color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(bool playable MEMBER _playable READ playable WRITE setPlayable NOTIFY playableChanged)

public:
    explicit Tile(int xpos = -1, int ypos = -1, QWidget *parent = nullptr, TileState s = TileState::Empty);


    TileState tileState() const { return _tileState; }
    void setTileState(const TileState &state);

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

    QColor color() const {  return _color; }
    void setColor(QColor c);

    QColor colorFromState(const TileState &state) const;
    QColor colorFromCurrentState() const {return colorFromState(tileState()); }

    bool empty() const { return tileState() == TileState::Empty; }

    bool playable() const { return m_playable; }

public slots:
    void setPlayable(bool playable) { m_playable = playable; emit playableChanged(m_playable); update(); }

signals:
    void colorChanged(QColor _color);

    void playableChanged(bool playable);

private:
    int _xpos;
    int _ypos;
    TileState _tileState = TileState::Empty;

    const QColor _botColor;
    const QColor _userColor;
    QColor _emptyColor;

    QColor _color;
    bool m_playable;


    const int _margin = 4;

protected:
    virtual void paintEvent (QPaintEvent *) override;
    virtual QSize sizeHint() const override;
    virtual QSize minimumSizeHint() const override;
};

