#ifndef __ShipDef__H__
#define __ShipDef__H__

//CUSTOM HEADER START
typedef struct ShipDefinition{ char mName[60];	unsigned mMaxWeight, mMinWeight;	unsigned mMinCrew, mMaxCrew; } ShipDef;
//CUSTOM HEADER END

void LoadShipDef(char * path, ShipDef *def);
#endif
