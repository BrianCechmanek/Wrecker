#ifndef _ECS_H
#define _ECS_H

// Convenience functions for ECS

#include "util.h"
#include "wrecker.h"

struct componentInfo
{
    char *name;
    size_t size;
    unsigned int flags;
    componentID *id;
};

// Create a list of components and return the Diana error result
int ecs_createComponents(struct diana *diana, unsigned int count, struct componentInfo *components);

struct systemInfo
{
    char *name;
    void (*starting)(struct diana *, void *);
    void (*process)(struct diana *, void *, unsigned int, float);
    void (*ending)(struct diana *, void *);
    void (*subscribed)(struct diana *, void *, unsigned int);
    void (*unsubscribed)(struct diana *, void *, unsigned int);
    void *userData;
    unsigned int flags;
};
const struct systemInfo ECS_SYSTEM_INFO_DEFAULT = { "(unnamed)", 0, 0, 0, 0, 0, 0, DL_SYSTEM_FLAG_PASSIVE };

// Create a system
int ecs_createSystem(struct diana *diana, struct systemInfo *info, systemID *system);

// Get a system to watch a list of components
int ecs_watchComponents(struct diana *diana, systemID system, unsigned int count, componentID *components);

void ecs_initialize(void);

#endif
