#include "systems.h"

#include "wrecker.h"
#include "diana.h"
#include "ECS.h"
#include "components.h"
#include "systems.h"
#include "model.h"

systemID renderSystemId;

int render_createSystem(struct diana *diana)
{
    int error;

    struct systemInfo info = ECS_SYSTEM_INFO_DEFAULT;
    info->name = "render";
    info->process = _process;

    error = ecs_createSystem(diana, info, &renderSystemId);
    if (error != DL_ERROR_NONE)
    {
        return error;
    }

    componentID *components = { PositionID, ModelID };
    error = ecs_watchComponents(renderSystemId, components);
    if (error != DL_ERROR_NONE)
    {
        return error;
    }

    return error;
}

static void _process(struct diana *diana, void *user_data, unsigned int entity, float delta)
{
	Position_c *position;
	Model_c *model;

	diana_getComponent(diana, entity, PositionID, (void **)&position);
	diana_getComponent(diana, entity, ModelID, (void **)&model);

	for (int i = 0; i < model->numberOfCells; i++)
	{
		terminal_put((int)(model->Cells[i].x + position->x), (int)(model->Cells[i].y + position->y), model->Cells[i].characterCode);
	}

	/*
	printf("%i rendered at (%f,%f,%c)\n", entity, position->x, position->y, renderer->code);
	displayBuffer[(int)(position->x)][(int)(position->y)].code = '#';// renderer->code;
	displayBuffer[(int)(position->x)][(int)(position->y)].foreColor = 0xFFFF0000;//r->foreColor;
	displayBuffer[(int)(position->x)][(int)(position->y)].backColor = 0xFF000000;//r->backColor;

	terminal_put((int)(position->x), (int)(position->y), displayBuffer[(int)(position->x)][(int)(position->y)].code);
	*/
}
