/*
 *  Wrecker
 *  v.001
 *  David Bergeron & Brian Cechmanek
 *  wrecker.h
 *  Location where we will define structure types and enums,
 *  essentially listings of how we want to organize objects
 *  and entities in the game. Very much a WIP, eventually will 
 *  contain function prototypes as well.
 */

#include <stdio.h>
#include <stdbool.h>
/*
 *   TO DO:
 *   
 *   EVERYTHING.
 *   short term: tile types, itemTable/Catalog? No item flags.

 *   I reference struct color_t, something called stats_r,
 *   not too sure how we should tackle those. Still this template
 *   should give you an idea of how we can organize entities inside
 *   the game.

 *   character_sheet struct?
 *   required_stats object?
 *   Turn-system still half baked: Right now I'm going with some
 *   sort of 'energy' based system, but not sold on it yet.

 *   All inventory setup is still pretty preliminary, but
 *   easy to change at this point.
 */


#define COLS        100
#define ROWS        35

#define DCOLS       (COLS - SIDE_BAR - 1)
#define DROWS       (ROWS - BOT_BAR - 2)

#define SIDE_BAR    20
#define BOT_BAR     3
// Marks enum as a Flag, allowing you to combine them. Bitwise hack.
#define Fl(N)           (1 << (N))

// Haven't thought this one through too much, but in theory we want
// To blend a variety of colors, and only want to update cells that
// have been affected by changes.
typedef struct cellDisplayBuffer
{
        char character;
        char fore_colors[3];
        char back_colors[3];
        char opacity;
        bool needs_update;
} cellDisplayBuffer;

typedef struct pcell
{
        enum tile_type;
        cellDisplayBuffer appearance;
} pcell;

typedef struct randomRange
{
        short lower;
        short upper;
        short smudge;
};

// We probably want to populate item information from some sort of
// catalog. Will show example in creature vs creatureClass/type..

enum itemCategory
{
        FOOD,
        TOOL,
        WEAPON,
        ARMOR,
        KEYS,
        MISC,
        
        NUMBER_ITEM_CATEGORIES
};

typedef struct stats_t
{
    short str;
    short dex;
    short intel;
};

enum attributes
{
    STRENGTH,
    DEXTERITY,
    INTELLIGENCE
};

typedef struct itemClassTable
{
        char name[30];
        short category;
        short classID;
        unsigned long flags; // Modifiers applied to all weapons of class?
        char description[500];
};

typedef struct itemTable {
	char name[30];
    short classID;
	short rarity;
	short marketValue;
    short toolSkill;
    stats_t requiredStats;
	randomRange damage;
    short range;
    bool identified;
    char description[1000];
} itemTable;

typedef struct item
{
        short category;
        short itemClassID; 
        short kind;
        short damage;
        short range;
        short armor;
        short ammo;
        short quantity;
        stats_t requiredStats;
        char display_character;
        color_t *foreColor;
        color_t *backColor;
        char inventory_letter;
        char *description;

        short xLoc;
        short yLoc;

        bool identified;
        
        struct item *nextItem;
} item;

// Really simple implementation for now, can refactor later.
typedef struct inventory
{
        unsigned short max_capacity;
        struct item *itemList;
} inventory;

enum alienTypes
{
    AT_PLAYER,

    AT_HUMAN,
    AT_HUMAN_ZOMBIE,
    AT_HUMAN_MERCHANT,

    AT_XENOMORPH,
    AT_XENOMORPH_EGG,
    AT_XENOMORPH_FACEHUGGER,
    AT_XENOMORPH_QUEEN,

    NUMBER_ALIEN_TYPES
};

enum statusEffects
{
        STATUS_WEAKENED,
        STATUS_SLOWED,
        STATUS_BERSERK,
        STATUS_BLIND,
        STATUS_STUCK,
        STATUS_INVISIBLE,
        STATUS_TERRIFIED,
        STATUS_BURNING,

        NUMBER_STATUS_EFFECTS
};

enum creatureState
{
        STATE_SLEEPING,
        STATE_TRACKING,
        STATE_FLEEING,
        STATE_ROAMING,
};

