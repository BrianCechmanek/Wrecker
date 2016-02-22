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
	displayBuffer = emalloc(DBUFF_MEM_HEIGHT * sizeof(cellDisplayBuffer *));
	displayBuffer[0] = emalloc(DBUFF_MEM_HEIGHT * DBUFF_MEM_WIDTH * sizeof(cellDisplayBuffer));
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
    ecs_initialize();
}

void updateGame( double delta )
{
    WRECK( process, delta );

	WRECK(processSystem, movementSystemId, delta);
}

void DrawDisplayBuffer()
{
	for (int y = 0; y < DBUFF_MEM_HEIGHT; y++){
		for (int x = 0; x < DBUFF_MEM_WIDTH; x++){
			terminal_put(x, y, displayBuffer[y][x].code);
		}
	}
}

void clearScreen()
{
	terminal_clear();
	for (int y = 0; y < DBUFF_MEM_HEIGHT; y++){
		for (int x = 0; x < DBUFF_MEM_WIDTH; x++){
			displayBuffer[y][x].code = SPACE_CHAR;
			displayBuffer[y][x].backColor = 0;
			displayBuffer[y][x].foreColor = 0;
			displayBuffer[y][x].needsUpdate = 0;
		}
	}
}

void render(double delta)
{
	clearScreen();
	WRECK(processSystem, renderSystemId, delta);
	DrawDisplayBuffer();

	// Add the fps counter and frametime to the screen
	char timebuffer[60];
	char fpsbuffer[60];
	sprintf(timebuffer, "%G ms", delta);
	unsigned fps = (unsigned)(1000.0 / delta);
	sprintf(fpsbuffer, "%d fps", fps);
	terminal_print(0, 0, timebuffer);
	terminal_print(0, 1, fpsbuffer);

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
