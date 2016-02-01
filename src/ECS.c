#include "ECS.h"

#include "wrecker.h"
#include "diana.h"
#include "systems.h"
#include "components.h"

#include <stdio.h>
#include <stdarg.h>

/*
 *  _buildComponent is used by _registerComponents.
 *  To add a new component, please add a new _buildComponent() call 
 *  to registerComponents
 */

void _buildComponent( char *name, size_t comp_size, componentID *id)
{
    int err;

    err = diana_createComponent( wreckerD, name, comp_size, DL_COMPONENT_FLAG_INLINE, id);
    check( err == 0, "Error loading component: %s. Error code: %d", name, err);
    return;

error:
    exit(1);
}

void _registerComponents(void)
{
    _buildComponent( "Position", sizeof(Position_c), &PositionID);
    _buildComponent( "Render", sizeof(Render_c), &RenderID);
    _buildComponent( "Velocity", sizeof(Velocity_c), &VelocityID);
    _buildComponent( "Model", sizeof(Model_c), &ModelID);
    _buildComponent( "Map", sizeof(Map_c), &MapID);

    log_info("Components Loaded.");
    return;
}

/*
   Lets you add multiple components to a System.
   num_c is the total number of components.
   ex:
        s_watchComponents( movementSystem, 2, Position, Velocity);
 */

void s_watchComponents( sysID theSystem, int num_c, ...)
{
        va_list ap;
        componentID theComponent;

        va_start(ap, num_c);
        for (int i = 0; i < num_c; i++){
                theComponent = va_arg(ap, componentID);
                WRECK(watch, theSystem, theComponent);
        }
        va_end(ap);
}

void _initSystems(void)
{
    initMovementSystem();
    initRenderSystem();
}

void initECS(void)
{
    allocate_diana(emalloc, free, &wreckerD);
    _registerComponents();
    _initSystems();
    WRECK(initialize);
}

/* Returns a void pointer to the requested entities component.
   Remember to CAST the void pointer after receiving it!
 */
// Change to macro later.

void *getComponent( entID theEntity, componentID theComponent)
{
    void *data;
    WRECK( getComponent, theEntity, theComponent, (void **)&data);
    return data;
}
