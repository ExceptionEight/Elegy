unsigned long timer = 0;
void tickEffect () {
  if (millis() - timer >= 10 && !effect.swapping) {
    timer = millis();
    switch (effect.current) {
    case 0:
      powerOff();
      break;
    case 1:
      solidColor ();
      break;
    case 2:
      heartbeat ();
      break;
    case 3:
      epelepsy ();
      break;
    case 4:
      rainbow ();
      break;
    }
  }
}