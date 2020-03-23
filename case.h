#pragma once

#include <QWidget>

class Case : public QWidget
{
    enum class State { User, Bot, Empty, Invalid};
    Q_OBJECT
public:
    explicit Case(QWidget *parent = nullptr, State s = State::Empty);

    void paintEvent (QPaintEvent *event) override;

signals:

private:
    State _state = State::Invalid;

};

