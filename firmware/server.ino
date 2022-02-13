void powerSwitch() {
  if (server.arg("power")=="0") {
    device.power = false;
    effect.swapping = true;
    buffer.current = effect.current;
    buffer.step = effect.step;
    buffer.speed = effect.speed;
    memcpy (buffer.frame, leds, sizeof (buffer.frame));
    effect.current = 0;
    effect.step = 0;
    effect.speed = 100;
    FastLED.clear();
    effect.swapping = false;
  }
  else {
    
  }
  server.send(200);
}

void setEffect () {
  byte id = server.arg("id").toInt();
  if (device.power == false && buffer.current == id) {
    device.power = true;
    effect.swapping = true;
    effect.current = buffer.current;
    effect.step = buffer.step;
    effect.speed = buffer.speed;
    memcpy (leds, buffer.frame, sizeof (leds));
    effect.swapping = false;
    server.send(200);
    Serial.println ("Fu");
    return;
  }

  if (effect.current != id)
  {
    device.power = true;
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
      case 5:
        effect.accentColor = server.arg("accentColor").toInt();
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

void getEnvironment () {
  //p = (power == true) ? "1" : "0";
  //String response = "{\"power\":" + p + "}";
  char response[230] = "";
  strcat (response, "{\"power\":");
  strcat (response, device.power ? "true" : "false");
  strcat (response, ",\"connected\":");
  strcat (response, (WiFi.status() == WL_CONNECTED) ? "true" : "false");
  strcat (response, ",\"brightness\":");
  itoa (brightness, response + strlen(response), DEC);
  strcat (response, ",\"currentEffect\":");
  itoa (effect.current, response + strlen(response), DEC);
  strcat (response, ",\"currentBufferEffect\":");
  itoa (buffer.current, response + strlen(response), DEC);
  strcat (response, ",\"uptime\":");
  itoa (uptime.current, response + strlen(response), DEC);

  strcat (response, ",\"powerOn\":");
  itoa (uptime.powerOn, response + strlen(response), DEC);

  strcat (response, ",\"powerOff\":");
  itoa (uptime.powerOff, response + strlen(response), DEC);

  strcat (response, ",\"powerCycle\":");
  itoa (device.powerCycle, response + strlen(response), DEC);
  strcat (response, "}");
  server.send (200, "application/json", response);
}

bool handleFileRead(String path){
  if(path.endsWith("/")) path += "index.html";
  String contentType = getContentType(path);
  if(LittleFS.exists(path)){
    File file = LittleFS.open(path, "r");
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