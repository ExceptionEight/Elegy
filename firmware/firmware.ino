#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ESP8266FtpServer.h>
#include <FastLED.h>
#include <EEPROM.h>
#define NUM_LEDS 60
#define PIN 5
ESP8266WebServer server(80);
FtpServer ftp;

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
  byte current = 2;
  bool swapping = false;
} effect;


void setup()
{
  EEPROM.begin (512);
  pinMode (5, OUTPUT);
  pinMode (D4, OUTPUT);
  FastLED.addLeds<WS2812, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(10);
  Serial.begin(115200);
  //WiFi.begin("TP-LINK_kapallink", "1p6a1p2a1969GenA77O98D08D");
  //WiFi.softAP("Elegy", "12344321");
  SPIFFS.begin();
  server.begin();
  ftp.begin("admin", "admin");

  networkInit();
  server.on("/powerSwitch", powerSwitch);
  server.on("/setBrightness", setBrightness);
  server.on("/setAccentColor", setAccentColor);
  server.on("/setSaturation", setSaturation);
  server.on("/setSpeed", setSpeed);
  server.on("/setEffect", setEffect);
  server.onNotFound([](){
    if (!handleFileRead(server.uri()))
    server.send (404, "text/plain", "DURILKA");
  });
}

void powerSwitch() {
  if (server.arg("power")=="1") {
    digitalWrite(D4, LOW);
    FastLED.setBrightness(0);
  }
  else {
    digitalWrite (D4, HIGH);
    FastLED.setBrightness(brightness);
  }
  server.send(200);
}

void setBrightness () {
  brightness = server.arg("value").toInt();
  FastLED.setBrightness(brightness);
  server.send(200);
}

void setAccentColor () {
  effect.accentColor = server.arg("value").toInt();
  server.send(200);
}

void setSaturation () {
  effect.saturation = server.arg("value").toInt();
  server.send(200);
}

void setSpeed () {
  effect.speed = server.arg("value").toInt();
  server.send(200);
}

void setEffect () {
  byte id = server.arg("id").toInt();
  if (effect.current != id)
  {
    effect.swapping = true;
    effect.current = id;
    effect.step = 0;
    effect.mode = 0;
    switch (id) {
      case 1:
        effect.accentColor = server.arg("accentColor").toInt();
        break;
      case 2:
        effect.accentColor = server.arg("accentColor").toInt();
        effect.value = 128;
        break;
    }
    effect.swapping = false;
  }
  server.send(200);
}

void loop() {
  server.handleClient();
  ftp.handleFTP();
  tickEffect();
  if (Serial.available() >0) {
    int data = Serial.parseInt ();
    if (data <= 117) {
      displayEEPROM (data);
    }
    if (data == 200) {
      hardReset ();
    }
  }
}

bool handleFileRead(String path){
  if(path.endsWith("/")) path += "index.html";
  String contentType = getContentType(path);
  if(SPIFFS.exists(path)){
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

String getContentType(String filename){
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".svg")) return "image/svg+xml";
  return "text/plain";
}
