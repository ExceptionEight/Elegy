let power = false;
let brightness = 100;
var synced = false;
let buffer = {}

let effects = {
  'default': {

  },
  'solid': {
    accentColor: 15,
    saturation: 255
  },
  'pulse': {
    accentColor: 15,
    saturation: 255,
    speed: 128
  }
}

configUpdater();
//let lastBrightnessSync = 0
const setBrightness = value => {
  buffer.brightness = value * 10
}

const setAccentColor = value => {
  buffer.accentColor = value
}

function configUpdater () {
  if (Object.keys(buffer).length > 0) {
    if (buffer.mode) {
      $.get(`setMode?value=${buffer.mode}`)
      delete buffer.mode
    }

    if (buffer.brightness) {
      $.get(`setBrightness?value=${buffer.brightness}`)
      delete buffer.brightness
    }

    if (buffer.accentColor) {
      $.get(`setAccentColor?value=${buffer.accentColor}`)
      delete buffer.accentColor
    }
  }
  setTimeout (configUpdater, 70);
}
/*
function configUpdater() {
  if (!synced) {
    synced = true;
    brightness = document.getElementById("brightnessSlider").value;
    $.get("config?brightness=" + brightness*10 + "&accentColor=" + accentColor + "&saturation=" + saturation + "&speed=" + speed);
  }
  setTimeout (configUpdater, 250);
}
*/

function powerSwitch(){
  if (power) {
    $.get("powerSwitch?power=0");
  }
  else {
    $.get("powerSwitch?power=1");
  }
  power = !power;
}
