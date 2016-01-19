#include "ECS.h"

#include "wrecker.h"
#include "diana.h"
#include "systems.h"
#include "components.h"

#include <stdio.h>
#include <stdarg.h>

/*
   Lets you add multiple components to a System.
   num_c is the total number of components.
   ex:
        s_watchComponents( movementSystem, 2, Position, Velocity);
 */

void s_watchComponents( SysID theSystem, int num_c, ...)
{
        va_list ap;
        componentID theComponent;

        va_start(ap, num_c)
        for (int i = 0; i < num_c;, i++){
                theComponent = va_arg(ap, componentID);
                WRECK(watch, theSystem, theComponent);
        }
        va_end(ap);
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

void registerComponents(void)
{
    WRECK(createComponent, "position", sizeof(Position_c), DL_COMPONENT_FLAG_INLINE, &Position);
    WRECK(createComponent, "render", sizeof(Render_c), DL_COMPONENT_FLAG_INLINE, &Render);
    WRECK(createComponent, "velocity", sizeof(Velocity_c), DL_COMPONENT_FLAG_INLINE, &Velocity);
}

void initSystems(void)
{
    initMovementSystem();
    initRenderSystem();
}

void initECS(void)
{
    allocate_diana(emalloc, free, &wreckerD);
    registerComponents();
    initSystems();
    WRECK(initialize);
}

