#include <FTPClient.h>
#include <FTPCommon.h>
#include <FTPServer.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include <FastLED.h>
#include <EEPROM.h>
#define NUM_LEDS 60
#define PIN 5
ESP8266WebServer server(80);
FTPServer ftp(LittleFS);

CRGB leds[NUM_LEDS];
byte counter = 0;
byte brightness = 100;

//unsigned char matrix [1][29][2];
struct {
  byte accentColor = 15;
  byte speed = 128;
  byte interval = 0;
  byte saturation = 255;
  byte value = 128;
  uint step = 0;
  byte mode = 0;
  byte current = 0;
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
} device;

struct {
  unsigned long current = 0;
  unsigned long powerOff;
  unsigned long powerOn;
  unsigned long timer = 0;
  unsigned long lastWrite = 0;
} uptime;

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
  //SPIFFS.begin();
  LittleFS.begin();
  server.begin();
  ftp.begin("admin", "admin");
  networkInit();
  incrementPowerCycle ();
  server.on("/powerSwitch", powerSwitch);
  server.on("/setBrightness", setBrightness);
  server.on("/setAccentColor", setAccentColor);
  server.on("/setSaturation", setSaturation);
  server.on("/setSpeed", setSpeed);
  server.on("/setEffect", setEffect);
  server.on("/setWifi", setWifi);
  server.on("/getEnvironment", getEnvironment);
  server.onNotFound([](){
    if (!handleFileRead(server.uri()))
    server.send (404, "text/plain", "DURILKA");
  });
}

void loop() {
  server.handleClient();
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
  }
}
