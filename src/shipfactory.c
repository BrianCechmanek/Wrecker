#include "shipfactory.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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

	ShipRoomList *rooms = shipType->mRequiredRoomList;
	printf("Required Rooms:");
	while (rooms != NULL)
	{
		printf("%s ",rooms->mRoomName);
		rooms = rooms->mNext;
	}
	printf("\n");
}