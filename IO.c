/*
 *  Wrecker
 *  
 *  IO.c
 *  Where we will handle main input loop,
 *  string handling,
 *  color blending?
 *    
 */
#include <stdio.h>
#include <string.h>

int countLines( FILE *table )
{
        int c, count = 0;
        do {
                c = fgetc(table);
                if ( c == '\n' ) {
                    count ++;
                }
        } while ( c != EOF );

        return count;
}

itemTable buildTableEntry( char *inBuffer )
{
        itemTable newEntry;
        int fields;
        short tmpBool;

        fields = sscanf( buffer, 
                "\"%[^\"]\" %hd %hd %hd %hd {%hd, %hd, %hd} {%hd, %hd, %hd} %hd %hd \"%[^\"]\" ",
                newEntry.name, &newEntry.class, &newEntry.rarity, &newEntry.value, &newEntry.toolSkill,
                &newEntry.required.str, &newEntry.required.dex, &newEntry.required.intel,
                &newEntry.damage.lower, &newEntry.damage.upper, &newEntry.damage.smudge,
                &newEntry.range, &tmpBool, newEntry.description );
        if ( fields < 14 ) {
            printf( "Something went wrong!\n");
            printf("%d fields recorded.\n", fields);
        }
        newEntry.identified = tmpBool;

        return newEntry;
}
// Implement function to see if file exists.
itemTable *buildTable( char *filepath )
{
        itemTable *newTable;
        int i, numEntries;
        char lineBuffer[2000];
        FILE *dataTable;

        if (( dataTable = fopen( filepath, "r") ) == NULL {
            printf("Error opening table file.\n");
            break; // This mkight be wrong? not sure how to handle.
        }

        numEntries = countLines( dataTable ) - 1;
        newTable = malloc( numEntries * sizeof(itemTable) );
        rewind(dataTable);

        fgets( lineBuffer, 2000, dataTable); // Skip first line
        for ( i = 0; i < numEntries; i++ )
        {
            fgets( lineBuffer, 2000, dataTable);
            newTable[i] = buildTableEntry( lineBuffer );
        }
        return newTable;
}

/* Steps to get screen
1. Build display buffer from visible pmap.
    1. Find Visible Pmap
    2. What's in this cell? Creature? Item? Space?
    3. Get cell display Priority (maybe creature is invisible, or
       hidden in fog.
    4. Cook colors that need to be cooked. (apply lighting)
    5. commit to cell's dbuffer
2. Build visible Cells with put
*/

// Might want to cook all of these into static Dbuff, build it up 
// in sequence?

displayBuffer getCellAppearance( short mapX, short mapY )
{
        displayBuffer newCell;
        //Complicated rules for determinining cell display priority

        newCell.needsUpdate = false;
        return newCell;
}

point getVisibleScreen( short camX, short camY)
{
        point vScreen;
        vScreen.x = camX - (DROWS / 2);
        vScreen.y = camY - (DCOLS / 2);
        
        return vScreen;
}

/*
 *  Lets us update whole screen if we move camera; or if camera
 *  remains static it will only update tiles that need updates.
 */
// Separate into two Bool values; maybe sometimes cell appearances
// Don't need updates, but the cell location does.
void updateDisplayBuffer( point vScreen )
{
        int i, j;
        short pMapX = vScreen.x;
        short pMapY = vScreen.y;

        for ( j = 0; j < DCOLS ; j++ ) {
            for ( i = 0; x < DROWS; i++) {
                
                if ( dMap[i][j].needsUpdate ) {
                    dMap[i][j] = getCellAppearance( pMapX + i, pMapY + j);
                }
            }
        }
}

void drawDbuff(void)
{
        int i, j;
        for ( j = 0; j < DCOLS; j++) {
            for ( i = 0; i < DROWS; i++) {
                terminal_color( color_from_argb( dMap[i][j].foreColor ) );
                terminal_bkcolor( color_from_argb( dMap[i][j].backColor ) );
                terminal_put( (SIDE_WIDTH + 1) + i, j + 1, dMap[i][j].character );
            }
        }
}
                
void drawGameScreen(void)
{
        drawMiniMap();  
        drawUI();
        drawDbuff();
}
    
