#include <diana.h>

#include "ECS.h"
#include "dbg.h"
#include "wrecker.h"

#include <stdarg.h>
#include <stdio.h>

// Includes for components
#include "health.h"
#include "resistance.h"
#include "components.h" // TODO delete this header, break it up into components

// Includes for systems
#include "systems.h" // TODO delete this header, break it up into components

static void _registerComponents(struct diana *diana)
{
    struct componentInfo components[] = {
        { "eType",      sizeof(eType_c),                    DL_COMPONENT_FLAG_INLINE, &eTypeID },
        { "Position",   sizeof(Position_c),                 DL_COMPONENT_FLAG_INLINE, &PositionID },
        { "Render",     sizeof(Render_c),                   DL_COMPONENT_FLAG_INLINE, &RenderID },
        { "Velocity",   sizeof(Velocity_c),                 DL_COMPONENT_FLAG_INLINE, &VelocityID },
        { "Model",      sizeof(Model_c),                    DL_COMPONENT_FLAG_INLINE, &ModelID },
        { "Map",        sizeof(Map_c),                      DL_COMPONENT_FLAG_INLINE, &MapID },
        { "RNG",        sizeof(RNG_c),                      DL_COMPONENT_FLAG_INLINE, &RNGID },
        { "Health",     sizeof(struct healthComponent),     DL_COMPONENT_FLAG_INLINE, &HealthID },
        { "Resistance", sizeof(struct resistanceComponent), DL_COMPONENT_FLAG_INLINE, &ResistanceID },
    };
    int error = ecs_createComponents(diana, ARRAY_SIZE(components), components);

    log_info("Components Loaded.");
    return;
}

static void _initSystems(struct diana *diana)
{
    movement_createSystem(diana);
    render_createSystem(diana);
}

void ecs_initialize()
{
    allocate_diana(emalloc, free, &wreckerD);
    _registerComponents(wreckerD);
    _initSystems(wreckerD);
    WRECK(initialize);
}

int ecs_createComponents(struct diana *diana, unsigned int count, struct componentInfo *components)
{
    int error = DL_ERROR_NONE;

    int i;
    for (int i = 0; i < count; i++)
    {
        struct componentInfo info = components[i];
        int error = diana_createComponent(diana, info.name, info.size, info.flags, info.id);
        if (error != DL_ERROR_NONE)
        {
            // Any failure in creating components is fatal; abandon other operations
            log_err("error %u: failed to create component %s of size %zu with flags %u", error, info.name, info.size, info.flags);
            break;
        }
    }

    return error;
}

int ecs_createSystem(struct diana *diana, struct systemInfo *info, systemID *system)
{
    int error = diana_createSystem(diana
        , info->name
        , info->starting
        , info->process
        , info->ending
        , info->subscribed
        , info->unsubscribed
        , info->userData
        , info->flags
        , system);
    if (error != DL_ERROR_NONE)
    {
        log_err("error %u: failed to create system %s", error, info->name);
    }

    return error;
}

int ecs_watchComponents(struct diana *diana, systemID system, unsigned int count, componentID *components)
{
    int error = DL_ERROR_NONE;

    int i;
    for (int i = 0; i < count; i++)
    {
        componentID component = components[i];
        int error = diana_watch(diana, system, component);
        if (error != DL_ERROR_NONE)
        {
            // Any failure in creating components is fatal; abandon other operations
            log_err("error %u: failed to put system %u watch on component %u", error, system, component);
            break;
        }
    }

    return error;
}
