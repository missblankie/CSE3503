#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "game.h"



void run_gui(GameState * game);

int main(void) {
    // initialize a new checkers game
    GameState game = init_game();

    // Run raylib Gui loop
    run_gui(&game);

   return 0;

}
