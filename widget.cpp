#include "widget.h"
#include "./ui_widget.h"
#include "ui_inputfen.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    InitProgram();
    InitWindow();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent* event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPixmap Board, InCheck, BoardBlock[8][8], BoardChess[8][8];
    Board.load(IMG_BOARD);
    InCheck.load(IMG_CHECK);
    painter.drawPixmap(60, 40, 640, 640, Board);
    if(HoldingChess){
        for(short ROW=0; ROW<8; ROW++){
            for(short COL=0; COL<8; COL++){
                if(ROW==OldRow && COL==OldCol){
                    BoardBlock[ROW][COL].load(IMG_VALID_BLOCK);
                    painter.drawPixmap(60+COL*80, 600-ROW*80, 80, 80, BoardBlock[ROW][COL]);
                    continue;
                }
                for(short s=0; ; s++){
                    if(valid[s][0]==-1){
                        break;
                    }
                    if((ROW==valid[s][0] && COL==valid[s][1])){
                        if(game->chess[ROW][COL]->p==Neutral){
                            BoardBlock[ROW][COL].load(IMG_VALID_POINT);
                            painter.drawPixmap(60+COL*80, 600-ROW*80, 80, 80, BoardBlock[ROW][COL]);
                        }
                        else{
                            BoardBlock[ROW][COL].load(IMG_VALID_BLOCK);
                            painter.drawPixmap(60+COL*80, 600-ROW*80, 80, 80, BoardBlock[ROW][COL]);
                        }
                    }
                }
            }
        }
    }
    if(game->turn==Neutral){
        if(!Empty){
            painter.drawPixmap(KingX, KingY, 80, 80, InCheck);
        }
    }
    else{
        short* KingPos = game->FindKing(game->turn);
        if(KingPos != nullptr && game->KingInCheck(game->turn, KingPos[0], KingPos[1])){
            KingX = 60+KingPos[1]*80;
            KingY = 600-KingPos[0]*80;
            painter.drawPixmap(KingX, KingY, 80, 80, InCheck);
            IsCheckmate();
        }
    }
    for(short ROW=0; ROW<8; ROW++){
        for(short COL=0; COL<8; COL++){
            if(game->chess[ROW][COL]->Type()!=Null){
                if(game->chess[ROW][COL]->p==White){
                    switch (game->chess[ROW][COL]->Type()){
                        case King:
                            BoardChess[ROW][COL].load(IMG_WHITE_KING);
                            break;
                        case Queen:
                            BoardChess[ROW][COL].load(IMG_WHITE_QUEEN);
                            break;
                        case Bishop:
                            BoardChess[ROW][COL].load(IMG_WHITE_BISHOP);
                            break;
                        case kNight:
                            BoardChess[ROW][COL].load(IMG_WHITE_KNIGHT);
                            break;
                        case Rook:
                            BoardChess[ROW][COL].load(IMG_WHITE_ROOK);
                            break;
                        case Pawn:
                            BoardChess[ROW][COL].load(IMG_WHITE_PAWN);
                            break;
                        default:
                            break;
                    }
                }
                else{
                    switch (game->chess[ROW][COL]->Type()){
                        case King:
                            BoardChess[ROW][COL].load(IMG_BLACK_KING);
                            break;
                        case Queen:
                            BoardChess[ROW][COL].load(IMG_BLACK_QUEEN);
                            break;
                        case Bishop:
                            BoardChess[ROW][COL].load(IMG_BLACK_BISHOP);
                            break;
                        case kNight:
                            BoardChess[ROW][COL].load(IMG_BLACK_KNIGHT);
                            break;
                        case Rook:
                            BoardChess[ROW][COL].load(IMG_BLACK_ROOK);
                            break;
                        case Pawn:
                            BoardChess[ROW][COL].load(IMG_BLACK_PAWN);
                            break;
                        default:
                            break;
                    }
                }
                painter.drawPixmap(60+COL*80, 600-ROW*80, 80, 80, BoardChess[ROW][COL]);
            }
        }
    }
}

