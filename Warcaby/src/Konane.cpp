#include "../headers/Includes.h"
#include "../headers/Position.h"


int main(int argc, char** argv) {

    
    SDL_Init(SDL_INIT_VIDEO);   // Initialize SDL2

    SDL_Window* window;        // Declare a pointer to an SDL_Window

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "Hawaian Gmae Konane",         //    const char* title
        SDL_WINDOWPOS_CENTERED,  //    int x: initial x position
        SDL_WINDOWPOS_CENTERED,  //    int y: initial y position
        640,                      //    int w: width, in pixels
        480,                      //    int h: height, in pixels
        SDL_WINDOW_SHOWN        //    Uint32 flags: window options, see docs
    );

    // Check that the window was successfully made
    if (window == NULL) {
        // In the event that the window could not be made...
        const char* logText = "Window creation failed";
        SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL, logText);
        SDL_Quit();
        return 1;
    }
    else
    {
        const char* logText = "Window creation succesful";
        //SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL);
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, logText);
    }
    const char* msgBogTitle = "Test Message Box Title";
    const char* msgBoxContent = "Test Message Box Title";
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, msgBogTitle, msgBoxContent, window);
    // The window is open: enter program loop (see SDL_PollEvent)
    SDL_Delay(3000);  // Wait for 3000 milliseconds, for example

    // Close and destroy the window
    SDL_DestroyWindow(window);

    
    // Clean up SDL2 and exit the program
    SDL_Quit();

    /*Position<6> a;
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
    }*/
    return 0;
}
