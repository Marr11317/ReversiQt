#pragma once

#include "playertile.h"
#include "reversiview.h"

#include <QLabel>
#include <QMainWindow>
#include <QPushButton>

class Reversi : public QMainWindow
{
    Q_OBJECT

public:
    Reversi(QWidget *parent = nullptr);
    ~Reversi();

private:
    ReversiView* _reversi; // The view
    QPushButton* _restart; // The restart button
    PlayerTile* _turn;           // Shows who's turn it is

    QLabel* _botCount;
    QLabel* _userCount;

    void setupUi();

    void updateTurn();
};
