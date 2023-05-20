#include "../headers/Includes.h"
#include "../headers/Position.h"



int main(int argc, char** argv) {

    Position<6> a;
    a.showBoard();

    while (a.piece[a.empty].count() < 1)
    {
        a.flipSquare(a.selectSquare());
        a.showBoard();
    }
    a.findLegalPositions(1, a.piece);
    return 0;
}
