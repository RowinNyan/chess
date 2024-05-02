#ifndef OPTIONS_H
#define OPTIONS_H

#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QButtonGroup>

#include "config.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Options;
}
QT_END_NAMESPACE

enum CastlingPath{
    Both, King2Tiles, KingOnRook
};

class Options : public QWidget
{
    Q_OBJECT
public:
    Options(QWidget *parent = nullptr);
    ~Options();

    void InitWindow();

    static bool ClickToMove, LightColorMode, AutoAccending;
    static CastlingPath CP;

private slots:
    void on_SaveConfig_clicked();

    void slots_ClickToMove();
    void slots_LightColorMode();
    void slots_AutoAccending();
    void slots_CastlingPath();

private:
    Ui::Options *ui;
    QButtonGroup* _ClickToMove;
    QButtonGroup* _LightColorMode;
    QButtonGroup* _CastlingPath;
    QButtonGroup* _AutoAccending;
};

#endif // OPTIONS_H
