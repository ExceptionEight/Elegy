var power = false;
var brightness = 100;
var accentColor = 15;
var saturation = 255;
var speed = 128;
var synced = false;


configUpdater();
function configUpdater() {
  if (synced == false) {
    synced = true;
    brightness = document.getElementById("brightnessSlider").value;
    $.get("config?brightness=" + brightness*10 + "&accentColor=" + accentColor + "&saturation=" + saturation + "&speed=" + speed);
  }
  setTimeout (configUpdater, 250);
}

function powerSwitch(){
  if (power) {
    $.get("powerSwitch?power=0");
  }
  else {
    $.get("powerSwitch?power=1");
  }
  power = !power;
}
