class Control {
  private:
    class SetEffect {
      private:
      void setDefaults () {
        device.power = true;
        effect.swapping = true;
        //effect.current = effectId;
        effect.step = 0;
        effect.mode = 0;
      }
      public:
      void solidColor (byte accentColor, byte saturation) {
        setDefaults();
        effect.current = 1;
        effect.accentColor = accentColor;
        effect.saturation = saturation;
        effect.swapping = false;
      }

      void heartbeatEffect (byte accentColor, byte saturation, byte interval) {
        setDefaults();
        effect.current = 2;
        heartbeat = heartbeatPause;
        FastLED.clear();
        effect.accentColor = accentColor;
        effect.saturation = saturation;
        effect.interval = interval;
        effect.value = 128;
        effect.speed = 15;
        fill_solid (leds, NUM_LEDS, CHSV(effect.accentColor, effect.saturation, effect.value));
        effect.swapping = false; // в деструктор можно эту хуйню добавить?
      }

      void epelepsy (byte speed) {
        setDefaults();
        effect.current = 3;
        effect.speed = speed;
        effect.swapping = false;
      }

      void rainbow (byte speed) {
        setDefaults();
        effect.current = 4;
        effect.speed = speed;
        effect.swapping = false;
      }

      void wave (byte accentColor, byte offset, byte speed) {
        setDefaults();
        effect.current = 5;
        effect.accentColor = accentColor;
        effect.offset = offset;
        effect.speed = speed;
        effect.swapping = false;
      }

      void segmentsEffect (byte accentColor, byte offset, byte speed) {
        setDefaults();
        effect.current = 6;
        segments = segmentsLoading;
        arrayShuffle (effect.queue);
        FastLED.clear();
        effect.accentColor =  accentColor;
        effect.offset = offset;
        effect.speed = speed;

        for (byte i = 0; i < 5; i++) {
          effect.colorQueue[i] = effect.accentColor + random8 (effect.offset+1);
        }
        effect.swapping = false;
      }

      void stars (byte accentColor, byte saturation, byte interval) {
        setDefaults();
        effect.current = 7;
        effect.speed = 15;
        FastLED.clear();
        effect.accentColor = accentColor;
        effect.saturation = saturation;
        effect.interval = interval;
        effect.swapping = false;
      }

      void fire (byte accentColor, byte offset, byte interval) {
        effect.speed = 15;
        effect.accentColor = accentColor;
        effect.offset = offset;
        effect.interval = interval;
        setDefaults();
        effect.current = 8;
        effect.swapping = false;
      }
  };
  public:

  SetEffect setEffect;
  Control() {

  }

  void powerOff (bool blinks) {
    device.power = false;
    effect.swapping = true;
    
    if (effect.current != 0) {
      buffer.current = effect.current;
      buffer.step = effect.step;
      buffer.speed = effect.speed;
      memcpy (buffer.frame, leds, sizeof (buffer.frame));
    }

    if (!blinks) {
      device.blinks = false;
    } else {
      device.blinks = true;
      effect.swapping = false;
    }
      
    effect.current = 0;
    effect.step = 0;
    effect.speed = 10;
    FastLED.clear();
    FastLED.show();
  }

  void powerOn () {
    if (device.power == false) {
      device.power = true;
      effect.swapping = true;
      effect.current = buffer.current;
      effect.step = buffer.step;
      effect.speed = buffer.speed;
      memcpy (leds, buffer.frame, sizeof (leds));
      effect.swapping = false;
    }
  }

  void setColorRange (byte accentColor, byte offset) {
    effect.swapping = true;
    effect.accentColor = accentColor;
    effect.offset = offset;
    switch (effect.current) {
      case 5:
        effect.step = 0;
        break;
      case 6:
        if (effect.mode == 1) segmentsFill();
        break;
    }
    
    effect.swapping = false;
  }
};