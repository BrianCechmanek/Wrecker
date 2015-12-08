/*
Simple Input Functions for firstScreen proto
*/

void getCellAppearance( int x, int y, int *code, color_t *retFore, color_t *retBack)
{
    int newCode;
    color_t newFore, newBack;
    creature *alien = NULL;
    //item *tileItem = NULL;
    ship *theShip = NULL;

    int tileType newTileType;

    if ( sectorMap[x][y].flags & HAS_PLAYER ) {
        alien = findPlayer();
    }
    if (sectorMap[x][y].flags & HAS_SHIP ) {
        newTileType = sectorMap[x][y].tileType;
        newBack = color_from_name(tileList[newTileType].backColor);
        if (!alien) {
            newCode = tileList[newTileType].code;
            newFore = color_from_name(tileList[newTileType].foreColor);
        } else {
            newCode = alien->code;
            newFore = color_from_name(alien->foreColor);
        }
    } else {
            newTileType = NOTHING;
            newBack = color_from_name("black");
            if (alien) {
                newFore = color_from_name(alien->foreColor);
                newCode = alien->code;
        }
        else {
            newFore = color_from_name("black");
            newCode = ' ';
        }
    }

    code = newCode;
    retFore = newFore;
    retBack = newBack;
} 

void updateDisplayBuffer(int camX, int camY)
{
    int i, j;
    for ( j = 0; j < DCOLS; j++) {
        for ( i = 0; i DROWS; i++){
            if( dMap[i][j].needsUpdate ) {
                getCellAppearance(camX+i, camY+j, dMap[i][j].code, dMap[i][j].foreColor, 
                                                dMap[i][j].backColor);
            }
        }
    }
}

void announce( char *string )
{
        
void handleInput(void)
{
    int code;
    do {

        code = terminal_read();
        if (code == TK_ESCAPE || code == TK_CLOSE || code == TK_Q) {
            running = 0;
        }
        else if (code == TK_TAB) {
            !gameInfo.cameraMode;
        }
        else if (code == TK_K || code == TK_UP || code == TK_KP_8) {
            if ( gameInfo.cameraMode ) {
                if (moveCamera( NORTH ) == false){
                    announce("Camera Movement Blocked");
                }   
            } else {
                if(movePlayer( NORTH ) == false ){
                    announce("Player Movement Blocked");
                }
            }
        }
        else if (code == TK_U || code == TK_KP_9) {
            if ( gameInfo.cameraMode ) {
                if (moveCamera(N_EAST) == false){
                    announce("Camera Movement Blocked");
                }   
            } else {
                if(movePlayer(N_EAST ) == false ){
                    announce("Player Movement Blocked");
                }
            }
        }

       else if (code == TK_L || code == TK_RIGHT || code == TK_KP_6) {
            if ( gameInfo.cameraMode ) {
                if (moveCamera(EAST) == false){
                    announce("Camera Movement Blocked");
                }   
            } else {
                if(movePlayer(EAST) == false ){
                    announce("Player Movement Blocked");
                }
            }
        }
        else if (code == TK_N || code == TK_KP_3) {
            if ( gameInfo.cameraMode ) {
                if (moveCamera( S_EAST ) == false){
                    announce("Camera Movement Blocked");
                }   
            } else {
                if(movePlayer( S_EAST ) == false ){
                    announce("Player Movement Blocked");
                }
            }
        }
        else if (code == TK_J || code == TK_DOWN || code == TK_KP_2) {
            if ( gameInfo.cameraMode ) {
                if (moveCamera(SOUTH) == false){
                    announce("Camera Movement Blocked");
                }   
            } else {
                if(movePlayer(SOUTH) == false ){
                    announce("Player Movement Blocked");
                }
            }
        }
        else if (code == TK_B || code == TK_KP_1) {
            if ( gameInfo.cameraMode ) {
                if (moveCamera(S_WEST) == false){
                    announce("Camera Movement Blocked");
                }   
            } else {
                if(movePlayer(S_WEST) == false ){
                    announce("Player Movement Blocked");
                }
            }
        }
        else if (code == TK_H || code == TK_LEFT || code == TK_KP_4) {
            if ( gameInfo.cameraMode ) {
                if (moveCamera(WEST) == false){
                    announce("Camera Movement Blocked");
                }   
            } else {
                if(movePlayer(WEST) == false ){
                    announce("Player Movement Blocked");
                }
            }
        }
        else if (code == TK_Y || code == TK_KP_7) {
            if ( gameInfo.cameraMode ) {
                if (moveCamera(N_WEST) == false){
                    announce("Camera Movement Blocked");
                }   
            } else {
                if(movePlayer(N_WEST) == false ){
                    announce("Player Movement Blocked");
                }
            }
        }
    } while (gameInfo.running && terminal_has_input());
}
