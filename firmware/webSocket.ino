void webSocketEvent(const WStype_t& type, uint8_t * payload, const size_t& length)
{
  switch (type)
  {
    case WStype_DISCONNECTED:
      if (isWSConnected)
      {
        Serial.println("[WSc] Disconnected!");
        isWSConnected = false;
      }

      break;

    case WStype_CONNECTED:
    {
      isWSConnected = true;

      Serial.print("[WSc] Connected to url: ");
      Serial.println((char *) payload);

      // send message to server when Connected
      DynamicJsonDocument doc(1024);
      doc["id"] = device.id;
      doc["power"] = device.power;
      doc["effectId"] = effect.current;
      doc["accentColor"] = effect.accentColor;
      doc["saturation"] = effect.saturation;
      doc["brightness"] = brightness;

      String output;
      serializeJson(doc, output);

      webSocket.sendTXT(output);
    }
    break;

    case WStype_TEXT: {
      webSocket.sendTXT("{\"status\":\"ok\"}");
      Serial.printf("[WSc] get text: %s\n", payload);

      DynamicJsonDocument doc(1024);
      Control control;
      DeserializationError error = deserializeJson(doc, payload);

      int action = doc["action"];
      Serial.printf("action: ", action);
      switch (action) {
        case 0:{ //power off
          bool blinks = doc["blinks"];
          control.powerOff (blinks);
        }
          break;
        case 1: //power on
        control.powerOn();
          break;
        case 2:{ //set brightness
          byte brightness = doc["brightness"];
          FastLED.setBrightness (brightness);
        } 
          
          break;
        case 3:{ //set accent color
          byte accentColor = doc["accentColor"];
          effect.accentColor = accentColor;
        }

          break;
        case 4:{ //set saturation
          byte saturation = doc["saturation"];
          effect.saturation = saturation;
        }
          break;
        case 5: { //set accent color & saturation
          byte accentColor = doc["accentColor"];
          effect.accentColor = accentColor;
          byte saturation = doc["saturation"];
          effect.saturation = saturation;
        }
        break;
        case 6: //set speed
          break;
        case 7: //set interval
          break;
        case 8: //set color range
          break;
        case 9:{ //set effect
          byte effectId = doc["effectId"];
          switch (effectId) {
            case 1: {
              byte accentColor = doc["accentColor"];
              byte saturation = doc["saturation"];
              control.setEffect.solidColor(accentColor, saturation);
              }
              break;
            case 2: {
              byte accentColor = doc["accentColor"];
              byte saturation = doc["saturation"];
              byte interval = doc["interval"];
              control.setEffect.heartbeatEffect(accentColor, saturation, interval);
              }
              break;
            case 3: {
              byte speed = doc["speed"];
              control.setEffect.epelepsy(speed);
            }
            break;
            case 4: {
              byte speed = doc["speed"];
              control.setEffect.rainbow(speed);
            }
            break;
            case 7: {
              byte accentColor = doc["accentColor"];
              byte saturation = doc["saturation"];
              byte interval = doc["interval"];
              control.setEffect.stars(accentColor, saturation, interval);
            }
            break;
            case 8: {
              byte accentColor = doc["accentColor"];
              byte offset = doc["offset"];
              byte interval = doc["interval"];
              control.setEffect.fire(accentColor, offset, interval);
            }
            break;
          }
        }
          break;
      }
      // bool power = doc["on"];
      // uint brightness = doc["brightness"];

      // if (power == false) {
      //   control.powerOff(false);
      // } else {
      //   control.powerOn();
      // }

      //Serial.printf ("on: ", on, " brightness: ", br);

      // send message to server
      //sendTXTMessage();
    }
      break;

    case WStype_BIN: 
      Serial.printf("[WSc] get binary length: %u\n", length);
      hexdump(payload, length);

      // send data to server
      webSocket.sendBIN(payload, length);
      break;

    case WStype_PING:
      // pong will be send automatically
      Serial.printf("[WSc] get ping\n");
      break;

    case WStype_PONG:
      // answer to a ping we send
      Serial.printf("[WSc] get pong\n");
      break;

    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      break;

    default:
      break;
  }
}