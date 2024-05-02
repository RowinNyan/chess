#include "inputfen.h"
#include "ui_inputfen.h"

InputFEN::InputFEN(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InputFEN)
{
    ui->setupUi(this);
}

InputFEN::~InputFEN()
{
    delete ui;
}

void InputFEN::on_InputFEN_2_clicked(){
    FEN = ui->FEN_String->toPlainText();
    hide();
}

