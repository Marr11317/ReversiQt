#include "reversi.h"

#include <QGridLayout>


Reversi::Reversi(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Reversi"));
    setStyleSheet("background-color: rgb(20, 20, 20);");

    setupUi();
}

Reversi::~Reversi()
{

}

void Reversi::setupUi()
{
    // No need to set a parent, as ownership is taken by the grid layout
    QGridLayout* grid = new QGridLayout;

    _reversi = new ReversiView;
    grid->setColumnStretch(0, 1);
    grid->addWidget(_reversi, 0, 0);

    _restart = new QPushButton;
    _restart->setText(tr("Rejouer"));
//    _restart->setEnabled(false);
    connect(_restart, &QPushButton::clicked, _reversi, &ReversiView::restart);
    grid->addWidget(_restart, 1, 1);

    QWidget* widget = new QWidget(this);
    widget->setLayout(grid);
    setCentralWidget(widget);
}
