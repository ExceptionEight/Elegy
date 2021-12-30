int leds[][] = new int [20][3];
byte col = -127; 
int step = 0;
void setup () {
  size (800, 200, P2D);
  colorMode(HSB);
  frameRate (75);
  pulseInit ();
  //println ()
  
}

void draw () {
  //PULSE 
  pulse();
  delay (5);
}

void rainbow () {
  background (255,255,0);
  byte buffer = col;
  for (int i = 0; i < 20; i++) {
    leds[i][0] = buffer+127;
    leds[i][1] = 255;
    leds[i][2] = 255;
    drawLed (i);
    buffer+=2;
  }
  col +=2;
}

void pulseInit () {
  for (int i = 0; i < 20; i++) {
    leds[i][0] = 0;
    leds[i][1] = 255;
    leds[i][2] = 128;
    drawLed (i);
  }
}

void pulse () {
  if (step < 60) {
    
  } else if (step < 70) {
    for (int i = 0; i< 20; i++) {
      leds[i][0] = 0;
      leds[i][1] = 255;
      leds[i][2] += 12;
      drawLed (i);
    }
    println (step + " " + leds[0][2]);
  } else if (step < 85) {
    for (int i = 0; i< 20; i++) {
      leds[i][0] = 0;
      leds[i][1] = 255;
      leds[i][2] -= 12;
      drawLed (i);
    }
    println (step + " " + leds[0][2]);
  } else if (step < 90) {
    for (int i = 0; i< 20; i++) {
      leds[i][0] = 0;
      leds[i][1] = 255;
      leds[i][2] += 12;
      drawLed (i);
    }
    println (step + " " + leds[0][2]);
  } else {
   step = 0; 
  }
  
  step++;
}

void drawLed (int led) {
  fill (leds[led][0], leds[led][1], leds[led][2]);
  circle (led*40+20, 40, 30);
}


/* RAINBOW (dash)
background (255,255,0);
  byte buffer = col;
  for (int i = 0; i < 20; i++) {
    leds[i][0] = buffer+127;
    leds[i][1] = 255;
    leds[i][2] = 255;
    fill (leds[i][0], leds[i][1], leds[i][2]);
    circle (i*40+20, 40, 30);
    buffer+=2;
  }
  col +=2;
  delay (15);
*/
