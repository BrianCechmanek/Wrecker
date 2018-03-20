#ifndef __ROOMFACTORY__H__
#define __ROOMFACTORY__H__

#include "krng.h"

int LoadRoomFactory();
void CreateRoom(int maxWeight, struct krng *rng);

#endif