void Widget::mousePressEvent(QMouseEvent* event){
    if(event->button()==Qt::RightButton){
        HoldingChess = false;
    }
    if(Options::ClickToMove){
        if(!HoldingChess){
            if(event->button()==Qt::LeftButton){
                if(event->pos().x()>60 && event->pos().x()<700 && event->pos().y()>40 && event->pos().y()<680){
                    if(game->chess[(680-event->pos().y())/80][(event->pos().x()-60)/80]->p==game->turn){
                        OldRow = (680-event->pos().y())/80;
                        OldCol = (event->pos().x()-60)/80;
                        valid = game->chess[OldRow][OldCol]->Valid();
                        HoldingChess = true;
                    }
                }
            }
        }
        else{
            if(event->button()==Qt::LeftButton){
                if(event->pos().x()>60 && event->pos().x()<700 && event->pos().y()>40 && event->pos().y()<680){
                    short ROW = (680-event->pos().y())/80;
                    short COL = (event->pos().x()-60)/80;
                    for(short s=0; ; s++){
                        if(valid[s][0]==-1){
                            HoldingChess = false;
                            return;
                        }
                        if(ROW==valid[s][0] && COL==valid[s][1]){
                            short Temp[2] = {7, 0};
                            HoldingChess = false;
                            bool EP = false;
                            switch (game->chess[OldRow][OldCol]->Type()) {
                                case King:
                                    if(OldCol-COL==2 || OldCol-COL==-2 || game->chess[ROW][COL]->Type()==Rook){
                                        if(COL==6||COL==7){
                                            game->Castling(game->turn, true);
                                        }
                                        else{
                                            game->Castling(game->turn, false);
                                        }
                                        break;
                                    }
                                case Pawn:
                                    if(OldCol!=COL && game->chess[ROW][COL]->Type()==Null){
                                        game->EnPassant(OldRow, OldCol, ROW, COL);
                                    }
                                    else if(ROW==Temp[(short)game->turn-1]){
                                        game->MoveChess(OldRow, OldCol, ROW, COL, false);
                                        if(Options::AutoAccending){
                                            game->NewChess(ROW, COL, Queen, game->turn);
                                        }
                                        else{
                                            ////////////////////////////////////////////////////////////////////
                                            ////////////////////////////////////////////////////////////////////
                                            ////////////////////////////////////////////////////////////////////
                                            ////////////////////////////////////////////////////////////////////
                                            ////////////////////////////////////////////////////////////////////
                                            ////////////////////////////////////////////////////////////////////
                                            ////////////////////////////////////////////////////////////////////
                                        }
                                    }
                                    else if(ROW-OldRow==2 || ROW-OldRow==-2){
                                        game->MoveChess(OldRow, OldCol, ROW, COL, true);
                                        if((COL>0 && game->chess[ROW][COL-1]->p!=game->turn && game->chess[ROW][COL-1]->Type()==Pawn)||
                                           (COL<7 && game->chess[ROW][COL+1]->p!=game->turn && game->chess[ROW][COL+1]->Type()==Pawn)){
                                            game->En_passant[0] = ROW;
                                            game->En_passant[1] = COL;
                                            EP = true;
                                        }
                                    }
                                    else{
                                        game->MoveChess(OldRow, OldCol, ROW, COL, true);
                                    }
                                    break;
                                default:
                                    game->MoveChess(OldRow, OldCol, ROW, COL, true);
                                    break;
                            }
                            if(!EP){
                                game->En_passant[0] = game->En_passant[1] = -1;
                            }
                            game->turn = (game->turn==White)? Black : White;
                        }
                    }
                }
            }
        }
    }
}

