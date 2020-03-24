#ifndef PLAYERTILE_H
#define PLAYERTILE_H

#include "case.h"

class PlayerTile : public Tile
{
    Q_OBJECT
public:
    PlayerTile();

private:
    virtual void paintEvent (QPaintEvent *event) override;

protected:
    virtual QSize sizeHint() const override;
    virtual QSize minimumSizeHint() const override;
};

#endif // PLAYERTILE_H
