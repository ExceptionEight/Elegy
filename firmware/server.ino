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

void setWifi () {
  if (server.arg("ssid").length() <= 32 && server.arg("password").length() <=64) {
    writeWifiSettings (server.arg("ssid"), server.arg("password"));
  }
  ESP.reset();
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