void Widget::InitProgram(){
    Options::ClickToMove = true;
    Options::LightColorMode = true;
    Options::AutoAccending = true;
    Options::CP = Both;
    QFile file(FILE_CONFIG);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream text(&file);
    while(!text.atEnd()){
        QString str = text.readLine();
        QString name = str.left(3);
        QString value = str.right(1);
        if(QString::localeAwareCompare(name, QString("CTM"))==0){
            Options::ClickToMove = (bool)value.toShort();
        }
        else if(QString::localeAwareCompare(name, QString("LCM"))==0){
            Options::LightColorMode = (bool)value.toShort();
        }
        else if(QString::localeAwareCompare(name, QString("AAC"))==0){
            Options::AutoAccending = (bool)value.toShort();
        }
        else if(QString::localeAwareCompare(name, QString("CTP"))==0){
            Options::CP = (CastlingPath)value.toShort();
        }
    }
    file.close();
}

void Widget::InitWindow(){
    options = new Options();
    acc = new Acc();
    fen = new InputFEN();
    game = new Game();
    timer = new QTimer();
    timer->start(100);
    connect(timer, SIGNAL(timeout()), this, SLOT(RepaintEvent()));
    HoldingChess = false;
    Empty = true;
    setFixedSize(960, 720);
    setWindowTitle(WINDOW_TITLE);
    connect(ui->ExitPro, SIGNAL(clicked()), this, SLOT(Quit()));
    connect(fen->ui->InputFEN_2, SIGNAL(clicked()), this, SLOT(FEN_Input()));
    ui->StopGame->setEnabled(false);
    ui->Output_FEN->setEnabled(false);
    ui->Output_PGN->setEnabled(false);
}

void Widget::IsCheckmate(){
    switch (game->EndOfGame()){
        case NotEnding:
            break;
        case Checkmate:
            game->turn = Neutral;
            ui->StopGame->setEnabled(false);
            ui->Output_FEN->setEnabled(false);
            break;
        case Stillmate:
            game->turn = Neutral;
            ui->StopGame->setEnabled(false);
            ui->Output_FEN->setEnabled(false);
            break;
    }
}

void Widget::RepaintEvent(){
    repaint();
}

void Widget::Quit(){
    QApplication* app;
    app->quit();
}

void Widget::FEN_Input(){
    game->InitBoard(1, fen->FEN);
    ui->StopGame->setEnabled(true);
    ui->Output_FEN->setEnabled(true);
    ui->Output_PGN->setEnabled(true);
}

void Widget::on_StdStart_clicked(){
    game->InitBoard(0, "");
    HoldingChess = false;
    Empty = false;
    ui->StopGame->setEnabled(true);
    ui->Output_FEN->setEnabled(true);
    ui->Output_PGN->setEnabled(true);
    update();
}


void Widget::on_StopGame_clicked(){
    game = new Game();
    HoldingChess = false;
    Empty = true;
    ui->StdStart->setEnabled(true);
    ui->StopGame->setEnabled(false);
    ui->Output_FEN->setEnabled(false);
    ui->Output_PGN->setEnabled(false);
}


void Widget::on_Options_clicked(){
    options->setWindowModality(Qt::ApplicationModal);
    options->show();
}

void Widget::on_Input_clicked(){
    fen->setWindowModality(Qt::ApplicationModal);
    fen->setFixedSize(400, 200);
    fen->setWindowTitle(WINDOW_TITLE_FEN);
    fen->ui->label->setText("请输入棋局的FEN字符串：");
    fen->ui->InputFEN_2->show();
    fen->ui->FEN_String->setPlainText("");
    fen->show();
}


void Widget::on_Output_FEN_clicked(){
    fen->setWindowModality(Qt::ApplicationModal);
    fen->setFixedSize(400, 200);
    fen->setWindowTitle(WINDOW_TITLE_FEN);
    fen->ui->label->setText("该棋局的FEN字符串：");
    fen->ui->InputFEN_2->hide();
    fen->ui->FEN_String->setPlainText(game->FEN());
    fen->show();
}

