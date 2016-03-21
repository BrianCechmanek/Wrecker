/* WRECKER 
 * stats.c
 * stats will be a component of all creatures
 * Extension of stats to tiles (walls, barrels, et c)
 * is still in question
 */

// REMEMBER TO UPDATE COMPONENTS.C/H
// AS WELL AS ECS.c

#include <stdio.h>   // only for printing errors. remove? 
#include "stats.h"
#include "wrecker.h"

// make ECS unique identifier
componentID StatsID;

//Functions
void changeAttrib(struct stats_c *theStats, struct stats_c_max *maxStatsint stat, int changeAmount) 
{
    // increments and decrements. 
    // allows <0
    switch(stat)
    {
        case STRENGTH:
            if ((theStats->str += changeAmount) > maxStats->str) 
            {
            theStats->str = maxStats->str;      
            }
            else 
            {
            theStats->str += changeAmount;
            }
            break;
        case DEXTERITY:
            if ((theStats->dex += changeAmount) > maxStats->dex) 
            {
            theStats->dex = maxStats->dex;      
            }
            else 
            {
            theStats->dex += changeAmount;
            }
            break;
        case INTELLIGENCE:
            if ((theStats->intel += changeAmount) > maxStats->intel) 
            {
            theStats->intel = maxStats->intel;      
            }
            else 
            {
            theStats->intel += changeAmount;
            }
            break;
        case WISDOM:
            if ((theStats->wis += changeAmount) > maxStats->wis) 
            {
            theStats->wis = maxStats->wis;      
            }
            else 
            {
            theStats->wis += changeAmount;
            }
            break;
        case CHARISMA:
            if ((theStats->charisma += changeAmount) > maxStats->charisma) 
            {
            theStats->charisma = maxStats->charisma;      
            }
            else 
            {
            theStats->charisma += changeAmount;
            }
            break;
        case CONSTITUTION:
            if ((theStats->constitution += changeAmount) > maxStats->constitution) 
            {
            theStats->constitution = maxStats->constitution;      
            }
            else 
            {
            theStats->constitution += changeAmount;
            }
            break;
        default:
            // not sure how to catch 'exceptions' 
            printf("Invalid stat parameter to change");
    }
}

void restoreAttrib(struct stats_c *theStats,  struct stats_c_max *maxStats, int stat)
{   
    switch (stat)
    {
        case STRENGTH:
            theStats->str = maxStats->strMax;
            break;
        case DEXTERITY:     
            theStats->dex = maxStats->dexMax;
            break;
        case INTELLIGENCE:
            theStats->intel = maxStats->intelMax;
            break;
        case WISDOM:
            theStats->wis = maxStats->wisMax;
            break;
        case CHARISMA:
            theStats->charisma = maxStats->charismaMax;
             break;
        case CONSTITUTION:
            theStats->constitution = maxStats->constitutionMax;
            break;
        default:
            //still not sure how to catch exceptions...
            printf("Some error in restoreAttrib()");
    }
}

void changeAttribMax(struct stats_c_max *maxStats, int stat, int changeAmount)
/* this function increases max stat by a given amount, it does 
 * NOT make it directly a new amount. My resoning being it's 
 * more likely that we'll get a +1 dex space-gloves, and not
 * "15 score" space gloves
*/
{
    switch (stat)
    {
        case STRENGTH:
            maxStats->strMax += changeAmount;
            break;
        case DEXTERITY:     
            maxStats->dexMax += changeAmount;
            break;
        case INTELLIGENCE:
            maxStats->intelMax += changeAmount;
            break;
        case WISDOM:
            maxStats->wisMax += changeAmount;
            break;
        case CHARISMA:
            maxStats->charismaMax += changeAmount;;
             break;
        case CONSTITUTION:
            maxStats->consitituionMax += changeAmount;
            break;
        default:
            //still not sure how to catch exceptions...
            printf("Some error in changeAttribMax()");
    }    
}
