//0-31 - SSID
//32-96 - PASS
//97 - effect
//98 - brightness
//99 - accent color
//100 - speed
//101 - interval
//102-105 - total uptime
//106-109 - power on
//110-113 - power off
//114-117 - power on counter
//118 - initial run

void writeWifiSettings(String ssid, String password) {
for (int i = 0; i < 96; i++) {
  EEPROM.write (i, 0);
}

for (int i = 0; i < ssid.length(); i++) {
   EEPROM.write (i, ssid[i]); 
  // Serial.println (ssid[i]);
}

for (int i = 0; i < password.length(); i++) {
   EEPROM.write (i+32, password[i]); 
   //Serial.println (password[i-32]);
}
EEPROM.commit ();
}

void writeUptime () {
  writeUlongEEPROM (106, uptime.powerOn);
  writeUlongEEPROM (110, uptime.powerOff);
  EEPROM.commit();
}

void hardReset () {
  EEPROM.write (118, 'r');
  checkEEPROM ();
}

void checkEEPROM () {
  if (EEPROM.read(118) != 'w') {
    for (int i = 0; i < 118; i++) {
      EEPROM.write (i, 0);
    }
    EEPROM.write (118, 'w');
    EEPROM.commit ();
  }
}

void displayEEPROM (byte data) {
  Serial.println (EEPROM.read(data));
}

unsigned long readUlongEEPROM (int address) {
  byte raw[4];
  for (byte i = 0; i < 4; i++) raw[i] = EEPROM.read(address+i);
  unsigned long &data = (unsigned long&)raw;
  return data;
}

void writeUlongEEPROM (int address, unsigned long data) {
  byte raw[4];
  (unsigned long&) raw = data;
  for (byte i = 0; i<4; i++) EEPROM.write (address+i, raw[i]);
}

void incrementPowerCycle () {
  unsigned long counter = readUlongEEPROM (114);
  counter++;
  device.powerCycle = counter;
  writeUlongEEPROM (114, counter);
  Serial.println (device.powerCycle);
  EEPROM.commit ();
}