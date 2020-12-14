#include "leds.h"
#include "drawers.h"

#include <SoftwareSerial.h>
SoftwareSerial BtSerial(2, 3);

void setup() {
    Serial.begin(9600);
    leds::setup();
  
    pinMode(2,INPUT);
    pinMode(3,OUTPUT);
    BtSerial.begin(9600);
    Serial.println("start prg");
}

char* EXPECTED_PASSWORD = "p1Z5q6cA";
#define PACKAGE_LENGTH 32
#define PASSWORD_LENGTH 8
#define PAYLOAD_LENGTH PACKAGE_LENGTH - PASSWORD_LENGTH

bool eq(char* a, char *b, byte l) {
    for (byte i = 0; i < l; i++) {
      if (a[i] != b[i]) {
          return false;
      }
    }

    return true;
}

void println(char* str, byte l) {
    for (byte i = 0; i < l; i++) {
        Serial.print(str[i]);
    }
    Serial.println();
}

unsigned long time;

void loop() {
    if (millis() - time > 1000) {
        if (BtSerial.available()) {
            char* password = new char[PASSWORD_LENGTH]{};
            for (byte i = 0; i < PASSWORD_LENGTH; i++) {
                password[i] = BtSerial.read();
            }

            Serial.print("\nPassword: ");
            println(password, PASSWORD_LENGTH);
            bool passwordCheck = eq(password, EXPECTED_PASSWORD, PASSWORD_LENGTH);
            if (!passwordCheck) {
            Serial.println("Password incorrect");
            }

            delete password;

            char* payload = new char[PAYLOAD_LENGTH];

            for (byte i = 0; i < PAYLOAD_LENGTH; i++) {
                if (!BtSerial.available()) {
                    Serial.println("Payload incorrect");
                    return;
                }

                payload[i] = BtSerial.read();
            }

            Serial.print("Payload");
            println(payload, PAYLOAD_LENGTH);

            delete payload;

            if (!passwordCheck) {
                return;
            }

            Serial.println("----");
        }
  
        Serial.println("No data");
        time = millis();
    }
  
    for (byte p = 0; p < PANELS_COUNT; p++) {
        leds::LedLightPanel panel = leds::panels[p];

        for (byte i = 0; i < panel.length(); i++) {
            drawing::ColorFunction colorFunction = drawing::colorFunctions[panel.mode];
            panel.set(i, colorFunction(i, panel.settings));
        }

        drawing::save(panel);
        leds::panels[p] = panel;
    }

    leds::show();

    delay(10);
}
