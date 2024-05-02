#ifndef CHESS_H
#define CHESS_H

#include <QString>
#include <QByteArray>

#include "options.h"

enum ChessType{
    Null, King, Queen, Bishop, kNight, Rook, Pawn
};

enum Player{
    Neutral, White, Black
};

enum End{
    NotEnding, Checkmate, Stillmate
};

class ChessBase{
    public:
        short r, c;
        Player p;

        ChessBase()=default;
        ChessBase(short, short, Player);
        virtual ~ChessBase()=default;
        virtual ChessType Type()=0;
        virtual short** Valid()=0;
};

class Game{
    public:
        Player turn;
        static ChessBase* chess[8][8];
        static ChessBase* chess2[8][8];
        static bool _Castling[2][2];
        static short En_passant[2];

        Game();
        void InitBoard(short, QString);
        static void NewChess(short, short, ChessType, Player);
        static void MoveChess(short, short, short, short, bool);
        static bool KingInCheck(Player, short, short);
        static void CopyChess();
        static void NewChess2(short, short, ChessType, Player);
        static void MoveChess2(short, short, short, short);
        static bool KingInCheck2(Player, short, short);
        static void EnPassant(short, short, short, short);
        static void Castling(Player, bool);
        static bool CanCastling(Player, bool);
        static short* FindKing(Player);
        End EndOfGame();
        QString FEN();
};

template <ChessType __CT>
class Chess: public ChessBase{
    public:
        short r, c;
        Player p;

        Chess()=default;
        ~Chess()=default;
        Chess(short, short, Player);
        ChessType Type();
        short** Valid();
};

#endif
