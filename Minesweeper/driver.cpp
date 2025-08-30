#include "window.h"


/*
        Desc: Entry Point For The Minesweeper Application Which Creates
        A Window Instance And Starts The Main Game Loop.

        Preconditions:
            1.) All Required Libraries And Dependencies Are Available

        Postconditions:
            1.) A Window Instance Will Be Created And Initialized
            2.) The Main Game Loop Will Execute Until Application Termination
            3.) Application Will Return 0 On Successful Completion

*/
int main()
{
    Window window;
    window.mainLoop();
    return 0;
}