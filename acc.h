#ifndef ACC_H
#define ACC_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>

#include "config.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Acc;
}
QT_END_NAMESPACE

class Acc : public QWidget
{
    Q_OBJECT
public:
    Acc(QWidget *parent = nullptr);
    ~Acc();
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);

    Player P;
    short COL, I;

signals:
    void Acced();

private:
    Ui::Acc *ui;
};

#include <QDialog>

namespace Ui {
class EasterEgg;
}

class EasterEgg : public QDialog
{
    Q_OBJECT

public:
    explicit EasterEgg(QWidget *parent = nullptr);
    ~EasterEgg();
    void paintEvent(QPaintEvent*);

private:
    Ui::EasterEgg *ui;
};

#endif // ACC_H
