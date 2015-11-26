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


