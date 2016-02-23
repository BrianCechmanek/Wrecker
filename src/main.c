/*   Wrecker V.03
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
#include "model.h"

gameState *Wrecker;
Event_s *eventSys;
struct diana *wreckerD;

cellDisplayBuffer **displayBuffer;

int main(void)
{
	int running = 1;

	Velocity_c ev = { 5, 0, 9 };
	Render_c r = { '@' };
	unsigned int e;

    terminal_open();

	char stringbuffer[256];
	sprintf(stringbuffer, "window.title = 'Wrecker %d.%d.%d'", WRECKER_MAJOR_VERSION, WRECKER_MINOR_VERS, WRECKER_PATCH_VERS);

	terminal_set(stringbuffer);

    initWrecker();
	Position_c pos = { 5.f, 5.f };

	Model_c model;
	model.Cells = malloc(sizeof(ModelCell) * 3);
	model.Cells[0].bgColor = 0xFFFF0000;
	model.Cells[0].fgColor = 0xFF000000;
	model.Cells[0].x = -1;
	model.Cells[0].y = 0;
	model.Cells[0].characterCode = '|';
	model.Cells[0].layer = 0;
	model.Cells[1].bgColor = 0xFFFF0000;
	model.Cells[1].fgColor = 0xFF000000;
	model.Cells[1].x = 0;
	model.Cells[1].y = 0;
	model.Cells[1].characterCode = '-';
	model.Cells[1].layer = 0;
	model.Cells[2].bgColor = 0xFFFF0000;
	model.Cells[2].fgColor = 0xFF000000;
	model.Cells[2].x = 1;
	model.Cells[2].y = 0;
	model.Cells[2].characterCode = '|';
	model.Cells[2].layer = 0;
	model.numberOfCells = 3;

	WRECK(spawn, &e);
	WRECK(setComponent, e, PositionID, &pos);
	WRECK(setComponent, e, ModelID, &model);
	WRECK(setComponent, e, VelocityID, &ev);
	WRECK(signal, e, DL_ENTITY_ADDED);


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
		
		// Draw game
		render(deltaMs); 

    }
    //Call Destructors
    terminal_close();
    return 0;
}
