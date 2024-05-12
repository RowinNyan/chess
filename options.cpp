#include "options.h"
#include "./ui_options.h"

bool Options::ClickToMove, Options::LightColorMode, Options::AutoAccending;
CastlingPath Options::CP;

Options::Options(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Options)
{
    ui->setupUi(this);
    InitWindow();
}

Options::~Options()
{
    delete ui;
}

void Options::InitWindow(){
    {
        ui->DarkColor->setEnabled(false);
        ui->Drag->setEnabled(false);
    }
    setFixedSize(400, 300);
    setWindowTitle(WINDOW_TITLE_CONFIG);
    _ClickToMove = new QButtonGroup(this);
    _LightColorMode = new QButtonGroup(this);
    _AutoAccending = new QButtonGroup(this);
    _CastlingPath = new QButtonGroup(this);
    _ClickToMove->addButton(ui->Click, 1);
    _ClickToMove->addButton(ui->Drag, 0);
    _LightColorMode->addButton(ui->LightColor, 1);
    _LightColorMode->addButton(ui->DarkColor, 0);
    _AutoAccending->addButton(ui->PawnToQueen, 1);
    _AutoAccending->addButton(ui->PawnToUnknown, 0);
    _CastlingPath->addButton(ui->Both, 0);
    _CastlingPath->addButton(ui->King2Tiles, 1);
    _CastlingPath->addButton(ui->KingOnRook, 2);
    if(ClickToMove){
        ui->Click->setChecked(true);
    }
    else{
        ui->Drag->setChecked(true);
    }
    if(LightColorMode){
        ui->LightColor->setChecked(true);
    }
    else{
        ui->DarkColor->setChecked(true);
    }
    if(AutoAccending){
        ui->PawnToQueen->setChecked(true);
    }
    else{
        ui->PawnToUnknown->setChecked(true);
    }
    switch (CP){
        case Both:
            ui->Both->setChecked(true);
            break;
        case King2Tiles:
            ui->King2Tiles->setChecked(true);
            break;
        case KingOnRook:
            ui->KingOnRook->setChecked(true);
            break;
    }
    connect(ui->Click, SIGNAL(clicked(bool)), this, SLOT(slots_ClickToMove()));
    connect(ui->Drag, SIGNAL(clicked(bool)), this, SLOT(slots_ClickToMove()));
    connect(ui->LightColor, SIGNAL(clicked(bool)), this, SLOT(slots_LightColorMode()));
    connect(ui->DarkColor, SIGNAL(clicked(bool)), this, SLOT(slots_LightColorMode()));
    connect(ui->PawnToQueen, SIGNAL(clicked(bool)), this, SLOT(slots_AutoAccending()));
    connect(ui->PawnToUnknown, SIGNAL(clicked(bool)), this, SLOT(slots_AutoAccending()));
    connect(ui->Both, SIGNAL(clicked(bool)), this, SLOT(slots_CastlingPath()));
    connect(ui->King2Tiles, SIGNAL(clicked(bool)), this, SLOT(slots_CastlingPath()));
    connect(ui->KingOnRook, SIGNAL(clicked(bool)), this, SLOT(slots_CastlingPath()));
}

void Options::on_SaveConfig_clicked(){
    QFile file(FILE_CONFIG);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream text(&file);
    text<<"CTM"<<" "<<(short)ClickToMove<<Qt::endl;
    text<<"LCM"<<" "<<(short)LightColorMode<<Qt::endl;
    text<<"AAC"<<" "<<(short)AutoAccending<<Qt::endl;
    text<<"CTP"<<" "<<(short)CP<<Qt::endl;
    file.close();
    hide();
}

void Options::slots_ClickToMove(){
    ClickToMove = (bool)_ClickToMove->checkedId();
}

void Options::slots_LightColorMode(){
    LightColorMode = (bool)_LightColorMode->checkedId();
}

void Options::slots_AutoAccending(){
    AutoAccending = (bool)_AutoAccending->checkedId();
}

void Options::slots_CastlingPath(){
    switch (_CastlingPath->checkedId()){
        case 0:
            CP = Both;
            break;
        case 1:
            CP = King2Tiles;
            break;
        case 2:
            CP = KingOnRook;
            break;
    }
}
