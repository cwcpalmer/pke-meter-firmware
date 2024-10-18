#ifndef __neopixel__
#define __neopixel__

#include <arduino.h>

// Color Choices
#define NEOPIXEL_COLOR_RED    0
#define NEOPIXEL_COLOR_GREEN  1
#define NEOPIXEL_COLOR_BLUE   2
#define NEOPIXEL_COLOR_YELLOW 3
#define NEOPIXEL_COLOR_BLACK  4

void init_neopixel(void);
void set_neopixel_color(uint8_t ledColor);

#endif