/*
 * Wrecker v0.1
 * items.c
 * Functions pertaining to items
 */

#include "wrecker.h"
#include <stdlib.h>

item initialize_new_item(void)
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

item *makeItemInto( item *new_item, short item_category, short item_class, short item_type)
{
}


        


        
