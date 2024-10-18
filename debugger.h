#ifndef __debugger__
#define __debugger__

#define __DEBUGGER_ON__ 1

#include <arduino.h>

void init_debugger(void);
void debug_ln(String output_string);

#endif