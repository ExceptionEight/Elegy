void initWebServerFunctions() {
  server.on("/powerOff", HTTP_GET, [](AsyncWebServerRequest *request){
    device.power = false;
    effect.swapping = true;
    if (request->getParam("blinks")->value().toInt() == -1) {
      device.blinks = false;
    } else {
      device.blinks = true;
      effect.swapping = false;
    } 
    if (effect.current != 0) {
      buffer.current = effect.current;
      buffer.step = effect.step;
      buffer.speed = effect.speed;
      memcpy (buffer.frame, leds, sizeof (buffer.frame));
    }
      
    effect.current = 0;
    effect.step = 0;
    effect.speed = 10;
    FastLED.clear();
    FastLED.show();
    request->send(200);
  });

  server.on("/setEffect", HTTP_GET, [](AsyncWebServerRequest *request){
    byte id = request->getParam("id")->value().toInt();
     if (device.power == false && buffer.current == id) {
    device.power = true;
    effect.swapping = true;
    effect.current = buffer.current;
    effect.step = buffer.step;
    effect.speed = buffer.speed;
    memcpy (leds, buffer.frame, sizeof (leds));
    effect.swapping = false;
    request->send(200);
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
        effect.accentColor = request->getParam("accentColor")->value().toInt();
        effect.saturation = request->getParam("saturation")->value().toInt();
        break;
      case 2:
        heartbeat = heartbeatPause;
        FastLED.clear();
        effect.accentColor = request->getParam("accentColor")->value().toInt();
        effect.saturation = request->getParam("saturation")->value().toInt();
        effect.interval = request->getParam("speed")->value().toInt();
        effect.value = 128;
        effect.speed = 15;
        fill_solid (leds, NUM_LEDS, CHSV(effect.accentColor, effect.saturation, effect.value));
        break;
      case 3:
        effect.speed = request->getParam("speed")->value().toInt();
        break;
      case 4:
        effect.speed = request->getParam("speed")->value().toInt();
        break;
      case 5:
        effect.accentColor = request->getParam("accentColor")->value().toInt();
        effect.offset = request->getParam("offset")->value().toInt();
        effect.speed = request->getParam("speed")->value().toInt();
        break;
      case 6:
        segments = segmentsLoading;
        arrayShuffle (effect.queue);
        FastLED.clear();
        effect.accentColor = request->getParam("accentColor")->value().toInt();
        effect.offset = request->getParam("offset")->value().toInt();
        effect.speed = request->getParam("speed")->value().toInt();

        for (byte i = 0; i < 5; i++) {
          effect.colorQueue[i] = effect.accentColor + random8 (effect.offset+1);
        }
        break;
      case 7:
        effect.speed = 15;
        FastLED.clear();
        effect.accentColor = request->getParam("accentColor")->value().toInt();
        effect.saturation = request->getParam("saturation")->value().toInt();
        effect.interval = request->getParam("speed")->value().toInt();
        break;
    }
    effect.swapping = false;
  }
  request->send(200);
  });

  server.on("/setBrightness", HTTP_GET, [](AsyncWebServerRequest *request){
    FastLED.setBrightness(request->getParam("value")->value().toInt());
    request->send(200);
  });

  server.on("/setAccentColor", HTTP_GET, [](AsyncWebServerRequest *request){
    effect.accentColor = request->getParam("value")->value().toInt();
    request->send(200);
  });

  server.on("/setSaturation", HTTP_GET, [](AsyncWebServerRequest *request){
    effect.saturation = request->getParam("value")->value().toInt();
    request->send(200);
  });

  server.on("/setSpeed", HTTP_GET, [](AsyncWebServerRequest *request){
    switch (effect.current) {
      case 2:
        effect.interval = request->getParam("value")->value().toInt();
        break;
      case 7:
        effect.interval = request->getParam("value")->value().toInt();
        break;
      default:
        effect.speed = request->getParam("value")->value().toInt();
        break;
    }
    request->send(200);
  });

  server.on("/setColorRange", HTTP_GET, [](AsyncWebServerRequest *request){
    effect.swapping = true;
    effect.accentColor = request->getParam("accentColor")->value().toInt();
    effect.offset = request->getParam("offset")->value().toInt();
    switch (effect.current) {
      case 5:
        effect.step = 0;
        break;
      case 6:
        if (effect.mode == 1) segmentsFill();
        break;
    }
    
    effect.swapping = false;
    request->send(200);
  });

  server.on("/setWifi", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->getParam("ssid")->value().length() <= 32 && request->getParam("password")->value().length() <=64) {
      writeWifiSettings (request->getParam("ssid")->value(), request->getParam("password")->value());
      ESP.reset();
    }
  });

  server.on("/getEnvironment", HTTP_GET, [](AsyncWebServerRequest *request){
  char response[230] = "";
  strcat (response, "{\"power\":");
  strcat (response, device.power ? "true" : "false");
  strcat (response, ",\"nextBlink\":");
  itoa ((!device.power && device.blinks) ? (200 - effect.step) * 10 : -1, response + strlen(response), DEC);
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
  request->send(200, "application/json", response);
  });

  server.onNotFound([](AsyncWebServerRequest *request){
    String path = request->url();
    if(path.endsWith("/")) path += "index.html";
    String contentType = getContentType(path);
    if (LittleFS.exists(path)){
      request->send(LittleFS, path, contentType);
    } else {
      request->send (404, "text/plain", "DURILKA");
    }
  });
}

/*bool handleFileRead(String path)){
  if(path.endsWith("/")) path += "index.html";
  String contentType = getContentType(path);
  if(LittleFS.exists(path)){
    File file = LittleFS.open(path, "r");
    size_t fileSize = file;
    request->send (404, "text/plain", "DURILKA_HYR");
    //request->send(file, contentType, fileSize);
    file.close();
    return true;
  }
  return false;
}
*/

String getContentType(String filename){
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".svg")) return "image/svg+xml";
  return "text/plain";
}