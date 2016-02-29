#ifndef __SHIPFACTORY__H__
#define __SHIPFACTORY__H__

typedef struct ShipDefinition
{
	char mName[60];
	unsigned mMaxWeight, mMinWeight;
	unsigned mMinCrew, mMaxCrew;
} ShipDef;

int LoadFactory(ShipDef ** shipDefinitions);
void CreateShip(ShipDef * shipDefinitions, int numberOfDefs);

#endif