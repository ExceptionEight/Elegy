let power = false;
let brightness = 100;
let buffer = {}
let selectedEffect = {name: 'solid', isSettingsActive: false}

const getParams = () => {

}

const showEffects = () => {
  const amount = Object.keys(effects).length
  for (let i = 0; i < amount; i++) {
    const commonEffectContainer = document.createElement('div')
    commonEffectContainer.className = 'commonEffectContainer'
    commonEffectContainer.id = Object.keys(effects)[i]
    const effectName = document.createElement('div')
    effectName.className = 'effectName'
    effectName.innerHTML = effects[Object.keys(effects)[i]]['name']

    effectName.onclick = () => {
      //document.getElementById(selectedEffect).className =
      if (selectedEffect.name == Object.keys(effects)[i] && !selectedEffect.isSettingsActive) {
        showEffectSettings(commonEffectContainer, effects[Object.keys(effects)[i]])
        selectedEffect.isSettingsActive = true
      } else {
        selectedEffect.name = Object.keys(effects)[i]
        selectedEffect.isSettingsActive = false
        document.getElementById('settingsEffectContainer').remove()
      }
    }

    commonEffectContainer.appendChild(effectName)
    document.getElementById('effectsContainer').appendChild(commonEffectContainer)
  }
}
showEffects()

const showEffectSettings = (container, effect) => {
  const settingsEffectContainer = document.createElement('div')
  settingsEffectContainer.className = 'settingsEffectContainer'
  settingsEffectContainer.id = 'settingsEffectContainer'
  if (effect.accentColor != null) {
    const sliderWrapper = document.createElement('div')
    sliderWrapper.className = 'effectSliderWrapper'
    const slider = document.createElement('input')
    slider.className = 'hsvSlider'
    slider.type = 'range'
    slider.min = 1
    slider.max = 255
    slider.value = effect.accentColor
    slider.oninput = () => {
      setAccentColor(slider.value)
    }
    sliderWrapper.appendChild(slider)
    settingsEffectContainer.appendChild(sliderWrapper)
  }

  if (effect.saturation != null) {
    const sliderWrapper = document.createElement('div')
    sliderWrapper.className = 'effectSliderWrapper'
    const slider = document.createElement('input')
    slider.className = 'saturationSlider'
    slider.type = 'range'
    slider.min = 1
    slider.max = 255
    slider.value = effect.saturation
    slider.oninput = () => {
      setSaturation(slider.value)
    }
    sliderWrapper.appendChild(slider)
    settingsEffectContainer.appendChild(sliderWrapper)
  }

  if (effect.speed != null) {
    const sliderWrapper = document.createElement('div')
    sliderWrapper.className = 'effectSliderWrapper'
    const slider = document.createElement('input')
    slider.className = 'speedSlider'
    slider.type = 'range'
    slider.min = 1
    slider.max = 255
    slider.value = effect.speed
    slider.oninput = () => {
      setSpeed(slider.value)
    }
    sliderWrapper.appendChild(slider)
    settingsEffectContainer.appendChild(sliderWrapper)
  }
  container.appendChild(settingsEffectContainer)
console.log (effect)
}

configUpdater();
const setBrightness = value => {
  buffer.brightness = value * 10
}

const setAccentColor = value => {
  buffer.accentColor = value
}

const setSaturation = value => {
  buffer.saturation = value
}

const setSpeed = value => {
  buffer.speed = value
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

function powerSwitch(){
  if (power) {
    $.get("powerSwitch?power=0");
  }
  else {
    $.get("powerSwitch?power=1");
  }
  power = !power;
}
