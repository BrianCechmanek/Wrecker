/*
Simple Input Functions for firstScreen proto
*/

void initCamera(int x, int y)
{
    camera.tileWidth = terminal_state(TK_CELL_WIDTH);
    camera.tileHeight = terminal_state(TK_CELL_WIDTH);
    camera.camX = x; camera.camY = y;
    camera.hpo = x * terminalWidth; 
    camera.vpo = y * terminalHeight;
    camera.scrollMode = 0;
}

void centerCameraOnPlayer(void)
{
    camera.camX = gameInfo->player->xLoc;
    camera.camY = gameInfo->player->yLoc;
    camera.hpo = camera.camX * camera.tileWidth;
    camera.vpo = camera.camY * camera.tileHeight;
    camera.hSpeed = 0;
    camera.vSpeed = 0;
}

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

void scrollCamera(void)
{
    int viewWidth = CAMW * camera.tileWidth;
    int viewHeight = CAMH * camera.tileHeight;
    int xBord = SCOLS - CAMW; int yBord = SROWS - CAMH;
    
    int tx, ty;         //Remainder of Offset / Tilesize
    int jx, jy;         // Map Looping info.
    int hc, vc;
    int mapX, mapY;
    int xPixel, yPixel;
    displayBuffer *tmpDcell;

    if ( camera.hpo < 0 ) camera.hpo = 0;
    else if ((camera.hpo / camera.tileWidth) > xBord) camera.hpo = (xBord - 1) * camera.tileWidth;
    else if ((camera.hpo + camera.hSpeed < 0)) camera.hpo = 0;
    else camera.hpo += camera.hSpeed;
    
    if (camera.vpo < 0) camera.vpo = 0;
    else if (camera.vpo / camera.tileHeight > yBord) camera.vpo = (yBord - 1) * camera.tileHeight;
    else if (camera.vpo + camera.vSpeed < 0) camera.vpo = 0;
    else camera.vpo += camera.vSpeed;

    tx = camera.hpo %camera.tileWidth; // get Remainder of tile division
    ty = camera.vpo %camera.tileHeight;
        
    camera.camX = (camera.hpo - tx) /camera.tileWidth; // Gives (x, y) coordinates.
    camera.camY = (camera.vpo - ty) /camera.tileHeight;
        
    jx = camera.camX < 0? (camera.camX) % SCOLS: SCOLS + (camera.camX % SCOLS);
    jy = camera.camY < 0? (camera.camY) % SROWS: SROWS + (camera.camY % SROWS);
        
    hc = ceil( (viewWidth +camera.tileWidth - tx) / (float)camera.tileWidth );
    vc = ceil( (viewHeight +camera.tileHeight -ty) / (float)camera.tileHeight);
    
    for ( yPixel = 0; yPixel < vc; yPixel++ ) {
        for (xPixel = 0; xPixel < hc; xPixel++) {
        
            dBuffX = (xPixel + jx) % DCOLS;
            dBuffY = (yPixel + jy) % DROWS;
            
            dCell = dMap[dBuffX][dBuffY];
            terminal_bkcolor = dCell->backColor;
            terminal_color = dCell->foreColor;
            terminal_put_ext(VIEW_xOFF + xPixel, VIEW_yOFF + yPixel, (xPixel - 1)-tx, 
                                        (yPixel - 1) - ty, dCell->code, NULL);
        }
    }

void drawGameScreen(void){
}
        

// Add Announcement Log Global? Make new one at Main?   
messageString *newMessage( char *string )
{
    messageString *new = malloc(sizeof(messageString));
    new->message = malloc(sizeof(strlen(string)) +1 );
    strcpy(new->message, string);
    new->height = 0;
    new->next = NULL;
    return new;
}

void announce( char *string, messageString *msgChain )
{
    int delta;
    messageString *tmp;
    messageString *new = newMessage(string);
    new->height = terminal_measuref("[bbox=%d]%s", LOG_WIDTH, new->message);
    new->next = msgChain;
    msgChain = new;

    for ( tmp = msgChain; tmp && delta < LOG_HEIGHT; tmp = tmp->next){
        terminal_printf(61, 15 + delta, "[bbox=%d]%s", LOG_WIDTH, tmp->message);
        delta += tmp->height +1;
    }
}
        
void handleInput(void)
{
    int code;
    do {

        code = terminal_read();
        if (code == TK_ESCAPE || code == TK_CLOSE || code == TK_Q) {
            running = 0;
        }
        else if (code == TK_TAB) {
            !camera.scrollMode;
            if (!camera.scrollMode) {
                centerCameraOnPlayer();
            }
        }
        else if (code == TK_K || code == TK_UP || code == TK_KP_8) {
            if ( camera.scrollMode ) {
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
