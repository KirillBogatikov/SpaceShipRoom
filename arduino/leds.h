#ifndef DESK_LIGHT_LEDS
#define DESK_LIGHT_LEDS

#include "desklight.h"

#include <FastLED.h>

#define MIN_STATUS_VALUE 400
#define PANELS_COUNT 6
#define SETTINGS_COUNT 5

#define WORKBENCH_WALL_LENGTH 47
#define WORKBENCH_WALL_PIN 11

CRGB workbenchWall[WORKBENCH_WALL_LENGTH];

#define WORKBENCH_MAIN_LENGTH 60
#define WORKBENCH_MAIN_PIN 4
#define WORKBENCH_STATUS_PIN A7

CRGB workbenchMain[WORKBENCH_MAIN_LENGTH];

#define FIRST_SHELF_LENGTH 60
#define FIRST_SHELF_PIN 9
#define FIRST_SHELF_STATUS_PIN A2

CRGB firstShelf[FIRST_SHELF_LENGTH];

#define SECOND_SHELF_LENGTH 59
#define SECOND_SHELF_PIN 8
#define SECOND_SHELF_STATUS_PIN A3

CRGB secondShelf[SECOND_SHELF_LENGTH];

#define THIRD_SHELF_LENGTH 60
#define THIRD_SHELF_PIN 7
#define THIRD_SHELF_STATUS_PIN A5

CRGB thirdShelf[THIRD_SHELF_LENGTH];

#define UNDERTABLE_LENGTH 50
#define UNDERTABLE_PIN 6

CRGB undertable[UNDERTABLE_LENGTH];

namespace leds {
  typedef struct LedLightPanel {
    byte id;
    byte mode;
    byte* settings;
    byte brightness;

    byte length() {
      switch(id) {
        case 0: return WORKBENCH_WALL_LENGTH;
        case 1: return WORKBENCH_MAIN_LENGTH;
        case 2: return FIRST_SHELF_LENGTH;
        case 3: return SECOND_SHELF_LENGTH;
        case 4: return THIRD_SHELF_LENGTH;
        case 5: return UNDERTABLE_LENGTH;
        default: return 0;
      }
    }
    
    void set(byte pixel, CRGB color) {
      switch(id) {
        case 0: workbenchWall[pixel] = color; break;
        case 1: workbenchMain[pixel] = color; break;
        case 2: firstShelf[pixel] = color; break;
        case 3: secondShelf[pixel] = color; break;
        case 4: thirdShelf[pixel] = color; break;
        case 5: undertable[pixel] = color; break;
      }
    };

    bool enabled() { 
      byte pinStatus;
      switch(id) {
        case 0: 
        case 1: pinStatus = WORKBENCH_STATUS_PIN; break;
        case 2: pinStatus = FIRST_SHELF_STATUS_PIN; break;
        case 3: pinStatus = SECOND_SHELF_STATUS_PIN; break;
        case 4: pinStatus = THIRD_SHELF_STATUS_PIN; break;
        case 5: return true;
      }

      int status = analogRead(pinStatus);
      Serial.print(" -- ");
      Serial.print(status);
      Serial.print(" -- ");
           
      return status > MIN_STATUS_VALUE;
    }
  };

  LedLightPanel* panels = new LedLightPanel[PANELS_COUNT];

  void setup() {    
    FastLED.addLeds<WS2812B, UNDERTABLE_PIN, GRB>(undertable, UNDERTABLE_LENGTH).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<WS2812B, WORKBENCH_WALL_PIN, GRB>(workbenchWall, WORKBENCH_WALL_LENGTH).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<WS2812B, WORKBENCH_MAIN_PIN, GRB>(workbenchMain, WORKBENCH_MAIN_LENGTH).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<WS2812B, FIRST_SHELF_PIN, GRB>(firstShelf, FIRST_SHELF_LENGTH).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<WS2812B, SECOND_SHELF_PIN, GRB>(secondShelf, SECOND_SHELF_LENGTH).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<WS2812B, THIRD_SHELF_PIN, GRB>(thirdShelf, THIRD_SHELF_LENGTH).setCorrection(TypicalLEDStrip);
    
    FastLED.setBrightness(192);
    for (byte i = 0; i < PANELS_COUNT; i++) {
      
      
      panels[i] = LedLightPanel {
        id: i,
        mode: 0,
        //settings: new byte[SETTINGS_COUNT] { 0x00, 0x02, 0x02, 0xFF, 0xFF }
        settings: new byte[SETTINGS_COUNT] { 0xFF, 0xFF, 0xFF, 0x02, 0xFF }
      };
    }
/*
    pinMode(WORKBENCH_WALL_PIN, OUTPUT);
    pinMode(WORKBENCH_MAIN_PIN, OUTPUT);
    pinMode(FIRST_SHELF_PIN, OUTPUT);
    pinMode(SECOND_SHELF_PIN, OUTPUT);
    pinMode(THIRD_SHELF_PIN, OUTPUT);
    pinMode(UNDERTABLE_PIN, OUTPUT);*/
    
    pinMode(WORKBENCH_STATUS_PIN, INPUT);
    pinMode(FIRST_SHELF_STATUS_PIN, INPUT);
    pinMode(SECOND_SHELF_STATUS_PIN, INPUT);
    pinMode(THIRD_SHELF_STATUS_PIN, INPUT);
  }

  void show() {
    FastLED.show();
  }
}

#endif //DESK_LIGHT_LEDS
