let power = false;
let brightness = 100;
let buffer = {}
let selectedEffect = {name: 'solid', isSettingsActive: false}

const showWifiCard = () => {
  const commonEffectContainer = document.createElement('div')
  commonEffectContainer.className = 'commonEffectContainer'
  commonEffectContainer.style.background = 'black'
  //commonEffectContainer.id = 'wifiSettings'
  const wifiText1 = document.createElement('div')
  wifiText1.className = 'wifiText1'
  wifiText1.innerHTML = 'Ягода,'
  const wifiText2 = document.createElement('div')
  wifiText2.className = 'wifiText2'
  wifiText2.innerHTML = 'настрой вайфай'

  const wifiSSID = document.createElement('input')
  wifiSSID.className = 'wifiInput'
  wifiSSID.id = 'wifiSSID'
  wifiSSID.placeholder = 'SSID'

  const wifiPassword = document.createElement('input')
  wifiPassword.className = 'wifiInput'
  wifiPassword.id = 'wifiPassword'
  wifiPassword.placeholder = 'Пароль'
  wifiPassword.type = 'password'

  const buttonWrapper = document.createElement('div')
  buttonWrapper.className = 'wifiButtonWrapper'

  const saveWifiConfigButton = document.createElement('div')
  saveWifiConfigButton.className = 'saveWifiConfigButton'
  saveWifiConfigButton.innerHTML = 'Оке'
  saveWifiConfigButton.onclick = () => {
    const ssid = document.getElementById('wifiSSID').value
    const pass = document.getElementById('wifiPassword').value
    if (ssid.length == 0 || ssid.length > 32) {
      document.getElementById ('wifiSSID').style.borderStyle = "solid"
      document.getElementById ('wifiSSID').style.borderColor = "red"
    } else if (pass.length < 8) {
      document.getElementById ('wifiPassword').style.borderStyle = "solid"
      document.getElementById ('wifiPassword').style.borderColor = "red"
    } else {
        $.get(`setWifi?ssid=${ssid}&password=${pass}`)
    }
  }


  commonEffectContainer.appendChild(wifiText1)
  commonEffectContainer.appendChild(wifiText2)
  commonEffectContainer.appendChild(wifiSSID)
  commonEffectContainer.appendChild(wifiPassword)
  buttonWrapper.appendChild(saveWifiConfigButton)
  commonEffectContainer.appendChild(buttonWrapper)
  document.getElementById('effectsContainer').appendChild(commonEffectContainer)
}

const showEffects = () => {
  Object.entries(effects).forEach (([key, value]) => {
    const commonEffectContainer = document.createElement('div')
    commonEffectContainer.className = 'commonEffectContainer'
    commonEffectContainer.id = key
    const effectName = document.createElement('p')
    effectName.className = 'effectName'
    effectName.innerHTML = value['name']

    effectName.onclick = () => {
      setEffect (value['id'], value['settings'])
      document.getElementById(selectedEffect.name).classList.remove ('active')
      commonEffectContainer.className = 'commonEffectContainer active'
      if (selectedEffect.name == key && !selectedEffect.isSettingsActive) {
        showEffectSettings(commonEffectContainer, value['settings'])
        selectedEffect.isSettingsActive = true
      } else {
        selectedEffect.name = key
        selectedEffect.isSettingsActive = false
        document.getElementById('settingsEffectContainer').remove()
      }
    }

    commonEffectContainer.appendChild(effectName)
    document.getElementById('effectsContainer').appendChild(commonEffectContainer)
  })
}

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

const setEffect = (id, settings) => {
  let query = `id=${id}`
  if (settings['accentColor'] != null) query += `&accentColor=${settings['accentColor']}`
  if (settings['saturation'] != null) query += `&saturation=${settings['saturation']}`
  if (settings['speed'] != null) query += `&speed=${settings['speed']}`
  if (settings['interval'] != null) query += `&interval=${settings['interval']}`
  buffer.effect = query
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
    if (buffer.effect != null) {
      $.get(`setEffect?${buffer.effect}`)
      delete buffer.effect
    }

    if (buffer.brightness != null) {
      $.get(`setBrightness?value=${buffer.brightness}`)
      delete buffer.brightness
    }

    if (buffer.accentColor != null) {
      $.get(`setAccentColor?value=${buffer.accentColor}`)
      delete buffer.accentColor
    }

    if (buffer.saturation != null) {
      $.get(`setSaturation?value=${buffer.saturation}`)
      delete buffer.saturation
    }

    if (buffer.speed != null) {
      $.get(`setSpeed?value=${buffer.speed}`)
      delete buffer.speed
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
  window.navigator.vibrate(10)
  power = !power;
}
let environment;
const drawer = () => {

  $.getJSON ('getEnvironment', data => {
    document.getElementById('brightnessSlider').value = data.brightness/10
    if (data.connected === false) showWifiCard()
    showEffects()
  })
}
drawer()
