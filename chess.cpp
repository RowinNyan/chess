#include "chess.h"

ChessBase* Game::chess[8][8];
ChessBase* Game::chess2[8][8];
bool Game::_Castling[2][2];
short Game::En_passant[2];

ChessBase::ChessBase(short ROW, short COL, Player P){
    r = ROW;
    c = COL;
    p = P;
}

template <ChessType __CT>
Chess<__CT>::Chess(short ROW, short COL, Player P):
ChessBase(ROW, COL, P){
    r = ROW;
    c = COL;
    p = P;
}

template <ChessType __CT>
ChessType Chess<__CT>::Type(){
    return __CT;
}

template <ChessType __CT>
short** Chess<__CT>::Valid(){
    short** i = new short*[1];
    for(short s=0; s<1; s++){
        i[s] = new short[2];
        i[s][0] = -1;
        i[s][1] = -1;
    }
    return i;
}

template <>
short** Chess<King>::Valid(){
    short Temp[2] = {0, 7};
    short** valid = new short*[20];
    for(short i=0; i<20; i++){
        valid[i] = new short[2];
        valid[i][0] = valid[i][1] = -1;
    }
    short cur=0;
    if(r>0 && Game::chess[r-1][c]->p!=p){
        Game::CopyChess();
        Game::MoveChess2(r, c, r-1, c);
        if(!Game::KingInCheck2(p, r-1, c)){
            valid[cur][0] = r-1;
            valid[cur][1] = c;
            cur++;
        }
    }
    if(r<7 && Game::chess[r+1][c]->p!=p){
        Game::CopyChess();
        Game::MoveChess2(r, c, r+1, c);
        if(!Game::KingInCheck2(p, r+1, c)){
            valid[cur][0] = r+1;
            valid[cur][1] = c;
            cur++;
        }
    }
    if(c>0 && Game::chess[r][c-1]->p!=p){
        Game::CopyChess();
        Game::MoveChess2(r, c, r, c-1);
        if(!Game::KingInCheck2(p, r, c-1)){
            valid[cur][0] = r;
            valid[cur][1] = c-1;
            cur++;
        }
    }
    if(c<7 && Game::chess[r][c+1]->p!=p){
        Game::CopyChess();
        Game::MoveChess2(r, c, r, c+1);
        if(!Game::KingInCheck2(p, r, c+1)){
            valid[cur][0] = r;
            valid[cur][1] = c+1;
            cur++;
        }
    }
    if(r>0 && c>0 && Game::chess[r-1][c-1]->p!=p){
        Game::CopyChess();
        Game::MoveChess2(r, c, r-1, c-1);
        if(!Game::KingInCheck2(p, r-1, c-1)){
            valid[cur][0] = r-1;
            valid[cur][1] = c-1;
            cur++;
        }
    }
    if(r>0 && c<7 && Game::chess[r-1][c+1]->p!=p){
        Game::CopyChess();
        Game::MoveChess2(r, c, r-1, c+1);
        if(!Game::KingInCheck2(p, r-1, c+1)){
            valid[cur][0] = r-1;
            valid[cur][1] = c+1;
            cur++;
        }
    }
    if(r<7 && c>0 && Game::chess[r+1][c-1]->p!=p){
        Game::CopyChess();
        Game::MoveChess2(r, c, r+1, c-1);
        if(!Game::KingInCheck2(p, r+1, c-1)){
            valid[cur][0] = r+1;
            valid[cur][1] = c-1;
            cur++;
        }
    }
    if(r<7 && c<7 && Game::chess[r+1][c+1]->p!=p){
        Game::CopyChess();
        Game::MoveChess2(r, c, r+1, c+1);
        if(!Game::KingInCheck2(p, r+1, c+1)){
            valid[cur][0] = r+1;
            valid[cur][1] = c+1;
            cur++;
        }
    }
    if(Game::CanCastling(p, true)){
        switch (Options::CP){
            case Both:
                valid[cur][0] = Temp[(short)p-1];
                valid[cur][1] = 6;
                cur++;
                valid[cur][0] = Temp[(short)p-1];
                valid[cur][1] = 7;
                cur++;
                break;
            case King2Tiles:
                valid[cur][0] = Temp[(short)p-1];
                valid[cur][1] = 6;
                cur++;
                break;
            case KingOnRook:
                valid[cur][0] = Temp[(short)p-1];
                valid[cur][1] = 7;
                cur++;
                break;
        }
    }
    if(Game::CanCastling(p, false)){
        switch (Options::CP){
            case Both:
                valid[cur][0] = Temp[(short)p-1];
                valid[cur][1] = 2;
                cur++;
                valid[cur][0] = Temp[(short)p-1];
                valid[cur][1] = 0;
                cur++;
                break;
            case King2Tiles:
                valid[cur][0] = Temp[(short)p-1];
                valid[cur][1] = 2;
                cur++;
                break;
            case KingOnRook:
                valid[cur][0] = Temp[(short)p-1];
                valid[cur][1] = 0;
                cur++;
                break;
        }
    }
    return valid;
}

