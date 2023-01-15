void initWebServerFunctions() {
  server.on("/powerOff", HTTP_GET, [](AsyncWebServerRequest *request){
    Control control;
    if (request->getParam("blinks")->value().toInt() == -1) {
      control.powerOff (false);
    } else {
      control.powerOff (true);
    } 
    request->send(200);
  });

  server.on("/setEffect", HTTP_GET, [](AsyncWebServerRequest *request){
    Control control;
    byte id = request->getParam("id")->value().toInt();

    if (device.power == false && buffer.current == id) {
      control.powerOn();
      return;
    }

    if (effect.current == id) {
      request->send(200);
      return;
    }

    switch (id) {
      case 1:
        control.setEffect.solidColor (request->getParam("accentColor")->value().toInt(), request->getParam("saturation")->value().toInt());
        break;
      case 2:
        control.setEffect.heartbeatEffect (request->getParam("accentColor")->value().toInt(), request->getParam("saturation")->value().toInt(), request->getParam("speed")->value().toInt());
        break;
      case 3:
        control.setEffect.epelepsy (effect.speed = request->getParam("speed")->value().toInt());
        break;
      case 4:
        control.setEffect.rainbow (effect.speed = request->getParam("speed")->value().toInt());
        break;
      case 5:
        control.setEffect.wave (request->getParam("accentColor")->value().toInt(), request->getParam("offset")->value().toInt(), request->getParam("speed")->value().toInt());
        break;
      case 6:
        control.setEffect.segmentsEffect (request->getParam("accentColor")->value().toInt(), request->getParam("offset")->value().toInt(), request->getParam("speed")->value().toInt());
        break;
      case 7:
        control.setEffect.stars (request->getParam("accentColor")->value().toInt(), request->getParam("saturation")->value().toInt(), request->getParam("speed")->value().toInt());
        break;
      case 8:
        control.setEffect.fire (request->getParam("accentColor")->value().toInt(), request->getParam("offset")->value().toInt(), request->getParam("speed")->value().toInt());
        break;
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
      case 8:
        effect.interval = request->getParam("value")->value().toInt();
        break;
      default:
        effect.speed = request->getParam("value")->value().toInt();
        break;
    }
    request->send(200);
  });

  server.on("/setColorRange", HTTP_GET, [](AsyncWebServerRequest *request){
    Control control;
    control.setColorRange (request->getParam("accentColor")->value().toInt(), request->getParam("offset")->value().toInt());
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