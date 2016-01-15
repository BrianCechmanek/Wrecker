#include "systems.h"

#include "wrecker.h"
#include "components.h"
#include "diana.h"

#define NORMAL_SYSTEM 0
#define PASSIVE_SYSTEM 1

static unsigned int renderSystem;

int buildRenderSystem(void)
{
    WRECK(createSystem, "render", NULL, p_Render, NULL, NULL, NULL, NULL,
                         NORMAL_SYSTEM, &renderSystem);
    WRECK(watch, renderSystem, Position_c);
    WRECK(watch, renderSystem, Render_c);
    return 0;
}


static void p_Render( struct diana *diana, void *ud, unsigned int e, float d)
{
    Render_c *r;
    Position_c *p;

    int x, y;
    WRECK( getComponent, e, Render, (void **)&p);
    WRECK( getComponent, e, Position, (void **)&v);

    x = rLoc->x; y = rLoc->y;
    displayBuffer[x][y].code = r->code;
    displayBuffer[x][y].foreColor = r->foreColor;
    displayBuffer[x][y].backColor = r->backColor;
}

