#include "widget.h"
#include "./ui_widget.h"
#include "ui_inputfen.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    setFocusPolicy(Qt::StrongFocus);
    ui->setupUi(this);
    InitProgram();
    InitWindow();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent* event){
    {
        ui->Output_PGN->setEnabled(false);
    }
    if(Options::LightColorMode){
        ui->TurnLabel->setStyleSheet("color:#0f0f0f;");
    }
    else{
        ui->TurnLabel->setStyleSheet("color:#f0f0f0;");
        QPainter p(this);
        QPen pen;
        QBrush brush;
        QColor color;
        color.setRgb(15, 15, 15);
        pen.setColor(color);
        brush.setColor(color);
        brush.setStyle(Qt::SolidPattern);
        p.setPen(pen);
        p.setBrush(brush);
        p.drawRect(rect());
    }
    if(InGame){
        switch (game->turn){
            case White:
                ui->TurnLabel->setText("白方回合");
                break;
            case Black:
                ui->TurnLabel->setText("黑方回合");
                break;
            default:
                break;
        }
    }
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
                    if((ROW+COL)%2){
                        BoardBlock[ROW][COL].load(IMG_VALID_BLOCK);
                    }
                    else{
                        BoardBlock[ROW][COL].load(IMG_VALID_BLOCK2);
                    }
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
                            if((ROW+COL)%2){
                                BoardBlock[ROW][COL].load(IMG_VALID_BLOCK);
                            }
                            else{
                                BoardBlock[ROW][COL].load(IMG_VALID_BLOCK2);
                            }
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
        }
        IsCheckmate();
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
            if(game->turn!=Neutral && event->button()==Qt::LeftButton){
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
                                        game->turn = (game->turn==White)? Black : White;
                                        break;
                                    }
                                    game->MoveChess(OldRow, OldCol, ROW, COL, true);
                                    game->turn = (game->turn==White)? Black : White;
                                    break;
                                case Pawn:
                                    if(OldCol!=COL && game->chess[ROW][COL]->Type()==Null){
                                        game->EnPassant(OldRow, OldCol, ROW, COL);
                                        game->turn = (game->turn==White)? Black : White;
                                    }
                                    else if(ROW==Temp[(short)game->turn-1]){
                                        game->MoveChess(OldRow, OldCol, ROW, COL, false);
                                        if(Options::AutoAccending){
                                            game->NewChess(ROW, COL, Queen, game->turn);
                                            game->turn = (game->turn==White)? Black : White;
                                        }
                                        else{
                                            PawnAcc(COL);
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
                                        game->turn = (game->turn==White)? Black : White;
                                    }
                                    else{
                                        game->MoveChess(OldRow, OldCol, ROW, COL, true);
                                        game->turn = (game->turn==White)? Black : White;
                                    }
                                    break;
                                default:
                                    game->MoveChess(OldRow, OldCol, ROW, COL, true);
                                    game->turn = (game->turn==White)? Black : White;
                                    break;
                            }
                            if(!EP){
                                game->En_passant[0] = game->En_passant[1] = -1;
                            }
                        }
                    }
                }
            }
        }
    }
}

void Widget::keyPressEvent(QKeyEvent* event){
    using namespace Qt;
    Key Easter_Egg[10] = {Key_Up, Key_Up, Key_Down, Key_Down, Key_Left, Key_Right, Key_Left, Key_Right, Key_B, Key_A};
    if(event->key()==Easter_Egg[Egg]){
        Egg++;
        if(Egg == 10){
            EasterEgg* ee = new EasterEgg();
            ee->show();
        }
    }
    else{
        Egg = 0;
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
    Egg = 0;
}

void Widget::InitWindow(){
    int fontId = QFontDatabase::addApplicationFont(FONT_MAIN);
    QStringList fontFamilies=QFontDatabase::applicationFontFamilies(fontId);
    QFont font1, font2;
    font1.setFamily(fontFamilies[0]);
    font1.setPointSize(13);
    font2.setFamily(fontFamilies[0]);
    font2.setPointSize(20);
    ui->ExitPro->setFont(font1);
    ui->Input->setFont(font1);
    ui->Options->setFont(font1);
    ui->Output_FEN->setFont(font1);
    ui->Output_PGN->setFont(font1);
    ui->StdStart->setFont(font1);
    ui->StopGame->setFont(font1);
    ui->TurnLabel->setFont(font2);
    setWindowIcon(QIcon(ICON_CHESS));
    options = new Options();
    acc = new Acc();
    fen = new InputFEN();
    game = new Game();
    timer = new QTimer();
    timer->start(100);
    connect(timer, SIGNAL(timeout()), this, SLOT(RepaintEvent()));
    HoldingChess = InGame = false;
    Empty = true;
    setFixedSize(960, 720);
    setWindowTitle(WINDOW_TITLE);
    connect(ui->ExitPro, SIGNAL(clicked()), this, SLOT(Quit()));
    connect(fen->ui->InputFEN_2, SIGNAL(clicked()), this, SLOT(FEN_Input()));
    connect(acc, SIGNAL(Acced()), this, SLOT(AccChess()));
    ui->StopGame->setEnabled(false);
    ui->Output_FEN->setEnabled(false);
    ui->Output_PGN->setEnabled(false);
}

void Widget::IsCheckmate(){
    switch (game->EndOfGame()){
        case NotEnding:
            break;
        case Checkmate:
            switch (game->turn){
                case White:
                    ui->TurnLabel->setText("将死\n黑方胜利");
                    break;
                case Black:
                    ui->TurnLabel->setText("将死\n白方胜利");
                    break;
                default:
                    break;
            }
            game->turn = Neutral;
            ui->StopGame->setEnabled(false);
            ui->Output_FEN->setEnabled(false);
            break;
        case Stillmate:
            ui->TurnLabel->setText("逼和\n和棋");
            game->turn = Neutral;
            ui->StopGame->setEnabled(false);
            ui->Output_FEN->setEnabled(false);
            Empty = true;
            break;
    }
}

void Widget::PawnAcc(short COL){
    acc->P = game->turn;
    acc->COL = COL;
    acc->setWindowModality(Qt::ApplicationModal);
    acc->show();
}

void Widget::RepaintEvent(){
    repaint();
}

void Widget::Quit(){
    QApplication* app;
    app->quit();
}

void Widget::FEN_Input(){
    HoldingChess = Empty = false;
    InGame = true;
    game->InitBoard(1, fen->FEN);
    ui->StopGame->setEnabled(true);
    ui->Output_FEN->setEnabled(true);
    ui->Output_PGN->setEnabled(true);
}

void Widget::AccChess(){
    short Temp1[2] = {7, 0};
    ChessType Temp2[4] = {Queen, Bishop, Rook, kNight};
    game->NewChess(Temp1[(short)game->turn-1], acc->COL, Temp2[acc->I], game->turn);
    game->turn = (game->turn==White)? Black : White;
}

void Widget::on_StdStart_clicked(){
    game->InitBoard(0, "");
    HoldingChess = Empty = false;
    InGame = true;
    ui->StopGame->setEnabled(true);
    ui->Output_FEN->setEnabled(true);
    ui->Output_PGN->setEnabled(true);
    update();
}

void Widget::on_StopGame_clicked(){
    game = new Game();
    HoldingChess = InGame = false;
    Empty = true;
    ui->StdStart->setEnabled(true);
    ui->StopGame->setEnabled(false);
    ui->Output_FEN->setEnabled(false);
    ui->Output_PGN->setEnabled(false);
    ui->TurnLabel->setText("");
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
