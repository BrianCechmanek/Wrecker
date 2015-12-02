/*
 * Wrecker v0.1
 * items.c
 * Functions pertaining to items
 */

#include "wrecker.h"
#include <stdlib.h>

item *initialize_new_item(void)
{
        short i;
        item *new_item;
        new_item = (item *) malloc( sizeof(item) );
        memset(new_item, 0, sizeof(item) );

        new_item->category = 0;
        new_item->kind = 0;
        new_item->damage = 0;
        new_item->armor = 0;
        new_item->ammo = 0;
        new_item->required_stats = NULL;
        new_item->display_character = '#';
        new_item->displayColor = &white;
        new_item->inventory_letter = '\0';
        new_item->description[0] = '\0';
        new_item->quantity = 1;

        return new_item;
}

// Add support for random item generation here .
item *generateItem( short item_category, short item_class, short item_type)
{
        item *new_item = initialize_new_item();
        makeItemInto( new_item, item_category, item_type);
        return new_item;
}
//TODO 
item *makeItemInto( item *new_item, short item_category, short item_class, short item_type)
{
}

item *findFloorItem( short x, short y)
{
    item *listItem;

    for (listItem = floorItems; listItem != NULL && (listItem->xLoc != x || listItem != y);
         listItem = nextItem);
    return listItem;
}

// Place item at specific location, add to floorItems (itemChain);TODO 
item *putItemOnTile( item *newItem, short x, short y)
{
}

// Not too sure how to tackle this one yet. If a sector is filled with
// Zones (which could be a ship/asteroid/station/inside of space worm whatever),
// build it with the appropriate logic.TODO 
void buildItemHeatmap( heatmap *zoneHeatmap, zoneGrid *workGrid, short zoneType)
{
}

// Place item based on heatmap information.TODO 
bool findItemSpawnLoc( heatmap *zoneHeatmap, short *xLoc, short *yLoc)
{
}

// Fill zone with appropriate items. TODO 
void populateZoneItems( zone *workZone )
{
        heatmap newHeatmap;
        buildItemHeatmap( &newHeatmap, workZone->grid, workZone->type);
}

// Returns the most important/item if items are stacked
item *getStackedItemPrio( short x, short y )
{
}

//Turn this into a macro.
short freeInventorySpace( inventory *workInv ) {
        return workInv->maxCapacity - workInv->filledSlots;
}

bool itemKindInInventory( item *theItem, inventory *theInv)
{
        item *tmp;

        for ( tmp = theInv->itemList; tmp; tmp->nextItem) {
                if ( tmp->category == theItem->category && tmp->kind == theItem->kind) {
                        return true;
                }
        }
        return false;
}

bool addItemToInventory( item *newItem, inventory *workInv )
{
}

void alienGetsItem(creature *alien, item *newItem)
{
}


        
// Combine stacks of items (ammo, food whatever)
// Run logic to check if items are STACKABLE and same kind in parent.
void stackItems( item *itemA, item *itemB )
{
        itemA->quantity += itemB->quantity;
        deleteItem(itemB);
}

// Checks if creature can Wield item. Applies to player. TODO
// Reference against creatures Stats/traits/skills?
bool canWield( item *theItem, creature *theAlien )
{
}

// Item chain can be any item linked list (aka inventory, floorItems, shop w/e)
void *addItemToChain( item *newItem, item **itemChain )
{
        newItem->nextItem = *itemChain;
        *itemChain = newItem;
}

// For whenever we need to remove an item from a particular chain, maintain chain order.
bool removeItem( item *theItem, item **itemChain )
{
        
        item *tmp;
        for (tmp = *itemChain; tmp->nextItem; tmp = tmp->nextItem){
                if (tmp->nextItem == theItem) {
                        tmp->nextItem = theItem->nextItem;
                        return true;
                }
        }
        return false;
}
void deleteItem( *victimItem )
{
        free(victimItem);
}
// Cleanup function
void freeItemChain( item **itemChain )
{
        item *victim;
        while ( *itemChain != NULL ) {
                victim = *itemChain;
                *itemChain = (*itemChain)->nextItem;
                free(victim);
        }
}


