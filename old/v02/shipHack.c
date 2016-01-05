/*
    Ship Test 4
    character movement; screen limits.
 */    

#include "ships.h"

// In-File Prototypes
void drawBorder(void);

playerGlobals playerInfo;
Cell *map;
alien *alienChain;
Ship *shipChain;

tileEntry tileList[4] = {
  //Char    Flags
    {' ',   (T_IS_VACUUM) },
    {'.',   0},
    {'#',   (T_OBSTRUCTS_MOVEMENT | T_OBSTRUCTS_VISION)},
    {'C',   (T_OBSTRUCTS_MOVEMENT)}
};

machineEntry machineList[1] = {
    //char
    {'C',   "Ship Controls", STARTS_OFF, togglePilot }
};

int dirList[8][2] = {{0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}};

alienEntry alienList[2] = {
    //code      //Name
    {'@',       "Player"},
    {'e',       "Space Elf"}
};
//
int main(void)
{

    char *filename = "./ships/Uboat.txt";
    int i, j;
    Ship theShip;
    CameraInfo theCamera;

    terminal_open();
    terminal_set("window.title='Ship + Camera Prototype'");
    terminal_set("font: ./fonts/PixelCarnageMono.ttf, size=12");
    
    srand(time(NULL));
    initMap();
    initCamera( &theCamera );
    loadShip(&theShip, filename);
    placeShip(&theShip, 110, 110);

    alien *thePlayer;
    thePlayer = initPlayer( &theCamera );
    placeAlien( thePlayer, 112, 112);
    genShipAliens( 3, SPACE_ELF, &theShip);
    lockCamera( &theCamera);
    drawBorder();

    terminal_printf(0,0, "%s", theShip.name);

    int key;
    int running = 1;
    while (running) {

        drawMap(&theCamera);
        terminal_refresh();

        while ( running && terminal_has_input() ){
            key = terminal_read();
            
            if ( key == TK_CLOSE || key == TK_ESCAPE ) running = 0;
            else if (key == TK_SPACE){
                moveCameraToShip(&theCamera, &theShip);
            }
            if ( key == TK_TAB ) {
                theCamera.scrollMode = !theCamera.scrollMode;
                theCamera.hSpeed = 0;
                theCamera.vSpeed = 0;
            }
            if (!theCamera.scrollMode){
                if (key == TK_SPACE) lockCamera( &theCamera );
                if (key == TK_UP) movePlayer(NORTH);
                if (key == TK_DOWN) movePlayer(SOUTH);
                if (key == TK_LEFT) movePlayer(WEST);
                if (key == TK_RIGHT) movePlayer(EAST);
                if (key == TK_E) alienUse( playerInfo.playerAlien);
            }
        }
       
        if (theCamera.scrollMode){
            if ( terminal_state( TK_LEFT )) {
                if (theCamera.hSpeed > -SPEEDCAP && theCamera.hpo > 0 ) {
                        theCamera.hSpeed -= 1;
                    } else {
                        if (theCamera.hpo <= 0) theCamera.hSpeed = 0;
                    }
                }
                else if ( terminal_state( TK_RIGHT )) {
                    if (theCamera.hSpeed < SPEEDCAP && theCamera.hpo / theCamera.tileWidth < MAPWIDTH - CAM_W) {
                        theCamera.hSpeed += 1;
                    }
                    if (theCamera.hpo / theCamera.tileWidth >= MAPWIDTH - CAM_W) theCamera.hSpeed = 0;
                } else {
                    if (theCamera.hpo > 0 || theCamera.hpo / theCamera.tileWidth < MAPWIDTH - CAM_W ) {
                        theCamera.hSpeed -= ( theCamera.hSpeed > 0) ? 1: ( ( theCamera.hSpeed < 0) ? -1: 0);
                } else {
                        theCamera.hSpeed = 0;
                }
                }
                // Vert
            if (terminal_state( TK_UP )) {
                    if (theCamera.vSpeed > -SPEEDCAP && theCamera.vpo > 0 ) {
                        theCamera.vSpeed -= 1;
                    }
                    if (theCamera.vpo <= 0) theCamera.vSpeed = 0;
                }
            else if ( terminal_state( TK_DOWN )) {
                    if (theCamera.vSpeed < SPEEDCAP && theCamera.vpo / theCamera.tileHeight < MAPHEIGHT - CAM_H ) {
                        theCamera.vSpeed += 1;
                    }
                    if (theCamera.vpo / theCamera.tileHeight >= MAPHEIGHT - CAM_H ) theCamera.vSpeed = 0;
            } else {
                    if (theCamera.vpo > 0 || theCamera.vpo / theCamera.tileHeight < MAPHEIGHT - CAM_H ) {
                        theCamera.vSpeed -= ( theCamera.vSpeed > 0) ? 1: ( ( theCamera.vSpeed < 0) ? -1: 0);
                    } else {
                        theCamera.vSpeed = 0;
                    }
            }
        }
        terminal_printf(0,1, "%d %d  ", theShip.shipModel[0].xLoc, theShip.shipModel[0].yLoc);
        terminal_printf(0,3, "iX =%d  iY=%d  ", theCamera.camX, theCamera.camY);
        terminal_printf(0,4, "hSpeed=%d ", theCamera.hSpeed);
        terminal_printf(0,5, "vSpeed=%d ", theCamera.vSpeed);

        terminal_delay(1000 / FPS);
    }
     
    terminal_close();
    free(theShip.shipModel);
    free(map);
    freeAliens();
}

void drawBorder(void)
{
    int i, j;

    for ( i = VIEW_xOFF - 2; i <= VIEW_xOFF + CAM_W + 6; i++ ) {
        terminal_put(i, 1, '#');
        terminal_put(i, 24, '#'); 
    }

    for ( j = 2; j < 24; j++) {
        terminal_put(VIEW_xOFF - 2, j, '#');
        terminal_put(VIEW_xOFF + CAM_W + 6, j, '#');
    }
}
