#ifndef _SYSTEMS_H
#define _SYSTEMS_H
/*
 * Systems.h
 * Declaring global systems ID
 * System Management functions
 */

#include "diana.h"
#include "wrecker.h"


int movement_createSystem(struct diana *diana);

int render_createSystem(struct diana *diana);
void render(double);

extern systemID renderSystemId;
extern systemID movementSystemId;

#endif