template <>
short** Chess<Queen>::Valid(){
    short** valid = new short*[30];
    for(short i=0; i<30; i++){
        valid[i] = new short[2];
        valid[i][0] = valid[i][1] = -1;
    }
    short cur=0;
    short* KingPos = Game::FindKing(p);
    for(short i=1; i<8; i++){
        if(r-i<0 || c-i<0 || Game::chess[r-i][c-i]->p==p){
            break;
        }
        else if(Game::chess[r-i][c-i]->Type()==Null){
            Game::CopyChess();
            Game::MoveChess2(r, c, r-i, c-i);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = r-i;
                valid[cur][1] = c-i;
                cur++;
            }
        }
        else{
            Game::CopyChess();
            Game::MoveChess2(r, c, r-i, c-i);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = r-i;
                valid[cur][1] = c-i;
                cur++;
            }
            break;
        }
    }
    for(short i=1; i<8; i++){
        if(r-i<0 || c+i>7 || Game::chess[r-i][c+i]->p==p){
            break;
        }
        else if(Game::chess[r-i][c+i]->Type()==Null){
            Game::CopyChess();
            Game::MoveChess2(r, c, r-i, c+i);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = r-i;
                valid[cur][1] = c+i;
                cur++;
            }
        }
        else{
            Game::CopyChess();
            Game::MoveChess2(r, c, r-i, c+i);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = r-i;
                valid[cur][1] = c+i;
                cur++;
            }
            break;
        }
    }
    for(short i=1; i<8; i++){
        if(r+i>7 || c-i<0 || Game::chess[r+i][c-i]->p==p){
            break;
        }
        else if(Game::chess[r+i][c-i]->Type()==Null){
            Game::CopyChess();
            Game::MoveChess2(r, c, r+i, c-i);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = r+i;
                valid[cur][1] = c-i;
                cur++;
            }
        }
        else{
            Game::CopyChess();
            Game::MoveChess2(r, c, r+i, c-i);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = r+i;
                valid[cur][1] = c-i;
                cur++;
            }
            break;
        }
    }
    for(short i=1; i<8; i++){
        if(r+i>7 || c+i>7 || Game::chess[r+i][c+i]->p==p){
            break;
        }
        else if(Game::chess[r+i][c+i]->Type()==Null){
            Game::CopyChess();
            Game::MoveChess2(r, c, r+i, c+i);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = r+i;
                valid[cur][1] = c+i;
                cur++;
            }
        }
        else{
            Game::CopyChess();
            Game::MoveChess2(r, c, r+i, c+i);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = r+i;
                valid[cur][1] = c+i;
                cur++;
            }
            break;
        }
    }
    for(short ROW=r-1; ROW>=0; ROW--){
        if(Game::chess[ROW][c]->p==p){
            break;
        }
        if(Game::chess[ROW][c]->Type()==Null){
            Game::CopyChess();
            Game::MoveChess2(r, c, ROW, c);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = ROW;
                valid[cur][1] = c;
                cur++;
            }
        }
        else if(Game::chess[ROW][c]->p!=p){
            Game::CopyChess();
            Game::MoveChess2(r, c, ROW, c);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = ROW;
                valid[cur][1] = c;
                cur++;
            }
            break;
        }
    }
    for(short ROW=r+1; ROW<8; ROW++){
        if(Game::chess[ROW][c]->p==p){
            break;
        }
        if(Game::chess[ROW][c]->Type()==Null){
            Game::CopyChess();
            Game::MoveChess2(r, c, ROW, c);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = ROW;
                valid[cur][1] = c;
                cur++;
            }
        }
        else if(Game::chess[ROW][c]->p!=p){
            Game::CopyChess();
            Game::MoveChess2(r, c, ROW, c);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = ROW;
                valid[cur][1] = c;
                cur++;
            }
            break;
        }
    }
    for(short COL=c-1; COL>=0; COL--){
        if(Game::chess[r][COL]->p==p){
            break;
        }
        if(Game::chess[r][COL]->Type()==Null){
            Game::CopyChess();
            Game::MoveChess2(r, c, r, COL);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = r;
                valid[cur][1] = COL;
                cur++;
            }
        }
        else if(Game::chess[r][COL]->p!=p){
            Game::CopyChess();
            Game::MoveChess2(r, c, r, COL);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = r;
                valid[cur][1] = COL;
                cur++;
            }
            break;
        }
    }
    for(short COL=c+1; COL<8; COL++){
        if(Game::chess[r][COL]->p==p){
            break;
        }
        if(Game::chess[r][COL]->Type()==Null){
            Game::CopyChess();
            Game::MoveChess2(r, c, r, COL);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = r;
                valid[cur][1] = COL;
                cur++;
            }
        }
        else if(Game::chess[r][COL]->p!=p){
            Game::CopyChess();
            Game::MoveChess2(r, c, r, COL);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = r;
                valid[cur][1] = COL;
                cur++;
            }
            break;
        }
    }
    return valid;
}

template <>
short** Chess<Bishop>::Valid(){
    short** valid = new short*[15];
    for(short i=0; i<15; i++){
        valid[i] = new short[2];
        valid[i][0] = valid[i][1] = -1;
    }
    short cur=0;
    short* KingPos = Game::FindKing(p);
    for(short i=1; i<8; i++){
        if(r-i<0 || c-i<0 || Game::chess[r-i][c-i]->p==p){
            break;
        }
        else if(Game::chess[r-i][c-i]->Type()==Null){
            Game::CopyChess();
            Game::MoveChess2(r, c, r-i, c-i);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = r-i;
                valid[cur][1] = c-i;
                cur++;
            }
        }
        else{
            Game::CopyChess();
            Game::MoveChess2(r, c, r-i, c-i);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = r-i;
                valid[cur][1] = c-i;
                cur++;
            }
            break;
        }
    }
    for(short i=1; i<8; i++){
        if(r-i<0 || c+i>7 || Game::chess[r-i][c+i]->p==p){
            break;
        }
        else if(Game::chess[r-i][c+i]->Type()==Null){
            Game::CopyChess();
            Game::MoveChess2(r, c, r-i, c+i);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = r-i;
                valid[cur][1] = c+i;
                cur++;
            }
        }
        else{
            Game::CopyChess();
            Game::MoveChess2(r, c, r-i, c+i);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = r-i;
                valid[cur][1] = c+i;
                cur++;
            }
            break;
        }
    }
    for(short i=1; i<8; i++){
        if(r+i>7 || c-i<0 || Game::chess[r+i][c-i]->p==p){
            break;
        }
        else if(Game::chess[r+i][c-i]->Type()==Null){
            Game::CopyChess();
            Game::MoveChess2(r, c, r+i, c-i);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = r+i;
                valid[cur][1] = c-i;
                cur++;
            }
        }
        else{
            Game::CopyChess();
            Game::MoveChess2(r, c, r+i, c-i);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = r+i;
                valid[cur][1] = c-i;
                cur++;
            }
            break;
        }
    }
    for(short i=1; i<8; i++){
        if(r+i>7 || c+i>7 || Game::chess[r+i][c+i]->p==p){
            break;
        }
        else if(Game::chess[r+i][c+i]->Type()==Null){
            Game::CopyChess();
            Game::MoveChess2(r, c, r+i, c+i);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = r+i;
                valid[cur][1] = c+i;
                cur++;
            }
        }
        else{
            Game::CopyChess();
            Game::MoveChess2(r, c, r+i, c+i);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = r+i;
                valid[cur][1] = c+i;
                cur++;
            }
            break;
        }
    }
    return valid;
}

