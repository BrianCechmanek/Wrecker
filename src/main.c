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

gameState *Wrecker;
Event_s *eventSys;
struct diana *wreckerD;

cellDisplayBuffer **displayBuffer;

int main(void)
{
    terminal_open();
    terminal_set("window.title='Wrecker'");

    initWrecker();

    int running = 1;
    while (running){

        uint64_t actualTime = getTicks();
        if (( actualTime - Wrecker->lastActual) > (MS_PER_UPDATE * UPDATE_PER_FRAME) ){
            Wrecker->clock += (MS_PER_UPDATE * UPDATE_PER_FRAME);
        } else {
            Wrecker->clock += (actualTime - Wrecker->lastActual);
        }
        while (Wrecker->clock >= Wrecker->nextUpdate) {
            Wrecker->timeCurrent = Wrecker->nextUpdate;
            updateGame();
            Wrecker->nextUpdate += MS_PER_UPDATE;
        }
        Wrecker->lastActual = getTicks();
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
