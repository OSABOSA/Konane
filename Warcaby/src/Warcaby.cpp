#include "../headers/Includes.h"
#include "../headers/Position.h"


int main(int argc, char** argv) {

    Position<6> a;
    a.showBoard();

    while (a.piece[a.empty].count() < 10)
    {
        a.flipSquare(a.selectSquare());
        a.showBoard();
    }
    
    return 0;
}
