#include "shipfactory.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "roomfactory.h"


static const char * SHIPCLASSES_PATH = "data/ships/ShipClasses.txt";

#define DEFAULT_DEFINITIONS_COUNT 10;
#define DEFAULT_DEFINITIONS_COUNT_INCREMENT 5;

int LoadFactory(ShipDef ** shipDefinitions)
{
	unsigned numberOfDefinitions = 0;
	unsigned maxNumberOfDefinitions = DEFAULT_DEFINITIONS_COUNT;

	*shipDefinitions = malloc(maxNumberOfDefinitions * sizeof(struct ShipDefinition));

	FILE * ShipClasses = fopen(SHIPCLASSES_PATH, "rt");

	char line[240];
	char root[80];

	fgets(root, 80, ShipClasses);
	size_t len = strlen(root);
	if (len > 0 && root[len - 1] == '\n') {
		root[--len] = '\0';
	}

	while (fgets(line, 240, ShipClasses) != NULL)
	{
		char fullPath[320];
		size_t len2 = strlen(line);
		if (len2 > 0 && line[len2 - 1] == '\n') {
			line[--len2] = '\0';
		}
		sprintf(fullPath, "%s%s", root, line);
		Init_ShipDef(&(*shipDefinitions)[numberOfDefinitions]);
		LoadShipDef(fullPath, &(*shipDefinitions)[numberOfDefinitions]);
		if (++numberOfDefinitions >= maxNumberOfDefinitions)
		{
			maxNumberOfDefinitions += DEFAULT_DEFINITIONS_COUNT_INCREMENT;
			*shipDefinitions = realloc(*shipDefinitions, maxNumberOfDefinitions * sizeof(ShipDef));
		}
	}
	return numberOfDefinitions;
}

void CreateShip(ShipDef * shipDefinitions, int numberOfDefs, struct krng *rng)
{
	ShipDef * shipType = shipDefinitions + krng_rand_rangei(rng, 0, numberOfDefs - 1);

	unsigned crewSize = krng_rand_rangei(rng, shipType->mMinCrew, shipType->mMaxCrew);
	unsigned shipWeight = krng_rand_rangei(rng, shipType->mMinWeight, shipType->mMaxWeight);

	printf("Created %s Ship, CrewSize is %u, weight is %u \n", shipType->mName, crewSize, shipWeight);

	int weightPercentForRequired = krng_rand_rangei(rng, 40, 60);
	int weightPercentForOptional = krng_rand_rangei(rng, 20, (100 - weightPercentForRequired < 40?100 - weightPercentForRequired - 5:40));

	ShipRoomList *rooms = shipType->mRequiredRoomList;
	printf("Required Rooms:\n");
	int numReqRooms = 0;
	int numOptRooms = 0;
	
	// count how many possible rooms the ship can have. 
	while (rooms != NULL)
	{
		numReqRooms++;
		rooms = rooms->mNext;
	}
	rooms = shipType->mOptionalRoomList;
	while (rooms != NULL)
	{
		numOptRooms++;
		rooms = rooms->mNext;
	}

	int actualNumberOfOptionalRooms = krng_rand_rangei(rng, 0, numOptRooms);
	int differenceBetweenActualOptAndPossibleOpt = numOptRooms - actualNumberOfOptionalRooms;
	
	//Adjust weights to make up for difference, right now give 5% back to required for each opt not used.
	weightPercentForRequired += 5 * differenceBetweenActualOptAndPossibleOpt;
	weightPercentForOptional -= 5 * differenceBetweenActualOptAndPossibleOpt;

	if (numReqRooms != 0)
	{
		int totalWeightForRequiredRooms = shipWeight * (weightPercentForRequired / 100.f);
		rooms = shipType->mRequiredRoomList;

		int weightPerRoom = totalWeightForRequiredRooms / numReqRooms;
		while (rooms != NULL)
		{
			printf("%d: %s Stats: ", numReqRooms, rooms->mRoomName);
			CreateRoom(weightPerRoom, rng);
			rooms = rooms->mNext;
		}
	}

	printf("\n");
	printf("Optional Rooms:\n");
	if (actualNumberOfOptionalRooms != 0)
	{
		int totalWeightForOptionalRooms = shipWeight * (weightPercentForOptional / 100.f);
		rooms = shipType->mOptionalRoomList;

		int weightPerRoom = totalWeightForOptionalRooms / actualNumberOfOptionalRooms;

		while (actualNumberOfOptionalRooms != 0)
		{
			if (rooms == NULL)
			{
				rooms = shipType->mOptionalRoomList;
			}

			int shouldAddRoom = krng_rand_rangei(rng, 0, 1);
			if (shouldAddRoom == 1)
			{
				printf("%d: %s Stats: ", actualNumberOfOptionalRooms, rooms->mRoomName);
				CreateRoom(weightPerRoom, rng);
				actualNumberOfOptionalRooms--;
			}

			rooms = rooms->mNext;
		}
	}
	printf("\n");
	printf("\n");

}