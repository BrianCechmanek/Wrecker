#include "systems.h"

#include "wrecker.h"
#include "components.h"
#include "diana.h"

#define NORMAL_SYSTEM 0
#define PASSIVE_SYSTEM 1

unsigned int renderSystem;

int initRenderSystem(void)
{
    WRECK(createSystem, "render", NULL, p_Render, NULL, NULL, NULL, NULL,
                         PASSIVE_SYSTEM, &renderSystem);
    WRECK(watch, renderSystem, Position);
    WRECK(watch, renderSystem, Render);
    return 0;
}

void p_Render( struct diana *diana, void *ud, unsigned int e, float d)
{
    Render_c *r;
    Position_c *p;

    int x, y;
    WRECK( getComponent, e, Render, (void **)&r);
    WRECK( getComponent, e, Position, (void **)&p);

    x = p->x; y = p->y;
    displayBuffer[x][y].code = r->code;
    //displayBuffer[x][y].foreColor = r->foreColor;
    //displayBuffer[x][y].backColor = r->backColor;
}
