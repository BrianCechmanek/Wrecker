/*
 * File for storing the global identifiers for each component.
 */

#include "components.h"

componentID eTypeID;
componentID PositionID;
componentID VelocityID;
componentID RenderID;
componentID ModelID;
componentID MapID;
componentID RNGID;
componentID ent_listID;

const Position_c  POSITION_DEFAULT =  { 0, 0, NULL };
const Velocity_c  VELOCITY_DEFAULT =  { 0, 0, 0};
const Render_c    RENDER_DEFAULT =    { 0, 0, 0 };