template <>
short** Chess<kNight>::Valid(){
    short** valid = new short*[10];
    for(short i=0; i<10; i++){
        valid[i] = new short[2];
        valid[i][0] = valid[i][1] = -1;
    }
    short cur=0;
    short* KingPos = Game::FindKing(p);
    if(r>0 && c>1 && Game::chess[r-1][c-2]->p!=p){
        Game::CopyChess();
        Game::MoveChess2(r, c, r-1, c-2);
        if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
            valid[cur][0] = r-1;
            valid[cur][1] = c-2;
            cur++;
        }
    }
    if(r>0 && c<6 && Game::chess[r-1][c+2]->p!=p){
        Game::CopyChess();
        Game::MoveChess2(r, c, r-1, c+2);
        if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
            valid[cur][0] = r-1;
            valid[cur][1] = c+2;
            cur++;
        }
    }
    if(r<7 && c>1 && Game::chess[r+1][c-2]->p!=p){
        Game::CopyChess();
        Game::MoveChess2(r, c, r+1, c-2);
        if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
            valid[cur][0] = r+1;
            valid[cur][1] = c-2;
            cur++;
        }
    }
    if(r<7 && c<6 && Game::chess[r+1][c+2]->p!=p){
        Game::CopyChess();
        Game::MoveChess2(r, c, r+1, c+2);
        if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
            valid[cur][0] = r+1;
            valid[cur][1] = c+2;
            cur++;
        }
    }
    if(r>1 && c>0 && Game::chess[r-2][c-1]->p!=p){
        Game::CopyChess();
        Game::MoveChess2(r, c, r-2, c-1);
        if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
            valid[cur][0] = r-2;
            valid[cur][1] = c-1;
            cur++;
        }
    }
    if(r>1 && c<7 && Game::chess[r-2][c+1]->p!=p){
        Game::CopyChess();
        Game::MoveChess2(r, c, r-2, c+1);
        if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
            valid[cur][0] = r-2;
            valid[cur][1] = c+1;
            cur++;
        }
    }
    if(r<6 && c>0 && Game::chess[r+2][c-1]->p!=p){
        Game::CopyChess();
        Game::MoveChess2(r, c, r+2, c-1);
        if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
            valid[cur][0] = r+2;
            valid[cur][1] = c-1;
            cur++;
        }
    }
    if(r<6 && c<7 && Game::chess[r+2][c+1]->p!=p){
        Game::CopyChess();
        Game::MoveChess2(r, c, r+2, c+1);
        if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
            valid[cur][0] = r+2;
            valid[cur][1] = c+1;
            cur++;
        }
    }
    return valid;
}

template <>
short** Chess<Rook>::Valid(){
    short** valid = new short*[15];
    for(short i=0; i<15; i++){
        valid[i] = new short[2];
        valid[i][0] = valid[i][1] = -1;
    }
    short cur=0;
    short* KingPos = Game::FindKing(p);
    for(short ROW=r-1; ROW>=0; ROW--){
        if(Game::chess[ROW][c]->p==p){
            break;
        }
        if(Game::chess[ROW][c]->Type()==Null){
            Game::CopyChess();
            Game::MoveChess2(r, c, ROW, c);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = ROW;
                valid[cur][1] = c;
                cur++;
            }
        }
        else if(Game::chess[ROW][c]->p!=p){
            Game::CopyChess();
            Game::MoveChess2(r, c, ROW, c);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = ROW;
                valid[cur][1] = c;
                cur++;
            }
            break;
        }
    }
    for(short ROW=r+1; ROW<8; ROW++){
        if(Game::chess[ROW][c]->p==p){
            break;
        }
        if(Game::chess[ROW][c]->Type()==Null){
            Game::CopyChess();
            Game::MoveChess2(r, c, ROW, c);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = ROW;
                valid[cur][1] = c;
                cur++;
            }
        }
        else if(Game::chess[ROW][c]->p!=p){
            Game::CopyChess();
            Game::MoveChess2(r, c, ROW, c);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = ROW;
                valid[cur][1] = c;
                cur++;
            }
            break;
        }
    }
    for(short COL=c-1; COL>=0; COL--){
        if(Game::chess[r][COL]->p==p){
            break;
        }
        if(Game::chess[r][COL]->Type()==Null){
            Game::CopyChess();
            Game::MoveChess2(r, c, r, COL);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = r;
                valid[cur][1] = COL;
                cur++;
            }
        }
        else if(Game::chess[r][COL]->p!=p){
            Game::CopyChess();
            Game::MoveChess2(r, c, r, COL);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = r;
                valid[cur][1] = COL;
                cur++;
            }
            break;
        }
    }
    for(short COL=c+1; COL<8; COL++){
        if(Game::chess[r][COL]->p==p){
            break;
        }
        if(Game::chess[r][COL]->Type()==Null){
            Game::CopyChess();
            Game::MoveChess2(r, c, r, COL);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = r;
                valid[cur][1] = COL;
                cur++;
            }
        }
        else if(Game::chess[r][COL]->p!=p){
            Game::CopyChess();
            Game::MoveChess2(r, c, r, COL);
            if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                valid[cur][0] = r;
                valid[cur][1] = COL;
                cur++;
            }
            break;
        }
    }
    return valid;
}

