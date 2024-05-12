#include "inputfen.h"
#include "ui_inputfen.h"

InputFEN::InputFEN(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InputFEN)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(ICON_CHESS));
    setFixedSize(400, 200);
}

InputFEN::~InputFEN()
{
    delete ui;
}

void InputFEN::on_InputFEN_2_clicked(){
    FEN = ui->FEN_String->toPlainText();
    hide();
}