// Can combine flags, see turret example.              
enum creatureBehaviourFlags
{
        C_INVISIBLE             = Fl(0),
        C_FLYING                = Fl(1),
        C_IMMUNE_TO_FIRE        = Fl(2),
        C_ALWAYS_TRACKING       = Fl(3),
        C_FLEES_NEAR_DEATH      = Fl(4),
        C_INVULNERABLE          = Fl(5),
        C_IMMOBILE              = Fl(6),
        C_INANIMATE             = Fl(7),
        C_EATS_CORPSES          = Fl(8),
        C_SURVIVE_VACUUM        = Fl(9),

        C_TURRET                = (C_INANIMATE | C_IMMOBILE | C_ALWAYS_TRACKING)
};

enum creatureAbilityFlags
{
       CA_ACID_BLOOD            = Fl(0),
       CA_USES_VENTS            = Fl(1),
       CA_USE_SIMPLE_MACHINES   = Fl(2),
       CA_BASHES_DOORS          = Fl(3),
       CA_HUGS_FACES            = Fl(4),
       CA_THERMOVISION          = Fl(5),
       CA_ATTACKS_PENETRATE     = Fl(6),
       CA_ATTACKS_POISON        = Fl(7),
       CA_ATTACKS_IGNITE        = Fl(8),
};

enum creatureBookkeepingFlags
{
        CB_WAS_VISIBLE          = Fl(0),
        CB_IS_BLEEDING          = Fl(1),
        CB_IS_LEADER            = Fl(2),
        CB_IS_FOLLOWER          = Fl(3),
        CB_CAPTIVE              = Fl(4),
        CB_SEIZED               = Fl(5),
        CB_IS_DYING             = Fl(6),
        CB_IS_SUFFOCATING       = Fl(7),
};
        
typedef struct creatureType
{
        enum alienTypes alienID;
        char monster_name[30];
        char display_char;
        color_t display_color;
        short startHP;
        short base_armor;
        stats_t base_stats;
        short base_damage;
        short base_speed;               // Faster creatures regen energy p faster?
        short base_vision;
        short base_energy;
        struct inventory *base_inventory;
        unsigned long ability_flags;
};
        
// memberList contains all grouped alienTypes, for instance all XENOMORPHS.
typedef struct creatureClass
{
        char name[30];
        short frequency;
        short minDepth;                 // Depth in this context is # of sectors visited.
        enum alienTypes memberList[15];
};

// Information about specific creature, usually contained in linked list.
typedef struct creature
{
        creatureType info;
        short xLoc;
        short yLoc;
        short sector;
        short spawnSector;
        short currentHP;
        short currentArmor;
        short currentEnergy;
        short energyRegen;              // Per turn
        short oxygenAmount;
        short nutritionLevel;
        stats_t creatureStats;

        struct item *weapon;
        struct item *armor;
        struct inventory *creatureInventory;

        enum creatureState currentState;
        short status[NUMBER_OF_STATUS_EFFECTS];

        unsigned long behaviorFlags;
        unsigned long bookkeepingFlags;
        struct creature *leader;        // If creature is follower, points to leader.
        struct creature *carriedCreature; //Used if Creature is holding someone else.

        // Maybe include some map info here? Map to player, map to safety etc.
        // short **mapToPlayer;
        // short **mapToSafety;

        struct creature *nextCreature;

        bool needs_update;
};

// Global variables pertaining to game state and player's unique variables.
// (Otherwise player character is just another creatureType)
typedef struct playerCharacter 
{
        short sector_number;             // What sector are we in
        short sectors_visited;           // Total sectors visited
        bool game_has_ended;
        bool just_rested;
        bool jedi_mode;                 // Invincibility (debug purpose)
        bool in_vacuum;                 // Is character in space;
        unsigned long seed;             // RNG seed for EVERYTHING
        unsigned long credits;          // How many credits we have.
      //struct StatSheet *stats;
        struct SkillSheet *skills;

        item *weapon;
        item *armor;

        unsigned long turn_number;

        //short **mapToAir;               // Used to path towards known air source.

        // Info for recording? (replays might help debug)
        bool replayMode;
        unsigned long current_turn_number;
        unsigned long total_turn_number;  // How many turns in replay/
        char replay_percentage;
} playerCharacter;
        
typedef struct sectorData
{
        bool visited;
        pcell mapStorage[COLS][ROWS];
        struct item *items;
        struct creature *aliens;
        struct creature *dormant_aliens;
        unsigned int sector_seed;
        unsigned int away_since;
} sectorData;
