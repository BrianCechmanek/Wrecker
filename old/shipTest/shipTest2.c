#include <stdio.h>
#include <stdlib.h>

typedef struct shipTile
{
    int tileType;
    int xLoc, yLoc;
} shipTile;

typedef struct Ship
{
        char name[20];
        shipTile *shipModel;
        int position;
        short dx, dy; //speed
        short face; // 0 up, right 1, down 2, left 3.
} Ship;

enum tileType
{
    NOTHING = 0,
    FLOOR,
    WALL,
    MACHINE,

    NUM_OF_TILETYPES
};

char tileList[4] = " .#C";

int getTileType(char c)
{
    if ( c == '.' ) return 1;
    if ( c == '#' ) return 2;
    if ( c == 'C' ) return 3;
    else
        return 0;
}

void loadShip( Ship *newShip, char *filename )
{
    FILE *openedFile;
    char lineBuffer[60];

    int i = 0; int j = 0;
    int w, h;
    char c;
    
    printf("Tile Size: %d\n", sizeof(shipTile));
    if ( (openedFile = fopen(filename, "r")) == NULL ) {
        printf("Error opening File: %s\n", filename);
        exit(1);
    }
    
    fgets( lineBuffer, 60, openedFile );
    sscanf(lineBuffer, "\"%[^\"]\"", newShip->name);
    fgets( lineBuffer, 60, openedFile);
    sscanf(lineBuffer, "%d %d", &w, &h);
    printf("Expected ship size: %d\n", (sizeof(shipTile) * h * w));

    newShip->shipModel = malloc(sizeof(shipTile) * w * h);

    fgets(lineBuffer, 60, openedFile); // Skip Line
   
    for (j = 0; j < h; j++){ 
        fgets(lineBuffer, 60, openedFile);
        for ( i = 0; i < w; i++ ){
            c = lineBuffer[i];
            newShip->shipModel[j * w + i].tileType = getTileType(c);
            newShip->shipModel[j * w + i].xLoc = i;
            newShip->shipModel[j * w + i].yLoc = j;
        }
    }
    fclose(openedFile);
}

int main(void)
{
    char *filename = "Uboat.txt";
    int i, j;
    Ship theShip;

    printf("\n***************\nshipViewer 0.1\n***************\n\n");

    loadShip( &theShip, filename);
    printf("%s \n\n", theShip.name);
    
    for ( j = 0; j < 9; j++){
        for (i = 0; i < 15; i++) {
            printf("%c", tileList[theShip.shipModel[j * 15 + i].tileType]);
        }
        printf("\n");
    }
    printf("\n");
    free(theShip.shipModel);
    return 0;
}
