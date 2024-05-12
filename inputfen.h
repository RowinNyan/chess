#ifndef INPUTFEN_H
#define INPUTFEN_H

#include <QWidget>
#include <QString>

#include "config.h"

namespace Ui {
class InputFEN;
}

class InputFEN : public QWidget
{
    Q_OBJECT

public:
    Ui::InputFEN *ui;
    QString FEN;
    explicit InputFEN(QWidget *parent = nullptr);
    ~InputFEN();

private slots:
    void on_InputFEN_2_clicked();

};

#endif // INPUTFEN_H
