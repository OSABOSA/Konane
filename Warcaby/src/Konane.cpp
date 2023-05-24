#include "../headers/Includes.h"
#include "../headers/Position.h"



int main(int argc, char** argv) {

    Position<6> a;
    bool isPlayerWhite = false, isBotMove = true;
    a.showBoard(a.piece);
    //a.removePiece(a.black, a.piece);
    //a.removePiece(a.white, a.piece);
    a.removeFirstPiece(isBotMove, a.piece);
    isBotMove = !isBotMove;
    a.removeSecondPiece(isBotMove, a.piece);
    isBotMove = !isBotMove;
   // auto k = a.findBestMove(isPlayerWhite, 4, a.piece);
    //a.showBoard(k.second);
    //std::cout << k.first;
    //_getch();
    //a.findLegalPositions(isPlayerWhite, a.piece);;
    while (!a.noMoves(isPlayerWhite, a.piece))
    {
        a.move(isPlayerWhite, isBotMove, a.piece);
        isPlayerWhite = !isPlayerWhite;
        isBotMove = !isBotMove;
    }
    a.showBoard(a.piece);
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
