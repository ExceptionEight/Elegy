void recoverUptime () {
  uptime.powerOn = readUlongEEPROM(106);
  uptime.powerOff = readUlongEEPROM(110);
}

void uptimeSaver () {
  if (millis() - uptime.timer >= 1000) {
    if (device.power) {
      uptime.powerOn++;
    } else {
      uptime.powerOff++;
    }
    uptime.current++;
    uptime.timer = millis();
  }
  if (millis() - uptime.lastWrite >= 300000) {
    writeUptime ();
    uptime.lastWrite = millis();
  }
}