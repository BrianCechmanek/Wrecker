#ifndef _IO_H
#define _IO_H

#include <stdint.h>

uint64_t getTicks(void);
void initWrecker(void);
void handleInput(int code);
void updateGame(void);

#endif
