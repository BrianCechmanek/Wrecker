﻿/*   Wrecker V.03
 *   Main.c
 *   Thought we should eventually build a real game loop instead of
 *   hacking something together. Hopefully this is good enough for a while.
 */

#include <stdlib.h>
#include <stdio.h>
#include "wrecker.h"
#include "IO.h"
#include "event.h"
#include "diana.h"
#include "systems.h"
#include "timer.h"
#include "components.h"

gameState *Wrecker;
Event_s *eventSys;
struct diana *wreckerD;

cellDisplayBuffer **displayBuffer;

int main(void)
{
	int running = 1;

	Velocity_c ev = { 1.5, 0, 9 };
	Render_c r = { '@' };
	unsigned int movementSystem, renderSystem, computeSystem, e, e1;

    terminal_open();

	char stringbuffer[256];
	sprintf(stringbuffer, "window.title = 'Wrecker %d.%d.%d'", WRECKER_MAJOR_VERSION, WRECKER_MINOR_VERS, WRECKER_PATCH_VERS);

	terminal_set(stringbuffer);

	initDisplayBuffer();

    initWrecker();

	WRECK(spawn, &e);
	WRECK(setComponent, e, PositionID, NULL);
	WRECK(setComponent, e, VelocityID, &ev);
	WRECK(signal, e, DL_ENTITY_ADDED);

	WRECK(spawn, &e1);
	WRECK(setComponent, e1, PositionID, NULL);
	WRECK(setComponent, e1, RenderID, &r);
	WRECK(signal, e1, DL_ENTITY_ADDED);

	// Start timer
	tick_t start, time;
	printf("Timer started\n");
	timer_lib_initialize();
	start = timer_current();

    while (running){
		time = timer_elapsed_ticks(start);
		start += time;
		double deltaMs = (double)timer_ticks_to_seconds(time) * 1000.0;
		//printf("%u \n", time);
		//printf("%G \n", deltaMs);

		// Get Input
		while (terminal_has_input())
		{
			int code = terminal_read();
			if (code == TK_ESCAPE || code == TK_CLOSE)
				running = 0;
			else
				handleInput(code);
		}

		//Update Game Elements
		updateGame(deltaMs);

		// Add the fps counter and frametime to the screen
		char timebuffer[60];
		char fpsbuffer[60];
		sprintf(timebuffer, "%G ms", deltaMs);
		unsigned fps = (unsigned)(1000.0 / deltaMs);
		sprintf(fpsbuffer, "%d fps", fps);
		terminal_print(1, 1, timebuffer);
		terminal_print(1, 2, fpsbuffer);

		// Draw game
		render(deltaMs);
    }
    //Call Destructors
    terminal_close();
    return 0;
}
