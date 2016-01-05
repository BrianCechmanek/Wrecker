#ifndef SHIPS_H
#define SHIPS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include "BearLibTerminal.h"

#define CELL(a, b) (map[(b*MAPWIDTH) + a])
#define Fl(N)   (1 << (N))

#define MAPWIDTH 200
#define MAPHEIGHT 200

#define VIEW_xOFF 20
#define VIEW_yOFF 2 

#define FPS 60 
#define SPEEDCAP 10

#define CAM_W 40
#define CAM_H 20

typedef struct{
    
    int camX; int camY;
    int hpo; int vpo;       // Horizontal and Vertical Pixel Offset
    int hSpeed; int vSpeed;
    int tileWidth; int tileHeight;
    bool scrollMode;

} CameraInfo;

// N = 0, E = 2, S = 4, W = 6.
enum directions
{
        NORTH,
        N_EAST,
        EAST,
        S_EAST,
        SOUTH,
        S_WEST,
        WEST,
        N_WEST
};

enum entityTypes{
    ALIEN_E = 1,
    ITEM_E,
    SHIP_E,
    MACH_E
};
    
enum tiles
{
    NOTHING = 0,
    FLOOR,
    WALL,
    MACHINE,

    NUM_OF_TILETYPES
};

typedef struct tileEntry
{
    int code;
    unsigned long terrainFlags;
} tileEntry;

typedef struct{
    int tileType;
    unsigned long cellFlags;
} Cell;

enum shipTileFlags {
    HAS_MACHINE = Fl(0),
};

typedef struct shipTile
{
    int tileType;
    int flags;
    int xLoc, yLoc;
} shipTile;

enum cellFlagTypes
{
    DISCOVERED = Fl(0),  // UNUSED
    HAS_PLAYER = Fl(1),
    HAS_ALIEN  = Fl(2),
    HAS_SHIP   = Fl(3),
};

enum terrainFlagTypes
{
    T_OBSTRUCTS_MOVEMENT    = Fl(0),
    T_OBSTRUCTS_VISION      = Fl(1),
    T_IS_VACUUM             = Fl(2) // UNUSED
};

enum machineType
{
    S_CONTROLS
};

enum machineInitStates
{
    STARTS_OFF,
    STARTS_ON,
};

struct Ship;

typedef struct Machine
{
    int type;
    int xLoc;
    int yLoc;
    struct Ship *parentShip;

    bool active;

    struct Machine *next;
} Machine;

typedef struct machineEntry
{
    int code;
    char name[20];
    int startCode;
    void (*fn)();
} machineEntry;

typedef struct Ship
{
        char name[20];
        int width, height;
        shipTile *shipModel;
        int xLoc, yLoc;
        Machine *shipMachines;
        short dx, dy; //speed
        short face; // 0 up, right 1, down 2, left 3.

        struct Ship *next;
} Ship;


enum alienType
{
    PLAYER,
    SPACE_ELF
};

typedef struct alienEntry
{
        int code;
        char alienName[30];
} alienEntry;

// a living alien somewhere on the map. Can be player.
typedef struct alien {
        int alienType;
        int xLoc;
        int yLoc;

        struct alien *next;
} alien;

typedef struct Entity {
    void *object;
    int type;
} Entity;

typedef struct Stack {
    Entity contents[500];
    int size;
} Stack;

typedef struct playerGlobals {
        alien *playerAlien;
        Ship *pilot;
        CameraInfo *camera;
        bool selectMode;
} playerGlobals;

        
// ********* GLOBAL DECLARATIONS **********
extern playerGlobals playerInfo;
extern Cell *map;
extern alien *alienChain;
extern Ship *shipChain;
extern tileEntry tileList[4];
extern machineEntry machineList[1];
extern int dirList[8][2];
extern alienEntry alienList[2];


// ********* MAP FUNCTIONS *****************
void initMap(void);
int getTileType(char c);
void initCamera(CameraInfo *camera);
void lockCamera(CameraInfo *theCamera);
void moveCameraToShip(CameraInfo *camera, Ship *theShip);
void drawMap( CameraInfo *cam);

// ********* SHIP FUNCTIONS *****************
void loadShip( Ship *newShip, char *filename );
void placeShip( Ship *theShip, int x, int y);
Ship *findShipAt( int x, int y); 
void clearShip( Ship *theShip );
int getFreeShipTile( Ship *theShip );
bool movePlayer(int direction);
bool moveShip( Ship *theShip, int direction );

// ********** MACHINE FUNCTIONS ***********
int getMachineType( char c);
Machine *buildMachine( char c);
Machine *getMachineAt( int x, int y);
bool moveMachine( Machine *theMachine, int direction);
void togglePilot( Machine *theMachine, alien *theAlien);

// ********** ALIEN FUNCTIONS ***************
alien *buildAlien( int typeNum );
alien *initPlayer(CameraInfo *theCamera);
bool placeAlien( alien *theAlien, int x, int y);
bool genShipAliens( int num, int alienType, Ship *theShip );
alien *findAlien(int x, int y);
bool alienUse( alien *theAlien);
bool moveAlien( alien *theAlien, int direction, bool forceMove);
void freeAliens(void);

// ********** DATA STRUCTURES *************
Stack newStack(void);
bool stackPush( void *ent, int type, Stack *theStack);
Entity *stackPop( Stack *theStack );

#endif // SHIPS_H
