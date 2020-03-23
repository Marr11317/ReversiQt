#pragma once

#include "reversiview.h"

#include <QMainWindow>
#include <QPushButton>

class Reversi : public QMainWindow
{
    Q_OBJECT

public:
    Reversi(QWidget *parent = nullptr);
    ~Reversi();

private:
    ReversiView* _reversi;
    QPushButton* _restart;

    void setupUi();
};
