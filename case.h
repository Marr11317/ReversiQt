#pragma once

#include <QWidget>

enum class TileState { User, Bot, Empty};

class Tile : public QWidget
{

    Q_OBJECT
public:
    explicit Tile(QWidget *parent = nullptr, TileState s = TileState::Empty);

    virtual QSize sizeHint() const override;
    virtual QSize minimumSizeHint() const;


    void paintEvent (QPaintEvent *event) override;

    TileState state() const { return _state; }
    void setState(const TileState &state) { _state = state; }

signals:

private:
    TileState _state = TileState::Empty;

};

