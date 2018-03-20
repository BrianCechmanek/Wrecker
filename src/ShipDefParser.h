#ifndef __ShipDef__H__
#define __ShipDef__H__

//CUSTOM HEADER START
typedef struct RoomList{ char mRoomName[24];    struct RoomList *mNext; } ShipRoomList; typedef struct ShipDefinition{ char mName[60];    unsigned mMaxWeight, mMinWeight;    unsigned mMinCrew, mMaxCrew;    ShipRoomList * mRequiredRoomList;    ShipRoomList * mOptionalRoomList; } ShipDef; static void Init_ShipDef(ShipDef*def) { def->mRequiredRoomList = 0;     def->mOptionalRoomList = 0; }
//CUSTOM HEADER END

void LoadShipDef(char * path, ShipDef *def);
#endif
