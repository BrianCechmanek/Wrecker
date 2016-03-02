#ifndef __SHIPFACTORY__H__
#define __SHIPFACTORY__H__

#include "ShipDefParser.h"
#include "krng.h"

int LoadFactory(ShipDef ** shipDefinitions);
void CreateShip(ShipDef * shipDefinitions, int numberOfDefs, struct krng *rng);

#endif