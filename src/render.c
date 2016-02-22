#include "systems.h"

#include "wrecker.h"
#include "diana.h"
#include "ECS.h"
#include "components.h"
#include "systems.h"
#include "model.h"

systemID renderSystemId;


static void _process(struct diana *diana, void *user_data, unsigned int entity, float delta)
{
	Position_c *position;
	Model_c *model;

	diana_getComponent(diana, entity, PositionID, (void **)&position);
	diana_getComponent(diana, entity, ModelID, (void **)&model);

	for (int i = 0; i < model->numberOfCells; i++)
	{
		displayBuffer[(int)(model->Cells[i].y + position->y)][(int)(model->Cells[i].x + position->x)].code = model->Cells[i].characterCode;// renderer->code;
		displayBuffer[(int)(model->Cells[i].y + position->y)][(int)(model->Cells[i].x + position->x)].foreColor = model->Cells[i].fgColor;//r->foreColor;
		displayBuffer[(int)(model->Cells[i].y + position->y)][(int)(model->Cells[i].x + position->x)].backColor = model->Cells[i].bgColor;//r->backColor;
		//terminal_put((int)(model->Cells[i].x + position->x), (int)(model->Cells[i].y + position->y), model->Cells[i].characterCode);
	}


	//terminal_put((int)(position->x), (int)(position->y), displayBuffer[(int)(position->y)][(int)(position->x)].code);
}


int render_createSystem(struct diana *diana)
{
    int error;

    struct systemInfo info = ECS_SYSTEM_INFO_DEFAULT;
    info.name = "render";
    info.process = _process;

    error = ecs_createSystem(diana, &info, &renderSystemId);
    if (error != DL_ERROR_NONE)
    {
        return error;
    }

	componentID components[2] = { PositionID, ModelID };
    error = ecs_watchComponents(diana, renderSystemId, 2, components);
    if (error != DL_ERROR_NONE)
    {
        return error;
    }

    return error;
}
