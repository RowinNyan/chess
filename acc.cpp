#include "acc.h"
#include "./ui_acc.h"

Acc::Acc(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Acc)
{
    ui->setupUi(this);
    setWindowTitle(WINDOW_TITLE_ACC);
    setWindowIcon(QIcon(ICON_CHESS));
    setFixedSize(400, 160);
}

Acc::~Acc()
{
    delete ui;
}

void Acc::paintEvent(QPaintEvent* event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPixmap acc_chesses[4];
    switch (P){
        case White:
            acc_chesses[0].load(IMG_WHITE_QUEEN);
            acc_chesses[1].load(IMG_WHITE_BISHOP);
            acc_chesses[2].load(IMG_WHITE_ROOK);
            acc_chesses[3].load(IMG_WHITE_KNIGHT);
            break;
        case Black:
            acc_chesses[0].load(IMG_BLACK_QUEEN);
            acc_chesses[1].load(IMG_BLACK_BISHOP);
            acc_chesses[2].load(IMG_BLACK_ROOK);
            acc_chesses[3].load(IMG_BLACK_KNIGHT);
            break;
        default:
            break;
    }
    for(short i=0; i<4; i++){
        painter.drawPixmap(40+i*80, 40, 80, 80, acc_chesses[i]);
    }
}

void Acc::mousePressEvent(QMouseEvent* event){
    if(event->button()==Qt::LeftButton){
        if(event->pos().x()>40 && event->pos().x()<360 && event->pos().y()>40 && event->pos().y()<120){
            I = (event->pos().x()-40)/80;
        }
    }
    Acced();
    hide();
}

#include "./ui_easteregg.h"
#include <QFont>
#include <qfontdatabase.h>

EasterEgg::EasterEgg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EasterEgg)
{
    ui->setupUi(this);
    int fontId = QFontDatabase::addApplicationFont(FONT_MAIN);
    QStringList fontFamilies=QFontDatabase::applicationFontFamilies(fontId);
    QFont font;
    font.setFamily(fontFamilies[0]);
    font.setPointSize(18);
    ui->ea->setFont(font);
    ui->ea->setStyleSheet("color:#ff7fea;");
    setWindowIcon(QIcon(ICON_CHESS));
    setFixedSize(640, 360);
}

EasterEgg::~EasterEgg()
{
    delete ui;
}

void EasterEgg::paintEvent(QPaintEvent* event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPixmap ea;
    ea.load(IMG_0);
    painter.drawPixmap(0, 0, 640, 360, ea);
}
