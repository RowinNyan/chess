#ifndef WIDGET_H
#define WIDGET_H

#include <QApplication>
#include <QTimer>
#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QImage>
#include <QPixmap>
#include <QFile>
#include <QTextStream>
#include <QHeaderView>

#include "chess.h"
#include "config.h"
#include "options.h"
#include "acc.h"
#include "inputfen.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void InitProgram();
    void InitWindow();
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void IsCheckmate();
    void PawnAcc(short);

private slots:
    void RepaintEvent();
    void Quit();
    void FEN_Input();
    void AccChess();
    void on_StdStart_clicked();
    void on_StopGame_clicked();
    void on_Options_clicked();
    void on_Input_clicked();
    void on_Output_FEN_clicked();

private:
    Options* options;
    Acc* acc;
    InputFEN* fen;
    QTimer* timer;
    Game* game;
    short** valid;
    short OldRow, OldCol, KingX, KingY;
    bool HoldingChess, Empty, InGame;
    Ui::Widget *ui;
};
#endif // WIDGET_H
