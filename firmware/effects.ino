void powerOff () {

}

void solidColor () {
  for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(effect.accentColor, effect.saturation, 255);
    }
    FastLED.show();
}

void heartbeat() {
  if (effect.mode == 0) {
    if (random(5) == 0) {
      effect.mode = 1;
    } else {
      effect.mode = 2;
    }
  }
  if (effect.mode == 1) {
    heartbeat1 ();
  } else if (effect.mode == 2) {
    heartbeat2 ();
  }
}

void heartbeat1 () {
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
    effect.mode = 0;
  }
  FastLED.show();
  effect.step++;
}

void heartbeat2 () {
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
    effect.mode = 0;
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