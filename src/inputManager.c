/*
 * INPUT MANAGER
 */

#include "dbg.h"
#include "inputManager.h"
#include "BearLibTerminal.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static CmdDef **inputDomains;
static unsigned long activeInputDomains;

float input_mouseX;
float input_mouseY;

void init_cmdManager(void)
{
    inputDomains = malloc( sizeof(CmdDef*) * NUM_OF_INPUT_DOMAINS);
    check_mem( inputDomains );

    for (int i = 0; i < NUM_OF_INPUT_DOMAINS; i++ ){
        inputDomains[i] = NULL;
    }
    return;
error:
    exit(1);
}

int add_cmdDef( int domain, void (*cb)(), char *name, int code, bool shift, bool ctrl, bool alt )
{
    // First we check to see if command already exists.
    CmdDef *newCmd = NULL;
    HASH_FIND_INT( inputDomains[domain], &code, newCmd);
    if ( newCmd == NULL){
        newCmd = malloc( sizeof( CmdDef ) );
        check_mem( newCmd);

        newCmd->keyCode = code;
        HASH_ADD_INT(inputDomains[domain], keyCode, newCmd);
    }

    if (!shift && !ctrl && !alt ){
        if( newCmd->standardName){
            log_info("Overwriting naked %d keycode for %s with %s in domain %d",
                      code, newCmd->standardName, name, domain);
        }
        newCmd->standardName = name;
        newCmd->standard = cb;
    }
    else if( shift && !ctrl && !alt ){
        if( newCmd->shiftName){
            log_info("Overwriting shift+%d keycode for %s with %s in domain %d",
                      code, newCmd->shiftName, name, domain);
        }
        newCmd->shiftName = name;
        newCmd->shiftMod = cb;
    }
    else if (ctrl && !shift && !alt){
        if(newCmd->ctrlName) {
            log_info("Overwriting ctrl+%d keycode for %s with %s in domain %d",
                      code, newCmd->ctrlName, name, domain);
        }
        newCmd->ctrlName = name;
        newCmd->ctrlMod = cb;
    }
    else if (alt && !shift && !ctrl){
        if(newCmd->altName){
            log_info("Overwriting alt+%d keycode for %s with %s in domain %d",
                      code, newCmd->altName, name, domain);
        }
        newCmd->altName = name;
        newCmd->altMod = cb;
    } else {
        sentinel("Error Adding command %s: Only one key-modifier allowed", name);
    }
    return 0;

error:
    exit(1);
}

int toggle_inputDomain( int domain )
{
    check( domain < NUM_OF_INPUT_DOMAINS && domain >= 0, "invalid domain passed to toggle_inputDomain.");
    activeInputDomains ^= (1 << (domain));

    return 0;

error: 
    return 1;
}

int checkCommand( int code )
{
    for ( int i = 0; i < NUM_OF_INPUT_DOMAINS; i++ ){
        // Check if domain bit is flipped.
        if ( activeInputDomains & (1 << i) ) {

            CmdDef *cmd = NULL;
            HASH_FIND_INT(inputDomains[i], &code, cmd);
            if (cmd == NULL){
                continue;
            }
            if (cmd->shiftName && terminal_state( TK_SHIFT )){
                cmd->shiftMod(); 
                return 1;
            }
            else if( cmd->ctrlName && terminal_state( TK_CONTROL)){
                cmd->ctrlMod();
                return 1;
            }
            //else if( cmd->altName && terminal_state( TK_ALT )) {
            //    cmd->altMod();
            //    break;
            else {
                cmd->standard();
                return 1;
            }
        }
    }
    return 0;
}

void freeInputDomain( int domain )
{
    CmdDef *cur, *tmp;
    HASH_ITER(hh, inputDomains[domain], cur, tmp){
        HASH_DEL(inputDomains[domain], cur);
        free(cur);
    }
}

void freeAllInputDomains()
{
    for (int i = 0; i < NUM_OF_INPUT_DOMAINS; i++){
        freeInputDomain(i);
    }
}
