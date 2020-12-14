#ifndef DESK_LIGHT_BLUETOOTH
#define DESK_LIGHT_BLUETOOTH

#include "desklight.h"
#include "leds.h"
#include <SoftwareSerial.h>

#define COMMAND_MODULE_INFO    0x01
#define COMMAND_PANEL_STATE    0x02
#define COMMAND_SET_MODE       0x04
#define RESULT_FAIL            0x10
#define RESULT_OK              0xFF
#define RESULT_TRANSPORT_ERROR 0xF0

namespace bt {
  SoftwareSerial BtSerial(2, 3);
  
  void setup() {
    BtSerial.begin(9600);
  };
  
  void writePanelState(byte id) {
    leds::LedLightPanel panel = leds::panels[id];
      
    BtSerial.write(panel.length());
    BtSerial.write(panel.enabled());
    
    string name = panel.name();
    BtSerial.write(strlen(name));
    BtSerial.print(name);
  };
  
  void writeModuleInfo() {
    BtSerial.print(MODULE_NAME);
    BtSerial.write(PANELS_COUNT);

    for (byte i = 0; i < PANELS_COUNT; i++) {
      writePanelState(i);
    }
  };

  void writeTransportError() {
    BtSerial.write(RESULT_TRANSPORT_ERROR);
  };

  void processPanelState() {
    if (!BtSerial.available()) {
      writeTransportError();
      return;
    }

    byte id = BtSerial.read();
    if (id > PANELS_COUNT) {
      BtSerial.write(RESULT_FAIL);
      return;
    }
    
    writePanelState(id);
    return;
  };

  bool processSetMode() {
    if (!BtSerial.available()) {
      writeTransportError();
      return false;
    }

    byte id = BtSerial.read();
    if (id > PANELS_COUNT) {
      BtSerial.write(RESULT_FAIL);
    }
    
    if (!BtSerial.available()) {
      writeTransportError();
      return false;
    }
    byte mode = BtSerial.read();

    byte* settings = new byte[SETTINGS_COUNT];
    for (byte i = 0; i < SETTINGS_COUNT; i++) {
      if (!BtSerial.available()) {
        writeTransportError();
        return false;
      }
      settings[i] = BtSerial.read();
    }

    leds::LedLightPanel panel = leds::panels[id];
    panel.mode = mode;
    panel.settings = settings;
    leds::panels[id] = panel;

    BtSerial.write(RESULT_OK);
    return true;
  };

  const byte* password = new byte[4]{ 0xA0, 0x34, 0x76, 0xB3 };
  
  bool processCommand() {
    if (!BtSerial.available()) {
      Serial.println("No data on BT");
      return false;
    }
    
    Serial.println("BT data received");

    for (byte i = 0; i < 4; i++) {
      if (!BtSerial.available()) {
        writeTransportError();
        return false;
      }

      byte data = BtSerial.read();
      if (password[i] != data) {
        Serial.print(data);
        Serial.print(" - ");
        Serial.println(password[i]);
        Serial.println("Incorrect password");
        writeTransportError();
        return false;
      }
    }

    if (!BtSerial.available()) {
      writeTransportError();
      return false;
    }

    switch (BtSerial.read()) {
      case COMMAND_MODULE_INFO: 
        writeModuleInfo();
        return false;
      case COMMAND_PANEL_STATE:
        processPanelState();
        return false;
      case COMMAND_SET_MODE:
        return processSetMode();
      default:
        writeTransportError();
        return false;
    }
  }
};

#endif //DESK_LIGHT_BLUETOOTH
