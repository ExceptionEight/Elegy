int leds[][] = new int [20][3];
byte col = -127; 
int step = 0;
void setup () {
  size (800, 200, P2D);
  colorMode(HSB);
  frameRate (75);
  //waveInit ();
}

void draw () {
  //pulse ();
  //rainbow ();
  //wave ();
  lakhta();
  delay (80);
}

void lakhta () {
  println (step);
  background (255,255,0);
  if (step == 0) {
    for (int i = 0; i < 20; i+=4) {
      leds[i][0] = 115;
      leds[i][1] = 255;
      leds[i][2] = 255;
      drawLed (i);
    }
  } else if (step <= 5) {
    
  } else {
  step = 0;
  return;
  }
  step++;
}

void waveInit () {
  for (int i = 0; i < 20; i++) {
    leds[i][0] = 115;
    leds[i][1] = 255;
    leds[i][2] = 255;
    drawLed (i);
  }
  col = 115-127;
}

void wave () {
  background (255,255,0);
  byte buffer = col;
  if (step < 20) {
    buffer = col;
   // for (int i = 0; i< 20; i++) {
      col+=10;
   // }
  } else if (step < 40) {
    buffer = col;
      col-=10;
  } else {step = 0;}
  leds[0][0] = buffer+127;
  drawLed (0);
  shiftLeds();
  step++;
}

void shiftLeds () {
  for (int i=19; i>0;i--) {
   println (leds[i][0] + "(" + i + ")=" + leds[i-1][0] + "(" + (i-1) + ")");
   leds[i][0] = leds[i-1][0]; 
   drawLed (i);
   //delay(100);
  }
}

void shiftLeds2 () {
  for (int i=19; i>1;i--) {
   leds[i] = leds[i-1]; 
   drawLed (i-1);
  }
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
