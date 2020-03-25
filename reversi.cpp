#include "reversi.h"
#include "case.h"
#include "playertile.h"

#include <QGridLayout>
#include <QLabel>


Reversi::Reversi(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Reversi"));
    setupUi();

    updateTurn();
}

Reversi::~Reversi()
{

}

void Reversi::setupUi()
{
    // No need to set a parent, as ownership is taken by the grid layout
    QGridLayout* grid = new QGridLayout;

    _reversi = new ReversiView;
    connect(_reversi, &ReversiView::turnChanged, this, &Reversi::updateTurn);
    grid->setColumnStretch(0, 1);
    grid->addWidget(_reversi, 0, 0, 3, 3);

    _restart = new QPushButton;
    _restart->setText(tr("Restart"));
//    _restart->setEnabled(false);
    connect(_restart, &QPushButton::clicked, _reversi, &ReversiView::restart);
    connect(_restart, &QPushButton::clicked, _reversi, [=](){ updateTurn(); });
    grid->addWidget(_restart, 3, 3);

    _userCount = new QLabel(QString::number(0));
    QFont f = _userCount->font();
    constexpr int margin = 5;
    f.setPixelSize(30);
    f.setUnderline(true);
    _userCount->setFont(f);
    _userCount->setStyleSheet("color: white;");
    grid->addWidget(_userCount, 2, 3, Qt::AlignCenter);

    _botCount  = new QLabel(QString::number(0));
    _botCount->setStyleSheet("color: white;");
    _botCount->setFont(f);
    grid->addWidget(_botCount, 0, 3, Qt::AlignCenter);

    grid->setRowStretch(1, 1);
    _turn = new PlayerTile();
    _turn->setMaxScore(_reversi->numberOfColumns() * _reversi->numberOfRows());
    updateTurn();
    grid->addWidget(_turn, 1, 3);

    QWidget* widget = new QWidget(this);
    widget->setLayout(grid);
    setCentralWidget(widget);
}

void Reversi::updateTurn()
{
    QPair<int, int> countPair = _reversi->countBotAndUser();
    _botCount->setText(QString::number(countPair.first));
    _userCount->setText(QString::number(countPair.second));

    _turn->setScore(countPair);
    _reversi->setEmptyColor(_reversi->emptyColorForTurn());
}