template <>
short** Chess<Pawn>::Valid(){
    short** valid = new short*[6];
    for(short i=0; i<6; i++){
        valid[i] = new short[2];
        valid[i][0] = valid[i][1] = -1;
    }
    short cur=0;
    short* KingPos = Game::FindKing(p);
    switch (p){
        case White:
            if(r==1){
                if(Game::chess[3][c]->Type()==Null && Game::chess[2][c]->Type()==Null){
                    Game::CopyChess();
                    Game::MoveChess2(r, c, 3, c);
                    if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                        valid[cur][0] = 3;
                        valid[cur][1] = c;
                        cur++;
                    }
                }
            }
            if(Game::En_passant[0]==r &&(Game::En_passant[1]-c==1 || Game::En_passant[1]-c==-1)){
                Game::CopyChess();
                Game::NewChess2(Game::En_passant[0], Game::En_passant[1], Null, Neutral);
                Game::MoveChess2(r, c, r+1, Game::En_passant[1]);
                if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                    valid[cur][0] = r+1;
                    valid[cur][1] = Game::En_passant[1];
                    cur++;
                }
            }
            if(Game::chess[r+1][c]->Type()==Null){
                Game::CopyChess();
                Game::MoveChess2(r, c, r+1, c);
                if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                    valid[cur][0] = r+1;
                    valid[cur][1] = c;
                    cur++;
                }
            }
            if(c>0 && Game::chess[r+1][c-1]->p==Black){
                Game::CopyChess();
                Game::MoveChess2(r, c, r+1, c-1);
                if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                    valid[cur][0] = r+1;
                    valid[cur][1] = c-1;
                    cur++;
                }
            }
            if(c<7 && Game::chess[r+1][c+1]->p==Black){
                Game::CopyChess();
                Game::MoveChess2(r, c, r+1, c+1);
                if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                    valid[cur][0] = r+1;
                    valid[cur][1] = c+1;
                    cur++;
                }
            }
            break;
        case Black:
            if(r==6){
                if(Game::chess[4][c]->Type()==Null && Game::chess[5][c]->Type()==Null){
                    Game::CopyChess();
                    Game::MoveChess2(r, c, 4, c);
                    if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                        valid[cur][0] = 4;
                        valid[cur][1] = c;
                        cur++;
                    }
                }
            }
            if(Game::En_passant[0]==r &&(Game::En_passant[1]-c==1 || Game::En_passant[1]-c==-1)){
                Game::CopyChess();
                Game::NewChess2(Game::En_passant[0], Game::En_passant[1], Null, Neutral);
                Game::MoveChess2(r, c, r-1, Game::En_passant[1]);
                if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                    valid[cur][0] = r-1;
                    valid[cur][1] = Game::En_passant[1];
                    cur++;
                }
            }
            if(Game::chess[r-1][c]->Type()==Null){
                Game::CopyChess();
                Game::MoveChess2(r, c, r-1, c);
                if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                    valid[cur][0] = r-1;
                    valid[cur][1] = c;
                    cur++;
                }
            }
            if(c>0 && Game::chess[r-1][c-1]->p==White){
                Game::CopyChess();
                Game::MoveChess2(r, c, r-1, c-1);
                if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                    valid[cur][0] = r-1;
                    valid[cur][1] = c-1;
                    cur++;
                }
            }
            if(c<7 && Game::chess[r-1][c+1]->p==White){
                Game::CopyChess();
                Game::MoveChess2(r, c, r-1, c+1);
                if(!Game::KingInCheck2(p, KingPos[0], KingPos[1])){
                    valid[cur][0] = r-1;
                    valid[cur][1] = c+1;
                    cur++;
                }
            }
            break;
        default:
            break;
    }
    return valid;
}

Game::Game(){
    _Castling[0][0] = _Castling[0][1] = _Castling[1][0] = _Castling[1][1] = true;
    turn = Neutral;
    for(short i=0; i<8; i++){
        for(short j=0; j<8; j++){
            chess[i][j] = new Chess<Null>(i, j, Neutral);
        }
    }
}

