// stats.h
#pragma once

#include "wrecker.h"

//ENUMS
enum stats 
{
    STRENGTH, 
    DEXTERITY,
    INTELLIGENCE,
    WISDOM,
    CHARISMA,
    CONSTITUTION
};

//STRUCTS
typedef struct stats_c 
{
    int str;
    int dex;
    int intel;
    int wis;
    int charisma;
    int constitution;    
} stats_c;

typedef struct stats_c_max
{
    int strMax;
    int dexMax;
    int intelMax;
    int wisMax;
    int charismaMax;
    int constitutionMax;    
} stats_c;


//Functions
// allows <0 stat values, but not more than statsMax
void changeAttrib(struct stats_c *theStats, struct stats_c_max *maxStats, int stat, int changeAmount);
// restores one attribute to the component's maxStat value
void restoreAttrib(struct stats_c *theStats, struct stats_c_max *maxStats, int stat);
// changes one attribute component maxStat value (increases or
// decreases like "+7 dex space gloves". 
void changeAttribMax(struct stats_c_max *maxStats, int stat, int changeAmount);

// Declare Globals.
extern componentID statsID;
