/*
 *  Wrecker
 *  IO.c 
 *  Input/Output functions; controls; interacting with system.
 */

#include "IO.h"

#include "defines.h"
#include "wrecker.h"
#include "inputManager.h"
#include "event.h"
#include "diana.h"
#include "ECS.h"
#include "components.h"
#include "systems.h"
#include "dbg.h"

#if __WINDOWS__
#include <time.h>
#else
#include <sys/time.h>
#endif
/*
 * Current function for pulling game time.
 * It's probably bad for some reason.
 */
uint64_t getTicks(void)
{
#if __WINDOWS__

#else
    struct timeval t;
    gettimeofday(&t, NULL);
	return t.tv_sec*1000 + t.tv_usec/1000;
#endif
	return 16;
}

void initWreckState(void)
{
    Wrecker = emalloc(sizeof(gameState));
    Wrecker->currentState = 1;
    Wrecker->lastActual = getTicks();
    Wrecker->clock = 0;
    Wrecker->nextUpdate = 0;
    Wrecker->timeCurrent = 0;

    Wrecker->screenHeight = SCREEN_H;
    Wrecker->screenWidth = SCREEN_W;
}

// Reworked; assigns extra initial memory to speed up resizing.
// TODO: Rework to be more flexible
void initDisplayBuffer(void)
{
    displayBuffer = emalloc( DBUFF_MEM_HEIGHT * sizeof(*displayBuffer));
    displayBuffer[0] = emalloc( DBUFF_MEM_HEIGHT * DBUFF_MEM_WIDTH * sizeof(  displayBuffer ));
    for (int i = 1; i < DBUFF_MEM_HEIGHT; i++){
        displayBuffer[i] = displayBuffer[0] + i * DBUFF_MEM_WIDTH;
    }
}

//TODO  Resize window memory?
void _resizeWindow(void)
{
   int newWidth  =  terminal_state(TK_WIDTH);
   int newHeight =  terminal_state(TK_HEIGHT);

    Wrecker->screenWidth  = newWidth;
    Wrecker->screenHeight = newHeight;
    //clearScreen();
    //redrawScreen(); ? Maybe just use clearScreen to mark new tiles for draw.
}
         
void initEventSys(){
    eventSys = newEvent_s();
    //registerEvents();
}

/*
 * Inits game state and allocates memory for DIANA
 */

void initWrecker()
{
    initWreckState();
    initDisplayBuffer();
    initEventSys();
    initECS();
}

void updateGame( double delta )
{
    WRECK( process, delta );
}

void render(double delta)
{
    //WRECK(processSystem, renderSystem, 0);
	terminal_refresh();
}

void handleInput( int code )
{
    if (code == TK_RESIZED){
        _resizeWindow();
    }

    if (code == TK_MOUSE_MOVE){
        input_mouseX = (float) terminal_state(TK_MOUSE_X);
        input_mouseY = (float) terminal_state(TK_MOUSE_Y);
    } else {
        checkCommand( code );
    }
    return;
}

void *emalloc(size_t size )
{
    void *p;
    p = malloc( size );
    if (!p){
        fprintf(stderr, "Memory allocation error. FILE: %s LINE: %d\n", __FILE__, __LINE__); 
        exit(1); 
    }
    return p; 
}
