#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H
/*
 * Wrecker (2016)
 * inputManager.h
 * checks input to see if it has an associated command in
 * our active input domains.
 */

#include <stdbool.h>
#include "uthash.h"

enum inputDomainType{
    INPUT_DOMAIN_GLOBAL = 0,
    INPUT_DOMAIN_MENU,
    INPUT_DOMAIN_GAME,
    INPUT_DOMAIN_TARGETING,
    INPUT_DOMAIN_INVENTORY,
        NUM_OF_INPUT_DOMAINS
};

typedef struct CmdDef 
{
    int keyCode;

    char *standardName;
    void (*standard)(void);

    char *shiftName;
    void (*shiftMod)(void);
    
    char *ctrlName;
    void (*ctrlMod)(void);

    char *altName;
    void (*altMod)(void);

    UT_hash_handle hh;
} CmdDef;

extern float input_mouseX;
extern float input_mouseY; 

/*
 * Add_CmdDef
 * Allows you to add a new command to a particular domain.
 * Ex: add_CmdDef( INPUT_DOMAIN_MENU, menu_up_select, "Select Up", true, TK_UP, false, false, false)
 */
int add_cmdDef( int domain, void (*cb)(void *), char *name, int code, bool shift, bool ctrl, bool alt );

/*
 * toggle_inputDomain
 * lets you activate or deactivate an inputDomain.
 * Multiple can be active at once
 * TODO: Add Error handling
 */
int toggle_inputDomain( int domain );

/*
 * checkCommand
 * inputHandling function.
 * Checks active domains to see if they have a command
 * associated with the input code.
 */
int checkCommand( int code );

void freeAllInputDomains(void);

/*
 * TODO:
 *       Add Monad (exclusivity mode);
 *       Check if key already assigned.
 */
#endif