void Game::InitBoard(short mode, QString str){
    _Castling[0][0] = _Castling[0][1] = _Castling[1][0] = _Castling[1][1] = false;
    En_passant[0] = En_passant[1] = -1;
    switch (mode){
        case 0:
            for(short ROW=0; ROW<8; ROW++){
                for(short COL=0; COL<8; COL++){
                    NewChess(ROW, COL, Null, Neutral);
                }
            }
            for(short COL=0; COL<8; COL++){
                NewChess(1, COL, Pawn, White);
            }
            for(short COL=0; COL<8; COL++){
                NewChess(6, COL, Pawn, Black);
            }
            NewChess(0, 0, Rook, White);
            NewChess(0, 1, kNight, White);
            NewChess(0, 2, Bishop, White);
            NewChess(0, 3, Queen, White);
            NewChess(0, 4, King, White);
            NewChess(0, 5, Bishop, White);
            NewChess(0, 6, kNight, White);
            NewChess(0, 7, Rook, White);
            NewChess(7, 0, Rook, Black);
            NewChess(7, 1, kNight, Black);
            NewChess(7, 2, Bishop, Black);
            NewChess(7, 3, Queen, Black);
            NewChess(7, 4, King, Black);
            NewChess(7, 5, Bishop, Black);
            NewChess(7, 6, kNight, Black);
            NewChess(7, 7, Rook, Black);
            turn = White;
            _Castling[0][0] = _Castling[0][1] = _Castling[1][0] = _Castling[1][1] = true;
            break;
        case 1:
        {
            char* ch;
            {
                QByteArray ba = str.toLatin1();
                ch = ba.data();
            }
            short ROW = 7;
            short COL = 0;
            short MODE = 0;
            bool EN_COL = false;
            for(int i=0; ; i++){
                if(ch[i]=='\0'){
                    break;
                }
                if(ROW>=0){
                    if(ch[i]>='1'&&ch[i]<='8'){
                        for(char c='0'; c<ch[i]; c++){
                            NewChess(ROW, COL, Null, Neutral);
                            COL++;
                        }
                    }
                    else if(ch[i]=='/' || ch[i]==' '){
                        ROW--;
                        COL = 0;
                    }
                    else{
                        switch (ch[i]){
                            case 'K':
                                NewChess(ROW, COL, King, White);
                                COL++;
                                break;
                            case 'Q':
                                NewChess(ROW, COL, Queen, White);
                                COL++;
                                break;
                            case 'B':
                                NewChess(ROW, COL, Bishop, White);
                                COL++;
                                break;
                            case 'N':
                                NewChess(ROW, COL, kNight, White);
                                COL++;
                                break;
                            case 'R':
                                NewChess(ROW, COL, Rook, White);
                                COL++;
                                break;
                            case 'P':
                                NewChess(ROW, COL, Pawn, White);
                                COL++;
                                break;
                            case 'k':
                                NewChess(ROW, COL, King, Black);
                                COL++;
                                break;
                            case 'q':
                                NewChess(ROW, COL, Queen, Black);
                                COL++;
                                break;
                            case 'b':
                                NewChess(ROW, COL, Bishop, Black);
                                COL++;
                                break;
                            case 'n':
                                NewChess(ROW, COL, kNight, Black);
                                COL++;
                                break;
                            case 'r':
                                NewChess(ROW, COL, Rook, Black);
                                COL++;
                                break;
                            case 'p':
                                NewChess(ROW, COL, Pawn, Black);
                                COL++;
                                break;
                        }
                    }
                }
                else{
                    if(ch[i]==' '){
                        MODE++;
                    }
                    else{
                        switch (MODE){
                            case 0:
                                switch (ch[i]){
                                    qDebug() << i << ch[i];
                                    case 'w':
                                        turn = White;
                                        break;
                                    case 'b':
                                        turn = Black;
                                        break;
                                }
                                break;
                            case 1:
                                switch (ch[i]){
                                    case 'K':
                                        _Castling[0][0]=true;
                                        break;
                                    case 'Q':
                                        _Castling[0][1]=true;
                                        break;
                                    case 'k':
                                        _Castling[1][0]=true;
                                        break;
                                    case 'q':
                                        _Castling[1][1]=true;
                                        break;
                                    case '-':
                                        break;
                                }
                                break;
                            case 2:
                                if(ch[i]=='-'){
                                    break;
                                }
                                else{
                                    if(!EN_COL){
                                        En_passant[1] = ch[i]-'a';
                                        EN_COL = true;
                                    }
                                    else{
                                        En_passant[0] =(turn==White)? 4 : 3;
                                    }
                                }
                                break;
                        }
                    }
                }
            }
            break;
        }
        default:
            break;
    }
}

void Game::NewChess(short ROW, short COL, ChessType CT, Player P){
    switch (CT){
        case Null:
            chess[ROW][COL] = new Chess<Null>(ROW, COL, Neutral);
            break;
        case King:
            delete chess[ROW][COL];
            chess[ROW][COL] = new Chess<King>(ROW, COL, P);
            break;
        case Queen:
            delete chess[ROW][COL];
            chess[ROW][COL] = new Chess<Queen>(ROW, COL, P);
            break;
        case Bishop:
            delete chess[ROW][COL];
            chess[ROW][COL] = new Chess<Bishop>(ROW, COL, P);
            break;
        case kNight:
            delete chess[ROW][COL];
            chess[ROW][COL] = new Chess<kNight>(ROW, COL, P);
            break;
        case Rook:
            delete chess[ROW][COL];
            chess[ROW][COL] = new Chess<Rook>(ROW, COL, P);
            break;
        case Pawn:
            delete chess[ROW][COL];
            chess[ROW][COL] = new Chess<Pawn>(ROW, COL, P);
            break;
    }
}

void Game::MoveChess(short OldRow, short OldCol, short NewRow, short NewCol, bool Add){
    switch (chess[OldRow][OldCol]->Type()){
        case King:
            switch (OldRow){
                case 0:
                    _Castling[0][0] = _Castling[0][1] = false;
                    break;
                case 7:
                    _Castling[1][0] = _Castling[1][1] = false;
                    break;
            }
            break;
        case Rook:
            switch (OldRow){
                case 0:
                    switch (OldCol){
                        case 0:
                            _Castling[0][1] = false;
                            break;
                        case 7:
                            _Castling[0][0] = false;
                            break;
                    }
                    break;
                case 7:
                    switch (OldCol){
                        case 0:
                            _Castling[1][1] = false;
                            break;
                        case 7:
                            _Castling[1][0] = false;
                            break;
                    }
                    break;
            }
            break;
        default:
            break;
    }
    NewChess(NewRow, NewCol, chess[OldRow][OldCol]->Type(), chess[OldRow][OldCol]->p);
    NewChess(OldRow, OldCol, Null, Neutral);
}

void Game::CopyChess(){
    for(short ROW=0; ROW<8; ROW++){
        for(short COL=0; COL<8; COL++){
            NewChess2(ROW, COL, chess[ROW][COL]->Type(), chess[ROW][COL]->p);
        }
    }
}

