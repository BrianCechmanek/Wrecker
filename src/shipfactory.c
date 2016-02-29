#include "shipfactory.h"

#include <stdio.h>
#include <string.h>

#include "krng.h"

static const char * SHIPCLASSES_PATH = "data/ships/ShipClasses.txt";

#define DEFAULT_DEFINITIONS_COUNT 10;
#define DEFAULT_DEFINITIONS_COUNT_INCREMENT 5;

void SetString(char* src, char ** dst)
{
	sscanf(src, "%*[^:]:%[^\]]", dst);
}

void SetInt(char* src, int * dst)
{
	sscanf(src, "%*[^:]:%d]", dst);
}


// WHY THE FUCK DOESN'T C HAVE PASS BY REFERENCE!!>!>!>!>!>?!?!?!?!?!?!?!
void ShipDefParseLine(char * line, ShipDef *def)
{
	int CurrentChar = 1; // skip the opening [ character
	if (*(line + CurrentChar) == 'N')
	{
		SetString(line, &(def->mName));
	}
	else if (*(line + CurrentChar) == 'M')
	{
		CurrentChar++;
		if (*(line + CurrentChar) == 'A')
		{
			CurrentChar += 2; // Can skip the X
			if (*(line + CurrentChar) == 'W')
			{
				SetInt(line, &def->mMaxWeight);
			}

			else if (*(line + CurrentChar) == 'C')
			{
				SetInt(line, &def->mMaxCrew);
			}
		}
		else if (*(line + CurrentChar) == 'I')
		{
			CurrentChar += 2; // Can skip the X
			if (*(line + CurrentChar) == 'W')
			{
				SetInt(line, &def->mMinWeight);
			}
			else if (*(line + CurrentChar) == 'C')
			{
				SetInt(line, &def->mMinCrew);
			}
		}
	}
}

void LoadShipDef(char * path, ShipDef *shipDef)
{
	FILE * ShipdefFile = fopen(path, "rt");
	char line[120];
	while (fgets(line, 120, ShipdefFile) != NULL)
	{
		ShipDefParseLine(line, shipDef);
	}
}

int LoadFactory(ShipDef ** shipDefinitions)
{
	unsigned numberOfDefinitions = 0;
	unsigned maxNumberOfDefinitions = DEFAULT_DEFINITIONS_COUNT;

	*shipDefinitions = malloc(maxNumberOfDefinitions * sizeof(ShipDef));

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
}