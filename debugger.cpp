#include "debugger.h"

bool debuggerRunning = false;

void init_debugger(void) {
#if __DEBUGGER_ON__
  Serial.begin(115200);
  debuggerRunning = true;
  debug_ln("Debugger Started");
#endif
}
//-------------------------------------------------------------------------------------------------
void debug_ln(String output_string) {
#if __DEBUGGER_ON__
  if(debuggerRunning) {
    Serial.println(output_string);
  }
#endif
}