void Game::NewChess2(short ROW, short COL, ChessType CT, Player P){
    switch (CT){
        case Null:
            chess2[ROW][COL] = new Chess<Null>(ROW, COL, Neutral);
            break;
        case King:
            delete chess2[ROW][COL];
            chess2[ROW][COL] = new Chess<King>(ROW, COL, P);
            break;
        case Queen:
            delete chess2[ROW][COL];
            chess2[ROW][COL] = new Chess<Queen>(ROW, COL, P);
            break;
        case Bishop:
            delete chess2[ROW][COL];
            chess2[ROW][COL] = new Chess<Bishop>(ROW, COL, P);
            break;
        case kNight:
            delete chess2[ROW][COL];
            chess2[ROW][COL] = new Chess<kNight>(ROW, COL, P);
            break;
        case Rook:
            delete chess2[ROW][COL];
            chess2[ROW][COL] = new Chess<Rook>(ROW, COL, P);
            break;
        case Pawn:
            delete chess2[ROW][COL];
            chess2[ROW][COL] = new Chess<Pawn>(ROW, COL, P);
            break;
    }
}

void Game::MoveChess2(short OldRow, short OldCol, short NewRow, short NewCol){
    NewChess2(NewRow, NewCol, chess2[OldRow][OldCol]->Type(), chess2[OldRow][OldCol]->p);
    NewChess2(OldRow, OldCol, Null, Neutral);
}

bool Game::KingInCheck2(Player P, short ROW, short COL){
    if(ROW>0 && COL>1){
        if(chess2[ROW-1][COL-2]->p!=P && chess2[ROW-1][COL-2]->Type()==kNight){
            return true;
        }
    }
    if(ROW>0 && COL<6){
        if(chess2[ROW-1][COL+2]->p!=P && chess2[ROW-1][COL+2]->Type()==kNight){
            return true;
        }
    }
    if(ROW<7 && COL>1){
        if(chess2[ROW+1][COL-2]->p!=P && chess2[ROW+1][COL-2]->Type()==kNight){
            return true;
        }
    }
    if(ROW<7 && COL<6){
        if(chess2[ROW+1][COL+2]->p!=P && chess2[ROW+1][COL+2]->Type()==kNight){
            return true;
        }
    }
    if(ROW>1 && COL>0){
        if(chess2[ROW-2][COL-1]->p!=P && chess2[ROW-2][COL-1]->Type()==kNight){
            return true;
        }
    }
    if(ROW>1 && COL<7){
        if(chess2[ROW-2][COL+1]->p!=P && chess2[ROW-2][COL+1]->Type()==kNight){
            return true;
        }
    }
    if(ROW<6 && COL>0){
        if(chess2[ROW+2][COL-1]->p!=P && chess2[ROW+2][COL-1]->Type()==kNight){
            return true;
        }
    }
    if(ROW<6 && COL<7){
        if(chess2[ROW+2][COL+1]->p!=P && chess2[ROW+2][COL+1]->Type()==kNight){
            return true;
        }
    }
    for(short r=ROW-1; r>=0; r--){
        if(chess2[r][COL]->p!=P &&(chess2[r][COL]->Type()==Rook || chess2[r][COL]->Type()==Queen)){
            return true;
        }
        else if(chess2[r][COL]->Type()!=Null){
            break;
        }
    }
    for(short r=ROW+1; r<8; r++){
        if(chess2[r][COL]->p!=P &&(chess2[r][COL]->Type()==Rook || chess2[r][COL]->Type()==Queen)){
            return true;
        }
        else if(chess2[r][COL]->Type()!=Null){
            break;
        }
    }
    for(short c=COL-1; c>=0; c--){
        if(chess2[ROW][c]->p!=P &&(chess2[ROW][c]->Type()==Rook || chess2[ROW][c]->Type()==Queen)){
            return true;
        }
        else if(chess2[ROW][c]->Type()!=Null){
            break;
        }
    }
    for(short c=COL+1; c<8; c++){
        if(chess2[ROW][c]->p!=P &&(chess2[ROW][c]->Type()==Rook || chess2[ROW][c]->Type()==Queen)){
            return true;
        }
        else if(chess2[ROW][c]->Type()!=Null){
            break;
        }
    }
    for(short i=1; i<8; i++){
        if(ROW-i<0 || COL-i<0){
            break;
        }
        else if(chess2[ROW-i][COL-i]->p!=P &&(chess2[ROW-i][COL-i]->Type()==Bishop || chess2[ROW-i][COL-i]->Type()==Queen)){
            return true;
        }
        else if(chess2[ROW-i][COL-i]->Type()!=Null){
            break;
        }
    }
    for(short i=1; i<8; i++){
        if(ROW-i<0 || COL+i>7){
            break;
        }
        else if(chess2[ROW-i][COL+i]->p!=P &&(chess2[ROW-i][COL+i]->Type()==Bishop || chess2[ROW-i][COL+i]->Type()==Queen)){
            return true;
        }
        else if(chess2[ROW-i][COL+i]->Type()!=Null){
            break;
        }
    }
    for(short i=1; i<8; i++){
        if(ROW+i>7 || COL-i<0){
            break;
        }
        else if(chess2[ROW+i][COL-i]->p!=P &&(chess2[ROW+i][COL-i]->Type()==Bishop || chess2[ROW+i][COL-i]->Type()==Queen)){
            return true;
        }
        else if(chess2[ROW+i][COL-i]->Type()!=Null){
            break;
        }
    }
    for(short i=1; i<8; i++){
        if(ROW+i>7 || COL+i>7){
            break;
        }
        else if(chess2[ROW+i][COL+i]->p!=P &&(chess2[ROW+i][COL+i]->Type()==Bishop || chess2[ROW+i][COL+i]->Type()==Queen)){
            return true;
        }
        else if(chess2[ROW+i][COL+i]->Type()!=Null){
            break;
        }
    }
    if(ROW>0 && chess2[ROW-1][COL]->p!=P && chess2[ROW-1][COL]->Type()==King){
        return true;
    }
    if(ROW<7 && chess2[ROW+1][COL]->p!=P && chess2[ROW+1][COL]->Type()==King){
        return true;
    }
    if(COL>0 && chess2[ROW][COL-1]->p!=P && chess2[ROW][COL-1]->Type()==King){
        return true;
    }
    if(COL<7 && chess2[ROW][COL+1]->p!=P && chess2[ROW][COL+1]->Type()==King){
        return true;
    }
    if(ROW>0 && COL>0 && chess2[ROW-1][COL-1]->p!=P &&(chess2[ROW-1][COL-1]->Type()==King ||(P==Black && chess2[ROW-1][COL-1]->Type()==Pawn))){
        return true;
    }
    if(ROW>0 && COL<7 && chess2[ROW-1][COL+1]->p!=P &&(chess2[ROW-1][COL+1]->Type()==King ||(P==Black && chess2[ROW-1][COL+1]->Type()==Pawn))){
        return true;
    }
    if(ROW<7 && COL>0 && chess2[ROW+1][COL-1]->p!=P &&(chess2[ROW+1][COL-1]->Type()==King ||(P==White && chess2[ROW+1][COL-1]->Type()==Pawn))){
        return true;
    }
    if(ROW<7 && COL<7 && chess2[ROW+1][COL+1]->p!=P &&(chess2[ROW+1][COL+1]->Type()==King ||(P==White && chess2[ROW+1][COL+1]->Type()==Pawn))){
        return true;
    }
    return false;
}

