#ifndef ACC_H
#define ACC_H

#include <QWidget>

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

private:
    Ui::Acc *ui;
};

#endif // ACC_H
