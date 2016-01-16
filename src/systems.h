#ifndef _SYSTEMS_H
#define _SYSTEMS_H
/*
 * Systems.h
 * Declaring global systems ID
 * System Management functions
 */

#include "diana.h"

int initMovementSystem(void);
void p_Movement( struct diana *diana, void *ud, unsigned int e, float d);

int initRenderSystem(void);
void p_Render(struct diana *diana, void *ud, unsigned int e, float d);
void render(void);

extern unsigned int renderSystem;
extern unsigned int movementSystem;

#endif