void Game::EnPassant(short OldRow, short OldCol, short NewRow, short NewCol){
    MoveChess(OldRow, OldCol, NewRow, NewCol, false);
    NewChess(En_passant[0], En_passant[1], Null, Neutral);
}

void Game::Castling(Player P, bool IsShort){
    short Temp[2] = {0, 7};
    if(IsShort){
        MoveChess(Temp[(short)P-1], 4, Temp[(short)P-1], 6, false);
        MoveChess(Temp[(short)P-1], 7, Temp[(short)P-1], 5, false);
    }
    else{
        MoveChess(Temp[(short)P-1], 4, Temp[(short)P-1], 2, false);
        MoveChess(Temp[(short)P-1], 0, Temp[(short)P-1], 3, false);
    }
}

bool Game::CanCastling(Player P, bool IsShort){
    short Temp[2] = {0, 7};
    if(IsShort){
        if(_Castling[(short)P-1][0]==false){
            return false;
        }
        else if(chess[Temp[(short)P-1]][5]->Type()!=Null || chess[Temp[(short)P-1]][6]->Type()!=Null ||
                KingInCheck(P, Temp[(short)P-1], 4) || KingInCheck(P, Temp[(short)P-1], 5) || KingInCheck(P, Temp[(short)P-1], 6)){
            return false;
        }
        else{
            return true;
        }
    }
    else{
        if(_Castling[(short)P-1][1]==false){
            return false;
        }
        else if(chess[Temp[(short)P-1]][3]->Type()!=Null || chess[Temp[(short)P-1]][2]->Type()!=Null || chess[Temp[(short)P-1]][1]->Type()!=Null ||
                KingInCheck(P, Temp[(short)P-1], 4) || KingInCheck(P, Temp[(short)P-1], 3) || KingInCheck(P, Temp[(short)P-1], 2)){
            return false;
        }
        else{
            return true;
        }
    }
}

bool Game::KingInCheck(Player P, short ROW, short COL){
    if(ROW>0 && COL>1){
        if(chess[ROW-1][COL-2]->p!=P && chess[ROW-1][COL-2]->Type()==kNight){
            return true;
        }
    }
    if(ROW>0 && COL<6){
        if(chess[ROW-1][COL+2]->p!=P && chess[ROW-1][COL+2]->Type()==kNight){
            return true;
        }
    }
    if(ROW<7 && COL>1){
        if(chess[ROW+1][COL-2]->p!=P && chess[ROW+1][COL-2]->Type()==kNight){
            return true;
        }
    }
    if(ROW<7 && COL<6){
        if(chess[ROW+1][COL+2]->p!=P && chess[ROW+1][COL+2]->Type()==kNight){
            return true;
        }
    }
    if(ROW>1 && COL>0){
        if(chess[ROW-2][COL-1]->p!=P && chess[ROW-2][COL-1]->Type()==kNight){
            return true;
        }
    }
    if(ROW>1 && COL<7){
        if(chess[ROW-2][COL+1]->p!=P && chess[ROW-2][COL+1]->Type()==kNight){
            return true;
        }
    }
    if(ROW<6 && COL>0){
        if(chess[ROW+2][COL-1]->p!=P && chess[ROW+2][COL-1]->Type()==kNight){
            return true;
        }
    }
    if(ROW<6 && COL<7){
        if(chess[ROW+2][COL+1]->p!=P && chess[ROW+2][COL+1]->Type()==kNight){
            return true;
        }
    }
    for(short r=ROW-1; r>=0; r--){
        if(chess[r][COL]->p!=P &&(chess[r][COL]->Type()==Rook || chess[r][COL]->Type()==Queen)){
            return true;
        }
        else if(chess[r][COL]->Type()!=Null){
            break;
        }
    }
    for(short r=ROW+1; r<8; r++){
        if(chess[r][COL]->p!=P &&(chess[r][COL]->Type()==Rook || chess[r][COL]->Type()==Queen)){
            return true;
        }
        else if(chess[r][COL]->Type()!=Null){
            break;
        }
    }
    for(short c=COL-1; c>=0; c--){
        if(chess[ROW][c]->p!=P &&(chess[ROW][c]->Type()==Rook || chess[ROW][c]->Type()==Queen)){
            return true;
        }
        else if(chess[ROW][c]->Type()!=Null){
            break;
        }
    }
    for(short c=COL+1; c<8; c++){
        if(chess[ROW][c]->p!=P &&(chess[ROW][c]->Type()==Rook || chess[ROW][c]->Type()==Queen)){
            return true;
        }
        else if(chess[ROW][c]->Type()!=Null){
            break;
        }
    }
    for(short i=1; i<8; i++){
        if(ROW-i<0 || COL-i<0){
            break;
        }
        else if(chess[ROW-i][COL-i]->p!=P &&(chess[ROW-i][COL-i]->Type()==Bishop || chess[ROW-i][COL-i]->Type()==Queen)){
            return true;
        }
        else if(chess[ROW-i][COL-i]->Type()!=Null){
            break;
        }
    }
    for(short i=1; i<8; i++){
        if(ROW-i<0 || COL+i>7){
            break;
        }
        else if(chess[ROW-i][COL+i]->p!=P &&(chess[ROW-i][COL+i]->Type()==Bishop || chess[ROW-i][COL+i]->Type()==Queen)){
            return true;
        }
        else if(chess[ROW-i][COL+i]->Type()!=Null){
            break;
        }
    }
    for(short i=1; i<8; i++){
        if(ROW+i>7 || COL-i<0){
            break;
        }
        else if(chess[ROW+i][COL-i]->p!=P &&(chess[ROW+i][COL-i]->Type()==Bishop || chess[ROW+i][COL-i]->Type()==Queen)){
            return true;
        }
        else if(chess[ROW+i][COL-i]->Type()!=Null){
            break;
        }
    }
    for(short i=1; i<8; i++){
        if(ROW+i>7 || COL+i>7){
            break;
        }
        else if(chess[ROW+i][COL+i]->p!=P &&(chess[ROW+i][COL+i]->Type()==Bishop || chess[ROW+i][COL+i]->Type()==Queen)){
            return true;
        }
        else if(chess[ROW+i][COL+i]->Type()!=Null){
            break;
        }
    }
    if(ROW>0 && chess[ROW-1][COL]->p!=P && chess[ROW-1][COL]->Type()==King){
        return true;
    }
    if(ROW<7 && chess[ROW+1][COL]->p!=P && chess[ROW+1][COL]->Type()==King){
        return true;
    }
    if(COL>0 && chess[ROW][COL-1]->p!=P && chess[ROW][COL-1]->Type()==King){
        return true;
    }
    if(COL<7 && chess[ROW][COL+1]->p!=P && chess[ROW][COL+1]->Type()==King){
        return true;
    }
    if(ROW>0 && COL>0 && chess[ROW-1][COL-1]->p!=P &&(chess[ROW-1][COL-1]->Type()==King ||(P==Black && chess[ROW-1][COL-1]->Type()==Pawn))){
        return true;
    }
    if(ROW>0 && COL<7 && chess[ROW-1][COL+1]->p!=P &&(chess[ROW-1][COL+1]->Type()==King ||(P==Black && chess[ROW-1][COL+1]->Type()==Pawn))){
        return true;
    }
    if(ROW<7 && COL>0 && chess[ROW+1][COL-1]->p!=P &&(chess[ROW+1][COL-1]->Type()==King ||(P==White && chess[ROW+1][COL-1]->Type()==Pawn))){
        return true;
    }
    if(ROW<7 && COL<7 && chess[ROW+1][COL+1]->p!=P &&(chess[ROW+1][COL+1]->Type()==King ||(P==White && chess[ROW+1][COL+1]->Type()==Pawn))){
        return true;
    }
    return false;
}

