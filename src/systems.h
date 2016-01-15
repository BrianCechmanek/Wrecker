#ifndef _SYSTEMS_H
#define _SYSTEMS_H
/*
 * Systems.h
 * Declaring global systems ID
 * System Management functions
 */

int buildMovementSystem(void);
void p_Movement( struct diana *diana, void *ud, unsigned int e, float d);

int buildRenderSystem(void);
void p_Render(void);

extern unsigned int movementSystem;
extern unsigned int renderSystem;

#endif
