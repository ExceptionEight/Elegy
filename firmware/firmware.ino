#include <FTPClient.h>
#include <FTPCommon.h>
#include <FTPServer.h>

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <FastLED.h>
#include <EEPROM.h>
#define NUM_LEDS 60
#define PIN 5
AsyncWebServer server(80);
FTPServer ftp(LittleFS);

CRGB leds[NUM_LEDS];
byte counter = 0;
byte brightness = 100;

//unsigned char matrix [1][29][2];
struct {
  byte accentColor = 15;
  byte speed = 10;
  byte interval = 0;
  byte saturation = 255;
  byte value = 128;
  byte offset = 0;
  uint step = 0;
  byte mode = 0;
  byte current = 0;
  byte queue[5] = {0, 1, 2, 3, 4}; //using for segments loading animation
  byte colorQueue[5];
  bool swapping = false;
} effect;

struct {
  byte current = 0;
  byte accentColor = 0;
  byte speed = 0;
  byte saturation = 0;
  uint step = 0;
  CRGB frame[NUM_LEDS];
} buffer;

struct {
  unsigned long powerCycle;
  bool power = false;
  bool blinks = true;
} device;

struct {
  unsigned long current = 0;
  unsigned long powerOff;
  unsigned long powerOn;
  unsigned long timer = 0;
  unsigned long lastWrite = 0;
} uptime;

void (*segments) ();

void arrayShuffle (byte array[]) {
  for (byte i = sizeof(array)/sizeof(byte); i > 0; i--) {
    byte j = random (i+1);
    byte temp = array[j];
    array[j] = array[i];
    array[i] = temp;
  }
}

void setup()
{
  WiFi.disconnect();
  EEPROM.begin (512);
  checkEEPROM ();
  recoverUptime ();
  pinMode (5, OUTPUT);
  pinMode (D4, OUTPUT);
  FastLED.addLeds<WS2812, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(10);
  Serial.begin(115200);
  LittleFS.begin();
  server.begin();
  ftp.begin("admin", "admin");
  networkInit();
  incrementPowerCycle ();
  initWebServerFunctions();
  server.begin();
}

void loop() {
  ftp.handleFTP();
  tickEffect();
  uptimeSaver();
  if (Serial.available() >0) {
    int data = Serial.parseInt ();
    if (data <= 117) {
      displayEEPROM (data);
    }
    if (data == 200) {
      hardReset ();
    }
    if (data == 201) {
      shiftLeds (1);
    }
    if (data == 202) {
      Serial.println (effect.step);
    }
  }
}
