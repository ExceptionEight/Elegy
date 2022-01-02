//0-31 - SSID
//32-96 - PASS
//97 - effect
//98 - brightness
//99 - accent color
//100 - speed
//101 - interval
//102-106 - total uptime
//107-111 - power on
//112-116 - power off
//117 - initial run

void hardReset () {
  EEPROM.write (117, 'r');
  //EEPROM.commit ();
  checkEEPROM ();

void checkEEPROM () {
  if (EEPROM.read(117) != 'w') {
    for (int i = 0; i < 116; i++) {
      EEPROM.write (i, 0);
    }
    EEPROM.write (117, 'w');
  }
}

void displayEEPROM (byte data) {
  Serial.println (EEPROM.read(data));
}