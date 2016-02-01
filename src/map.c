/*
 *  Wrecker
 *  map.c
 *  File for map creation functions.
 */

#include "map.h"
#include "dbg.h"
#include "components.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Model_c *loadModel( char *filename );
int _getModelDimensions( FILE *fp, int *height, int *width);

/*
 * Loads .txt file into a Model_c struct.
 * For reading pre-built ships/sections.
 */
// This probably belongs in IO but is only used for map functions right now.
Model_c *loadModel( char *filename )
{
    FILE *openedFile;
    char buffer[120];

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
                while ( fgets(buffer, 120, openedFile) != NULL ){
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
