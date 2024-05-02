#include "acc.h"
#include "./ui_acc.h"

Acc::Acc(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Acc)
{
    ui->setupUi(this);
}

Acc::~Acc()
{
    delete ui;
}