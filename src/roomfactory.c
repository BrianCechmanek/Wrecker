#include "shipfactory.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HULL_PANEL_WEIGHT 4
#define INNER_WALL_WEIGHT 2
#define FLOOR_WEIGHT 1

#define MAXROOMSIZE_WIDTH 8
#define MAXROOMSIZE_HEIGHT 8

#define WALL_CODE 0
#define FLOOR_CODE 1

int LoadRoomFactory()
{
	
}

struct linkedList {
	int xPos;
	int yPos;
	struct linkedList *next;
	struct linkedList *last;
};

void CreateRoom(int maxWeight, struct krng *rng)
{
	/* grab all the shit required for the room */

	// Figure out the maximum size we can have for a square room
	int maxSquareSize = (maxWeight - (4 * HULL_PANEL_WEIGHT)) / ((4 * HULL_PANEL_WEIGHT) + (2 * FLOOR_WEIGHT));

	// Randomly change the size so not everyroom is square
	int offset = 0;
	int wOrH = 0;
	if (maxSquareSize > 2)
	{
		offset = krng_rand_rangei(rng, 0, maxSquareSize - 2); // Don't want any rooms smaller than 2 tiles in any direction
		wOrH = krng_rand_rangei(rng, 0, 1);
	}
	

	int width = wOrH == 0 ? maxSquareSize + offset : maxSquareSize - offset;
	int height = wOrH == 0 ? maxSquareSize - offset : maxSquareSize + offset;
	unsigned newWeight = (2 * width * HULL_PANEL_WEIGHT) + (2 * height * HULL_PANEL_WEIGHT) + (4 * HULL_PANEL_WEIGHT) + (width * height * FLOOR_WEIGHT);
	while (newWeight > maxWeight)
	{
		width -= 1;
		height -= 1;
		newWeight = (2 * width * HULL_PANEL_WEIGHT) + (2 * height * HULL_PANEL_WEIGHT) + (4 * HULL_PANEL_WEIGHT) + (width * height * FLOOR_WEIGHT);
	}

	printf("%d %d: %d %d\n", maxWeight, newWeight, width, height);
}