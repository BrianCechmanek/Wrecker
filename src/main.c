/*   Wrecker V.03
 *   Main.c
 *   Thought we should eventually build a real game loop instead of
 *   hacking something together. Hopefully this is good enough for a while.
 */
#include <stdint.h>

#include "wrecker.h"
#include "IO.h"
#include "event.h"
#include "diana.h"
#include "systems.h"


#define SCREEN_W 80
#define SCREEN_H 25

gameState *wreckState;
Event_s *eventSys;
struct diana *wreckerD;

cellDisplayBuffer displayBuffer[SCREEN_W][SCREEN_H];

int main(void)
{
    terminal_open();
    terminal_set("window.title='Wrecker'");

    initWrecker();

    int running = 1;
    while (running){

        uint64_t actualTime = getTicks();
        if (( actualTime - wreckState->lastActual) > (MS_PER_UPDATE * UPDATE_PER_FRAME) ){
            wreckState->clock += (MS_PER_UPDATE * UPDATE_PER_FRAME);
        } else {
            wreckState->clock += (actualTime - wreckState->lastActual);
        }
        while (wreckState->clock >= wreckState->nextUpdate) {
            wreckState->timeCurrent = wreckState->nextUpdate;
            updateGame();
            wreckState->nextUpdate += MS_PER_UPDATE;
        }
        wreckState->lastActual = getTicks();
        render();

        while( running && terminal_has_input() ){
            int code = terminal_read();
            if (code == TK_ESCAPE || code == TK_CLOSE)
                running = 0;
            else
                handleInput( code );
        }
    }
    //Call Destructors
    terminal_close();
    return 0;
}
