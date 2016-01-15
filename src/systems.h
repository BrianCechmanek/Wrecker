#ifndef _SYSTEMS_H
#define _SYSTEMS_H
/*
 * Systems.h
 * Declaring global systems ID
 * System Management functions
 */

int buildMovementSystem(void);
void p_Movement( struct diana *diana, void *ud, unsigned int e, float d);

extern unsigned int movementSystem;

#endif
