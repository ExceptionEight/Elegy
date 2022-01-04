void networkInit() {
  String ssid;// = "TP-LINK_kapallink";
  String password;// = "1P6A1P2A1969GenA77O98D08D";
  for (int i = 0; i < 32; i++) {
    ssid += char(EEPROM.read(i));
  }
  Serial.println ("SSID: " + ssid);

  for (int i = 32; i < 96; i++) {
    password += char(EEPROM.read(i));
  }
  Serial.println ("PASSWORD: " + password);
  WiFi.begin(ssid.c_str(), password.c_str());
  if (isWifiConnected(millis()+10000)) {
    Serial.println ("Wifi connected");
    WiFi.softAPdisconnect (true);
    //set default mode
  } else {
    accessPoint ();
    Serial.println ("AP");
  }
}

bool isWifiConnected (unsigned long expiration) {
  while (millis() < expiration) {
    if (WiFi.status() == WL_CONNECTED) {
      return true;
    }
    yield();
  }
  return false;
}

void accessPoint () {
  WiFi.softAP("Elegy", "12344321");
}