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

#endif // ACC_H
