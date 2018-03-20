/*
 *  Wrecker
 *  map.c
 *  File for map creation functions.
 */

#include "map.h"
#include "diana.h"
#include "dbg.h"
#include "components.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define E_LIST_MAX 100

const int dirList[8][2] = {
//  NORTH    N_EAST   EAST    S_EAST  SOUTH   S_WEST   WEST     N_WEST
    {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}
};

//TODO: Rework into a resizing buffered list.

Model_c *loadModel( char *filename );
int _getModelDimensions( FILE *fp, int *height, int *width);


/*
 * Loads .txt file into a Model_c struct.
 * For reading pre-built ships/sections.
 */
// This probably belongs in IO but is only used for map functions right now.
/*
Model_c *loadModel( char *filename )
{
    FILE *openedFile;
    char buffer[120];
    char lineBuffer[120];

    if ( (openedFile = fopen(filename, "r")) == NULL ) {
        sentinel("Failure to load file: %s", filename);
    }

    Model_c *model = emalloc(sizeof(*model));
    fgets( buffer, 120, openedFile );
    if (strcmp(buffer, "#MODEL") != 0){
            log_info("Error loading model: No #MODEL tag. Model filename: %s", filename);
            return NULL;
    }

    fgets( buffer, 120, openedFile );
    sscanf(lineBuffer, "\"%[^\"]\"", model->name);
    fgets( buffer, 120, openedFile ); // Skip line.
    
    int height = 0; int width = 0;
    if ( _modelGetDimensions( openedFile, &height, &width ) == 0 ){
            sentinel(" Error loading ship model: %s", filename);
    }
    check( height < 500, "ERROR: .txt file %s has too many lines!", filename );
    model->height = height; 
    model->width = width;

    model->map = emalloc(height * sizeof( char *) );
    model->map[0] = emalloc(height * width * sizeof(int));
    for ( int i = 0; i < height; i++){
            model->map[i] = model->map[0] + i * width;
            //memset(model->map[i], '\0', width);
    }

    int j = 0;
    while ( fgets(buffer, 120, openedFile) != NULL ){
            
            strcpy(model->map[j], buffer);
            j++;
    }
    return model;

error:
    return NULL;
}

int _modelGetDimensions( FILE *fp, int *height, int *width)
{
        if (fp != NULL){
                char buffer[120];
                long pos = ftell(fp);
                while ( fgets(buffer, 120, fp) != NULL ){
                    int len = strlen(buffer);
                    if (len > 0 && buffer[len - 1] == '\n'){
                        buffer[len-1] = '\0';
                        len--;
                    }
                    if (len > *width) *width = len;
                    height++;
                }
                fseek(fp, pos, SEEK_SET);
                return 1;
        }
        return 0;
}

//Convert Model to Map.
Map_c *buildMap_model( Model_c *model ){
    
    check(model, "Error loading model.");
    Map_c *newMap = emalloc(sizeof(Map_c));

    newMap->owner = 0;
    newMap->height = model->height; newMap->width = model->width;

    newMap->map = emalloc(newMap->height * sizeof( Tile ));
    newMap->map[0] = emalloc(height * width * sizeof(int));
    for ( int i = 0; i < height; i++){
            newMap->map[i] = newMap->map[0] + i * width;
    }
    for  (int j = 0; j < height; j++){
        bool terminatorFound = false;
        for (int i = 0; i < width; i++){
            if (terminatorFound){
                newMap->map[i][j] = buildTile('\0');
                continue;
            }
            char c = model->map[i][j];
            if( c == '\0' ){
                terminatorFound = true;
            }
            newMap->map[i][j] = buildTile( c );
        }
    }
    return newMap;
error:
    return NULL;
}
*/


ent_list_c entitiesAt( Map_t *map, int xLoc, int yLoc)
{
    ent_list_c list = {0};
    ent_list_c *parentList;
    Position_c *pos;

    check( map->grid, "NULL Map passed to entitiesAt()." );
    check ( xLoc >= 0 && yLoc >=0 && xLoc <= map->width && yLoc <= map->height,
            "Coordinates lookup outside Bounds");

    
    WRECK(getComponent, map->parent, ent_listID, (void **) parentList);
    
    for (int i = 0; i < parentList->num; i++){
        WRECK(getComponent, parentList->ents[i], PositionID, (void **) &pos);
        if( (int) pos->x == xLoc && pos->y == yLoc ){
            check( list.num < E_LIST_MAX, 
                   "Too many entities for entity lookup at X: %d Y: %d", xLoc, yLoc );
            list.ents[list.num++] = parentList->ents[i];
        }
    }
    return list;

error:
    return list;
}

//TODO: Add entity type lookup for error handling
bool addToEntList( entID entity, ent_list_c *list)
{
    check( list, "Null ent_list passed for addToEntList()")
    check(list->num < E_LIST_MAX, "ERROR: Adding to FULL ent_list.");

    list->ents[ list-> num++ ] = entity;
    return true;

error:
    return false;
}

void append_entities(ent_list_c in, ent_list_c *out)
{
    if( !in.num ) return;

    check( in.num + out->num <= 100, "ent_list overflow, too many ents in list.");

    for(int i = 0; i < in.num; i++){
        addToEntList( in.ents[i], out);
    }

    return;

error:
    return;
}

ent_list_c getSurroundingEntities (Map_c *map, int xLoc, int yLoc )
{
    ent_list_c list;
    list.num = 0;

    for (int i = xLoc - 1; i <= xLoc + 1; i++){
        for (int j = yLoc - 1; j <= yLoc + 1; j++){
            // Bounds Check
            if ( i < 0 || j < 0 || i > map->width || j > map->height) {
                continue;
            }
            if( i == xLoc && j == yLoc ) continue; // Skip Occupied Tile (might not need)
            append_entities( entitiesAt(map, i, j ), &list);
        }
    }

    return list;
}

bool map_pathableCell( Map_t *map, int x, int y )
{
    if( map != NULL && x < map->width && y < map->height)
        return map->grid[x][y].walkable;
    else
        return false;
}

