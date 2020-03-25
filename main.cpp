#include "reversi.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet("Reversi {"
                    "background-color: rgb(20, 20, 20);"
                    "color: white;"
                    "}"
                    "ReversiView {"
                    "background-image: url(:/images/images/woodbackground.png);"
                    "}");
    Reversi w;
    w.show();
    return a.exec();
}
