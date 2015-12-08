// firstScreen tests.
#define Fl(N)   (1 << (N))

#define SCOLS 300
#define SROWS 300

enum tileType
{
    NOTHING = 0,
    FLOOR,
    WALL,
    MACHINE,

    NUM_OF_TILETYPES
};

enum cellFlags
{
    DISCOVERED = Fl(0),
    HAS_PLAYER = Fl(1),
    HAS_SHIP   = Fl(2),
}

enum terrainFlags
{
    T_OBSTRUCTS_MOVEMENT    = Fl(0),
    T_OBSTRUCTS_VISION      = Fl(1),
    T_IS_VACUUM             = Fl(2)
}

typedef struct tileEntry
{
    int code;
    char *foreColor;
    char *backColor;
    unsigned long flags;
} tileEntry;

typedef struct displayBuffer
{
        int *code;
        color_t *foreColor;
        color_t *backColor;
        bool needsUpdate;
} displayBuffer;
        
typedef struct pCell {
        enum tileType;
        unsigned long cellFlags;
} pCell;

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

// Different 'types' of creatures, corresponds to alienCatalog.
enum creatureType {
    PLAYER,
    XENOMORPH,
};

enum alienAbilityFlags {
        CAN_LEAP        = Fl(0),
        CAN_TALK        = Fl(1),
};

enum alienBehaviourFlags {
        FRIENDLY        = Fl(0),
        HOSTILE         = Fl(1),
        HUNTER          = Fl(2)
}

enum alienFlags {
        ACTIVE      = Fl(0),
        UNCONCIOUS  = Fl(1),
        SUFFOCATING = Fl(2)
        
        
// Listing of AlienInfo, lets us build aliens from catalog.
typedef struct alienType
{
        enum creatureType creatureID;
        char alienName[30];
        int code;
        char foreColor[15];
        unsigned long abilityFlags;
        unsigned long behaviorFlags;
}

// a living alien somewhere on the map. Can be player.
typedef struct alien {
        alienType info;
        int xLoc;
        int yLoc;
};

typedef struct playerGlobals {
        alien *playerAlien;
        int camX;
        int camY;
        int health;
        int active;
        bool camMoved;
        bool running;
} playerGlobals;

typedef struct messageString {
    int height;
    char *message;

    struct messageString *next;
} messageString;
