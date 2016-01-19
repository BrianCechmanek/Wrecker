#ifndef _SYSTEMS_H
#define _SYSTEMS_H
/*
 * Systems.h
 * Declaring global systems ID
 * System Management functions
 */

#include "diana.h"
#include "wrecker.h"


int initMovementSystem(void);
void p_Movement( struct diana *diana, void *ud, entID e, float d);

int initRenderSystem(void);
void p_Render(struct diana *diana, void *ud, entID e, float d);
void render(void);

extern sysID renderSystem;
extern sysID movementSystem;

#endif