short* Game::FindKing(Player P){
    short* pos = new short[2];
    for(short ROW=0; ROW<8; ROW++){
        for(short COL=0; COL<8; COL++){
            if(chess[ROW][COL]->Type()==King && chess[ROW][COL]->p==P){
                pos[0] = ROW;
                pos[1] = COL;
                return pos;
            }
        }
    }
    return nullptr;
}

End Game::EndOfGame(){
    if(turn==Neutral){
        return NotEnding;
    }
    else{
        for(short ROW=0; ROW<8; ROW++){
            for(short COL=0; COL<8; COL++){
                if(chess[ROW][COL]->p==turn){
                    short** valid = chess[ROW][COL]->Valid();
                    if(valid[0][1]!=-1){
                        return NotEnding;
                    }
                }
            }
        }
        short* KingPos = FindKing(turn);
        if(KingInCheck(turn, KingPos[0], KingPos[1])){
            return Checkmate;
        }
        else{
            return Stillmate;
        }
    }
}
QString Game::FEN(){
    QString Output("");
    for(short ROW=7; ROW>=0; ROW--){
        char Temp = '0';
        for(short COL=0; COL<8; COL++){
            if(chess[ROW][COL]->Type()==Null){
                Temp++;
            }
            else{
                if(Temp != '0'){
                    Output += Temp;
                }
                Temp = '0';
                char Ta;
                switch (chess[ROW][COL]->Type()){
                    case King:
                        Ta = 'K';
                        break;
                    case Queen:
                        Ta = 'Q';
                        break;
                    case Bishop:
                        Ta = 'B';
                        break;
                    case kNight:
                        Ta = 'N';
                        break;
                    case Rook:
                        Ta = 'R';
                        break;
                    case Pawn:
                        Ta = 'P';
                        break;
                }
                switch (chess[ROW][COL]->p){
                    case White:
                        break;
                    case Black:
                        Ta = Ta-'A'+'a';
                        break;
                }
                Output += Ta;
            }
            if(COL==7 && ROW!=0){
                if(Temp != '0'){
                    Output += Temp;
                }
                Output += '/';
            }
        }
    }
    Output += ' ';
    switch (turn){
        case White:
            Output += 'w';
            break;
        case Black:
            Output += 'b';
            break;
    }
    Output += ' ';
    bool _Ca = false;
    if(_Castling[0][0]){
        Output += 'K';
        _Ca = true;
    }
    if(_Castling[0][1]){
        Output += 'Q';
        _Ca = true;
    }
    if(_Castling[1][0]){
        Output += 'k';
        _Ca = true;
    }
    if(_Castling[1][1]){
        Output += 'q';
        _Ca = true;
    }
    if(!_Ca){
        Output += '-';
    }
    Output += ' ';
    if(En_passant[0]>1){
        Output += (char)('a'+En_passant[1]);
        Output += (char)('1'+En_passant[0]);
    }
    else{
        Output += '-';
    }
    Output += " 0 1";
    return Output;
}
