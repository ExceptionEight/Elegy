

void powerOff () {
  effect.step = (effect.step == 200) ? 0 : effect.step;
  if (effect.step <= 20) {
    for (int i = 0; i < NUM_LEDS; i+=5) {
      leds[i] = CHSV((WiFi.status() == WL_CONNECTED) ? 35 : 0, (WiFi.status() == WL_CONNECTED) ? 220 : 255, 255);
    }
  } else {
    FastLED.clear();
  }
  FastLED.show();
  effect.step++;
}

void solidColor () {
  for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(effect.accentColor, effect.saturation, 255);
    }
    FastLED.show();
}

void heartbeatPause() {
  if (effect.mode == 0 && effect.step >= effect.interval) {
    effect.step = 0;
    if (random8(5) == 0) {
      heartbeat = heartbeatPattern1;
    } else {
      heartbeat = heartbeatPattern2;
    }
  } else {
    effect.step++;
  }
}

void heartbeatPattern1 () {
  if (effect.step < 60) {

  } else if (effect.step < 66) {
    effect.value += 21;
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i]  = CHSV(effect.accentColor, effect.saturation, effect.value);
    }
  } else if (effect.step < 71) {
    effect.value -= 21;
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i]  = CHSV(effect.accentColor, effect.saturation, effect.value);
    }
  } else if (effect.step < 76) {
    effect.value += 21;
      for (int i = 0; i < NUM_LEDS; i++) {
        
        leds[i]  = CHSV(effect.accentColor, effect.saturation, effect.value);
    }
    } else if (effect.step < 82) {
    effect.value -= 21;
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i]  = CHSV(effect.accentColor, effect.saturation, effect.value);
    }
  }
     else {
    effect.step = 0;
    heartbeat = heartbeatPause;
  }
  FastLED.show();
  effect.step++;
}

void heartbeatPattern2 () {
  if (effect.step < 60) {

  } else if (effect.step < 70) {
    effect.value += 12;
    for (int i = 0; i < NUM_LEDS; i++) {
      
      leds[i]  = CHSV(effect.accentColor, effect.saturation, effect.value);
    }
  } else if (effect.step < 85) {
    effect.value -= 12;
      for (int i = 0; i < NUM_LEDS; i++) {
        
        leds[i]  = CHSV(effect.accentColor, effect.saturation, effect.value);
    }
  } else if (effect.step < 90) {
    effect.value += 12;
      for (int i = 0; i < NUM_LEDS; i++) {
        
        leds[i] = CHSV(effect.accentColor, effect.saturation, effect.value);
    }
  } else {
    effect.step = 0;
    heartbeat = heartbeatPause;
  }
  FastLED.show();
  effect.step++;
}

void epelepsy () {
  if (effect.step < 4) {
    for (int i = 0; i < NUM_LEDS; i++){
      leds[i] = CHSV(effect.accentColor, effect.saturation, 255);
    }
  } else if (effect.step < 8) {
    for (int i = 0; i < NUM_LEDS; i++){
      leds[i] = CHSV(0, 0, 0);
    }
  } else {
    effect.step = 0;
    effect.accentColor += 127;
  }
  FastLED.show();
  effect.step++;
}

void rainbow () {
  if (effect.step < 255) {
        for (int i = 0; i < NUM_LEDS; i++){
      leds[i] = CHSV(i+effect.step, 255, 255);
    }
      } else {
        effect.step = 0;
      }
      effect.step += 2;
    FastLED.show();
}

void wave () {
  leds[0] = CHSV (effect.accentColor, 255, 255);
  if (effect.step <= effect.offset) {
    effect.accentColor++;
  } else if (effect.step <= effect.offset*2) {
    effect.accentColor--;
  } else {
    effect.step = 0;
  }
  shiftLeds (1);
  effect.step++;
  FastLED.show();
}

void segmentsLoading () {
  if (effect.step == 0) {
    buffer.speed = effect.speed;
    effect.speed = 15;
  }

  if (effect.step < 192) {
    for (byte i = 0; i < NUM_LEDS/5; i++) {
    leds[i+(effect.queue[0]*(NUM_LEDS/5))] = CHSV(effect.colorQueue[0], 255, (byte)effect.step+63);
    }
  }
  if (effect.step > 63 && effect.step < 256) {
    for (byte i = 0; i < NUM_LEDS/5; i++) {
    leds[i+(effect.queue[1]*(NUM_LEDS/5))] = CHSV(effect.colorQueue[1], 255, (byte)effect.step-1);
    }
  }
  if (effect.step > 127 && effect.step < 320) {
    for (byte i = 0; i < NUM_LEDS/5; i++) {
    leds[i+(effect.queue[2]*(NUM_LEDS/5))] = CHSV(effect.colorQueue[2], 255, (byte)effect.step-65);
    }
  }
  if (effect.step > 191 && effect.step < 384) {
    for (byte i = 0; i < NUM_LEDS/5; i++) {
    leds[i+(effect.queue[3]*(NUM_LEDS/5))] = CHSV(effect.colorQueue[3], 255, (byte)effect.step-129);
    }
  }
  if (effect.step > 255 && effect.step < 448) {
    for (byte i = 0; i < NUM_LEDS/5; i++) {
    leds[i+(effect.queue[4]*(NUM_LEDS/5))] = CHSV(effect.colorQueue[4], 255, (byte)effect.step-193);
    }
  }
  effect.step++;
  effect.value++;
  if (effect.step == 448) {
    effect.step = 0;
    effect.mode = 1;
    effect.speed = buffer.speed;
    segments = segmentsLoaded;
  }

  FastLED.show();
}

void segmentsLoaded () {
  if (effect.step % 70 == 0) {
    effect.step = 0;
    byte segment = random (5);
    byte color = effect.accentColor + random (effect.offset+1);
    for (int i = 0; i < NUM_LEDS/5; i++) {
      leds[i+(segment*(NUM_LEDS/5))] = CHSV(color, 255, 255);
    }
    FastLED.show();
  }
  effect.step++;
}

void segmentsFill () {
  for (byte i = 0; i < 5; i++) {
    byte color = effect.accentColor + random (effect.offset+1);
    for (int j = 0; j < NUM_LEDS/5; j++) {
      leds[j+(i*(NUM_LEDS/5))] = CHSV(color, 255, 255);
    }
    FastLED.show();
  }
}

void stars () {
  if (random8 (effect.interval) == 0) {
    byte amount = random8 (3);
    for (int i = 0; i < amount; i++) {
      leds[random16(NUM_LEDS)] = CHSV(effect.accentColor, effect.saturation, 255);
    }
  }
  fadeToBlackBy (leds, NUM_LEDS, 1);
  FastLED.show();
}

void shiftLeds (byte amount) {
  for (int i = NUM_LEDS-1; i > 0; i--) {
    leds[i] = leds[i-1];
  }
}