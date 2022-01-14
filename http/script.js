let power = false;
let scrollPosition;
let buffer = {}
let selectedEffect = {name: 'solid', isSettingsActive: false}

const showWifiCard = () => {
  const commonEffectContainer = document.createElement('div')
  commonEffectContainer.className = 'commonEffectContainer'
  commonEffectContainer.style.background = 'black'
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
      setEffect (key, value['id'])
      document.getElementById(selectedEffect.name).classList.remove ('active')
      commonEffectContainer.className = 'commonEffectContainer active'
      if (selectedEffect.name == key && !selectedEffect.isSettingsActive) {
        showEffectSettings(commonEffectContainer, key)
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

const showEffectSettings = (container, effectKey) => {
  let data = JSON.parse(localStorage[effectKey])
  const settingsEffectContainer = document.createElement('div')
  settingsEffectContainer.className = 'settingsEffectContainer'
  settingsEffectContainer.id = 'settingsEffectContainer'

  if (data.activeColor != null) {
    const wrapper = document.createElement('div')
    wrapper.className = 'colorpickCircleWrapper'
    effects[effectKey]['colorScheme'].forEach((e, i) => {
      const colorpickCircle = document.createElement('div')
      colorpickCircle.className = data.activeColor !== i ? 'colorpickCircle' : 'colorpickCircle colorpickCircleSelected'
      colorpickCircle.style.background = `linear-gradient(135deg, ${e[1]},${e[2]})`
      colorpickCircle.onclick = () => {
        setAccentColor(e[0])
        data.activeColor = i
        localStorage.setItem(effectKey, JSON.stringify(data))
        wrapper.getElementsByClassName('colorpickCircleSelected')[0].className = 'colorpickCircle'
        colorpickCircle.className = 'colorpickCircle colorpickCircleSelected'
      }
      wrapper.appendChild(colorpickCircle)
    })
    settingsEffectContainer.appendChild(wrapper)
  }

  if (data.accentColor != null) {
    const sliderWrapper = document.createElement('div')
    sliderWrapper.className = 'effectSliderWrapper'
    const slider = document.createElement('input')
    slider.className = 'hsvSlider'
    slider.type = 'range'
    slider.min = 1
    slider.max = 255
    slider.value = data.accentColor
    slider.oninput = () => {
      setAccentColor(slider.value)
      data.accentColor = slider.value
      if (data.saturation != null) {
        const color = `hsl(${slider.value*1.41176}, 100%, 50%)`
        document.getElementById('saturation').style.background = `linear-gradient(90deg, #fff,${color})`
      }
    }
    slider.onmouseup = () => localStorage.setItem(effectKey, JSON.stringify(data))
    slider.ontouchend = () => localStorage.setItem(effectKey, JSON.stringify(data))
    sliderWrapper.appendChild(slider)
    settingsEffectContainer.appendChild(sliderWrapper)
  }

  if (data.saturation != null) {
    const sliderWrapper = document.createElement('div')
    sliderWrapper.className = 'effectSliderWrapper'
    const slider = document.createElement('input')
    slider.className = 'saturationSlider'
    slider.type = 'range'
    slider.id = 'saturation'
    slider.min = 1
    slider.max = 255
    slider.value = data.saturation
    slider.style.background = `linear-gradient(90deg, #fff,hsl(${data.accentColor*1.41176}, 100%, 50%))`
    slider.oninput = () => {
      setSaturation(slider.value)
      data.saturation = slider.value
    }
    slider.onmouseup = () => localStorage.setItem(effectKey, JSON.stringify(data))
    slider.ontouchend = () => localStorage.setItem(effectKey, JSON.stringify(data))
    sliderWrapper.appendChild(slider)
    settingsEffectContainer.appendChild(sliderWrapper)
  }

  if (data.speed != null) {
    const sliderWrapper = document.createElement('div')
    sliderWrapper.className = 'effectSliderWrapper'
    const slider = document.createElement('input')
    slider.className = 'speedSlider'
    slider.type = 'range'
    slider.min = 1
    slider.max = 15
    slider.value = data.speed
    slider.oninput = () => {
      setSpeed(Number(slider.value))
      data.speed = slider.value
    }
    slider.onmouseup = () => localStorage.setItem(effectKey, JSON.stringify(data))
    slider.ontouchend = () => localStorage.setItem(effectKey, JSON.stringify(data))
    sliderWrapper.appendChild(slider)
    settingsEffectContainer.appendChild(sliderWrapper)
  }

  container.appendChild(settingsEffectContainer)
}

configUpdater();
const setBrightness = value => {
  buffer.brightness = value * 10
}

const setEffect = (key, id) => {
  const settings = JSON.parse(localStorage[key])
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
  buffer.speed = 4+value*(value+1)/2
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
  setTimeout (configUpdater, 70)
}

const powerSwitch = () => {
  if (!power) {
    $.get("powerSwitch?power=0")
    showPowerOffScreen()
  }
  else {
    $.get("powerSwitch?power=1")
    showPowerOnScreen()
  }
  window.navigator.vibrate(10)
  power = !power
}

const setWhiteTheme = () => {
  document.getElementById('mainContainer').style.backgroundColor = '#fff'
  document.getElementById('controllCenter').style.backgroundColor = '#fff'
  document.getElementById('controllCenter').style.borderColor = '#000'
  document.getElementById('powerButton').style.borderColor = '#000'
  document.getElementById('brightnessSlider').style.borderColor = '#000'
  document.getElementById('brightnessSlider').style.backgroundColor = '#fff'
  document.getElementById('brightnessSlider').style.setProperty('--sliderColor', '#000')
}

const setDarkTheme = () => {
  document.getElementById('mainContainer').style.backgroundColor = '#222'
  document.getElementById('controllCenter').style.backgroundColor = '#222'
  document.getElementById('controllCenter').style.borderColor = '#bbb'
  document.getElementById('powerButton').style.borderColor = '#bbb'
  document.getElementById('brightnessSlider').style.borderColor = '#bbb'
  document.getElementById('brightnessSlider').style.backgroundColor = '#222'
  document.getElementById('brightnessSlider').style.setProperty('--sliderColor', '#bbb')
}

const showPowerOnScreen = () => {
  setWhiteTheme()
  document.getElementById('effectsContainer').style.display = 'block'
  document.getElementById('mainContainer').scrollTop = scrollPosition
}

const showPowerOffScreen = () => {
  setDarkTheme()
  scrollPosition = document.getElementById('mainContainer').scrollTop
  document.getElementById('effectsContainer').style.display = 'none'
}

const writeDefaultSettings = () => {
  Object.entries(effects).forEach (([key, value]) => {
    localStorage.setItem(key, JSON.stringify(value['settings']))
  })
}

const initialization = () => {
  if (localStorage.length === 0) writeDefaultSettings ()
  $.getJSON ('getEnvironment', data => {
    document.getElementById('brightnessSlider').value = data.brightness/10
    if (data.connected === false) showWifiCard()
    showEffects()
    selectedEffect.name = Object.keys(effects)[data.currentEffect-1]
    selectedEffect.isSettingsActive = false
    document.getElementById (Object.keys(effects)[data.currentEffect-1]).className = 'commonEffectContainer active'
    if (data.power === true) {
      showPowerOnScreen()
    } else {
      showPowerOffScreen()
    }
  })
}
showEffects()
initialization()
