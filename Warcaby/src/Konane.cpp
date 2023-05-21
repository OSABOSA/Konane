#include "../headers/Includes.h"
#include "../headers/Position.h"



int main(int argc, char** argv) {

    Position<6> a;
    a.showBoard(a.piece);
    a.removePiece(a.black, a.piece);
    a.removePiece(a.white, a.piece);
    bool isPlayerWhite = false;
    while (!a.noMoves(isPlayerWhite, a.piece))
    {
        a.playerMove(isPlayerWhite, a.piece);
        isPlayerWhite = !isPlayerWhite;
    }
    if (isPlayerWhite)
    {
        std::cout << "Black wins";
    }
    else
    {
        std::cout << "White wins";
    }
    return 0;
}
