#ifndef _ECS_H
#define _ECS_H
/*
 * ECS.h
 * For all functions related to dealing with the ECS.
 */

/*
 *  s_watchComponents( SysID theSystem, int arg_c, ...)
 *  Use this to attach a variable list of Components to a system in list format.
 *  Specify number of components in 2nd argument.
 *  ex:
 *      s_watchComponents( movementSystem, 2, Position, Velocity );
 */

#include "wrecker.h"

#define NORMAL_SYSTEM   0
#define PASSIVE_SYSTEM  1

void s_watchComponents( sysID theSystem, int arg_c, ...);
void *getComponent( entID entity, componentID compID);
void initECS(void);

#endif
