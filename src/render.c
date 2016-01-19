#include "systems.h"

#include "wrecker.h"
#include "diana.h"
#include "ECS.h"
#include "components.h"
#include "systems.h"

#define NORMAL_SYSTEM 0
#define PASSIVE_SYSTEM 1

sysID renderSystem;

int initRenderSystem(void)
{
    WRECK(createSystem, "render", NULL, p_Render, NULL, NULL, NULL, NULL,
                         PASSIVE_SYSTEM, &renderSystem);
    s_watchComponents( renderSystem, 2, Position,
                                        Render);

    return 0;
}

void p_Render( struct diana *diana, void *ud, unsigned int e, float d)
{
    Render_c *r;
    Position_c *p;

    int x, y;
    r = getComponent(e, Render);
    p = getComponent(e, Position);

    x = p->x; y = p->y;
    displayBuffer[x][y].code = r->code;
    //displayBuffer[x][y].foreColor = r->foreColor;
    //displayBuffer[x][y].backColor = r->backColor;
}
