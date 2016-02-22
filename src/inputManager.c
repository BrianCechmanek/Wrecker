/*
 * INPUT MANAGER
 */

#include "dbg.h"
#include "inputManager.h"
#include "BearLibTerminal.h"
#include "dataStructures.h"
#include "dbg.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static List **inputDomainList;
static unsigned long activeInputDomains;

float input_mouseX;
float input_mouseY;

void init_cmdManager(void)
{
    inputDomainList = malloc( NUM_OF_INPUT_DOMAINS * sizeof(List*));
    check_mem( inputDomainList );

    for (int i = 0; i < NUM_OF_INPUT_DOMAINS; i++ ){
        inputDomainList[i] = createList();
    }
    return;
error:
    exit(1);
}

int add_cmdDef( int domain, void (*cb)(), char *name, bool isKB, int code, bool shift, bool ctrl, bool alt ){
    CmdDef *newCommand = malloc( sizeof( CmdDef ) );
    check_mem( newCommand );

    newCommand->cb = cb;
    newCommand->name = name;
    newCommand->code = code;
    newCommand->shift = shift;
    newCommand->ctrl = ctrl;
    newCommand->alt = alt;

    listAppend( inputDomainList[ domain ], newCommand );

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
    int active = 0;
    for ( int i = 0; i < NUM_OF_INPUT_DOMAINS; i++ ){
        if ( activeInputDomains & (1 << i) ) {
            active++;
            Node *tmp = inputDomainList[i]->first;
            while (tmp != NULL){
                CmdDef *cmd = (CmdDef *) tmp->data;
                if ( code == cmd->code) {
                    if( cmd->shift){
                        if ( !terminal_state( TK_SHIFT ) ) continue;
                    }
                    if( cmd->ctrl ){
                        if ( !terminal_state( TK_CONTROL) ) continue;
                    }
                    /* TODO: Add Alt CHARCODE to bearlib.
                    if( cmd->alt ){
                        if (!terminal_state( TK_ALT )) continue;
                    }
                    */
                    cmd->cb(NULL);
                    return 0;
                }
                tmp = tmp->next;
            }
        }
    }
    printf("Active Domains: %d\n", active);

    return 1;
}

void freeInputDomain( int domain )
{
    freeList( inputDomainList[domain] );
}

/*
void foo(void *data)
{
    printf("Woot!!!\n");
}

int main(void)
{
    init_cmdManager();
    toggle_inputDomain( 1 );

    add_cmdDef( 1, foo, "Foo Command!", true, TK_O, false, false, false);

    checkCommand( TK_O );
    return 0;
}
*/
