#ifndef DESK_LIGHT_DRAWERS
#define DESK_LIGHT_DRAWERS

#include "leds.h"

namespace drawing {
    typedef CRGB (*ColorFunction)(byte i, byte* settings);

    /**
     * 0 - R / H
     * 1 - G / S
     * 2 - B / V
     * 3 - mode (1 - HSV, 2 - RGB)
     */
    CRGB solidDrawer(byte i, byte* settings) {
        switch(settings[3]) {
            case 1: return CHSV(settings[0], settings[1], settings[2]);
            case 2: return CRGB(settings[0], settings[1], settings[2]);
        };
    };

    /**
     * 0 - counter
     * 1 - multiplexor
     * 2 - step
     * 3 - saturation
     * 4 - brightness
     */
    CRGB rainbowDrawer(byte i, byte* settings) {
        CRGB color = CHSV(settings[0] + settings[1] * i, settings[3], settings[4]);
        return color;
    };
  
    ColorFunction* colorFunctions = new ColorFunction[3] { solidDrawer, rainbowDrawer };

    void save(leds::LedLightPanel panel) {
        switch(panel.mode) {
            case 0x01: panel.settings[0] += panel.settings[2];
        }
    }
}

#endif //DESK_LIGHT_DRAWERS
