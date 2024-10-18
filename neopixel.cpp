//Developer:  Charles Palmer, Alex Watts
//Initial:    2024.09.27
//Revision:   2024.09.27

#include "neopixel.h"
#include <Adafruit_NeoPixel.h>
#include "debugger.h"

#define NEOPIXEL_NUM  1

Adafruit_NeoPixel neopixel = Adafruit_NeoPixel(NEOPIXEL_NUM, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

struct NeoPixelColorOptions {
  uint32_t red, green, blue, yellow, black;
};

struct NeoPixelColorOptions neoPixelColors = {
  neopixel.Color(0x20, 0x00, 0x00),   /* RED */
  neopixel.Color(0x00, 0x20, 0x00),   /* GREEN */
  neopixel.Color(0x00, 0x00, 0x20),   /* BLUE */
  neopixel.Color(0x20, 0x20, 0x00),   /* YELLOW */
  neopixel.Color(0x00, 0x00, 0x00)    /* BLACK */
};
//-------------------------------------------------------------------------------------------------
void init_neopixel(void) {
  neopixel.begin();
  debug_ln("Started Neopixel");
  set_neopixel_color(NEOPIXEL_COLOR_YELLOW);
}
//-------------------------------------------------------------------------------------------------
void set_neopixel_color(uint8_t ledColor) {
  // Default to red
  uint32_t selectedColor = neoPixelColors.red;

  // Adjust if a color match is found
  switch (ledColor) {
    case NEOPIXEL_COLOR_RED:
      selectedColor = neoPixelColors.red;
      debug_ln("Neopixel Color Red");
      break;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    case NEOPIXEL_COLOR_GREEN:
      selectedColor = neoPixelColors.green;
      debug_ln("Neopixel Color Green");
      break;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    case NEOPIXEL_COLOR_BLUE:
      selectedColor = neoPixelColors.blue;
      debug_ln("Neopixel Color Blue");
      break;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    case NEOPIXEL_COLOR_YELLOW:
      selectedColor = neoPixelColors.yellow;
      debug_ln("Neopixel Color Yellow");
      break;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    case NEOPIXEL_COLOR_BLACK:
      selectedColor = neoPixelColors.black;
      debug_ln("Neopixel Color Black");
      break;
  }

  // Set led color
  neopixel.fill(selectedColor, 0, NEOPIXEL_NUM);
  neopixel.show();
}