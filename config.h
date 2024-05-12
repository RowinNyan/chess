#ifndef CONFIG_H
#define CONFIG_H

#include <QFont>

enum ChessType{
    Null, King, Queen, Bishop, kNight, Rook, Pawn
};

enum Player{
    Neutral, White, Black
};

enum End{
    NotEnding, Checkmate, Stillmate
};

#define WINDOW_TITLE "国际象棋"
#define WINDOW_TITLE_CONFIG "设置"
#define WINDOW_TITLE_FEN "FEN"
#define WINDOW_TITLE_ACC "升变"

#define FONT_MAIN ":/res/WenQuan.ttf"
#define ICON_CHESS 
#define FILE_CONFIG "config"

#define IMG_BOARD ":/res/Board.png"
#define IMG_CHECK ":/res/Checked.png"
#define IMG_WHITE_KING ":/res/WhiteKing.png"
#define IMG_WHITE_QUEEN ":/res/WhiteQueen.png"
#define IMG_WHITE_BISHOP ":/res/WhiteBishop.png"
#define IMG_WHITE_KNIGHT ":/res/WhiteKnight.png"
#define IMG_WHITE_ROOK ":/res/WhiteRook.png"
#define IMG_WHITE_PAWN ":/res/WhitePawn.png"
#define IMG_BLACK_KING ":/res/BlackKing.png"
#define IMG_BLACK_QUEEN ":/res/BlackQueen.png"
#define IMG_BLACK_BISHOP ":/res/BlackBishop.png"
#define IMG_BLACK_KNIGHT ":/res/BlackKnight.png"
#define IMG_BLACK_ROOK ":/res/BlackRook.png"
#define IMG_BLACK_PAWN ":/res/BlackPawn.png"
#define IMG_VALID_BLOCK ":/res/ValidBlock.png"
#define IMG_VALID_POINT ":/res/ValidPoint.png"

#endif