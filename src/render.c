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
		DL_SYSTEM_FLAG_NORMAL, &renderSystem);
    s_watchComponents( renderSystem, 2, PositionID,
                                        RenderID);

    return 0;
}

void p_Render(struct diana *diana, void *user_data, unsigned int entity, float delta)
{
	struct position_c *position;
	Render_c *renderer;

	diana_getComponent(diana, entity, PositionID, (void **)&position);
	diana_getComponent(diana, entity, RenderID, (void **)&renderer);

	printf("%i rendered at (%f,%f,%c)\n", entity, position->x, position->y, renderer->code);
	displayBuffer[(int)(position->x)][(int)(position->y)].code = '#';// renderer->code;
	displayBuffer[(int)(position->x)][(int)(position->y)].foreColor = 0xFFFF0000;//r->foreColor;
	displayBuffer[(int)(position->x)][(int)(position->y)].backColor = 0xFF000000;//r->backColor;

	terminal_put((int)(position->x), (int)(position->y), displayBuffer[(int)(position->x)][(int)(position->y)].code);
}
