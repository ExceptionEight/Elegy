const effects = {
  'solid': {
    id: 1,
    name: 'Монотонный',
    settings: {
      accentColor: 115,
      saturation: 255
    }
  },
  'pulse': {
    id: 2,
    name: 'Кокоро',
    speedParameters: [
      1, //min
      16, //max
      0, //add to result value (100 for e.g.) | first step
      1.875, //multiply result to (2 or 2.718 for e.g.) | second step
      false, //substract from (false or number) | last step
      true //is triangular number (true) or (false) for raw
    ],
    settings: {
      accentColor: 115,
      saturation: 255,
      speed: 3
    }
  },
  'epilepsy': {
    id: 3,
    name: 'Привет эпилептики',
    speedParameters: [1, 16, 0, 1.875, false, true],
    settings: {
      speed: 3
    }
  },
  'rainbow': {
    id: 4,
    name: 'ЛГБТ',
    speedParameters: [1, 16, 0, 1.875, false, true],
    settings: {
      speed: 3
    }
  },
  'waves': {
    id: 5,
    name: 'Волны',
    colorScheme: [
        [115, 30, '#00ffaa', '#00c8ff'],
        [0, 30, '#ff0000', '#ffb300'],
        [200, 30, '#ff00ee', '#ff0077'],
      [220, 30, '#ff00b3', '#ff0040'],
      [80, 10, '#00ff00', '#00ffd5'],
      [145, 10, '#0022ff', '#8400ff'],
      [170, 10, '#8400ff', '#ae00ff'],
    ],
    speedParameters: [1, 16, 0, 1.875, false, true],
    settings: {
      speed: 3,
      activeColor: 0
    }
  },
  'segments': {
    id: 6,
    name: 'Сегменты',
    colorScheme: [
      [115, 30, '#00ffaa', '#00c8ff'],
      [0, 30, '#ff0000', '#ffb300'],
      [200, 30, '#ff00ee', '#ff0077'],
      [220, 30, '#ff00b3', '#ff0040'],
      [80, 10, '#00ff00', '#00ffd5'],
      [145, 10, '#0022ff', '#8400ff'],
      [170, 10, '#8400ff', '#ae00ff']
    ],
    speedParameters: [1, 16, 0, 1.875, false, true],
    settings: {
      speed: 3,
      activeColor: 0
    }
  },
  'stars': {
    id: 7,
    name: 'Звезды',
    speedParameters: [1, 16, 0, 1, false, true],
    settings: {
      accentColor: 115,
      saturation: 255,
      speed: 10
    }
  },
  'fire': {
    id: 8,
    name: 'Агонь',
    colorScheme: [
      [115, 30, '#00ffaa', '#00c8ff'],
      [0, 30, '#ff0000', '#ffb300'],
      [200, 30, '#ff00ee', '#ff0077'],
      [220, 30, '#ff00b3', '#ff0040'],
      [80, 10, '#00ff00', '#00ffd5'],
      [145, 10, '#0022ff', '#8400ff'],
      [170, 10, '#8400ff', '#ae00ff']
    ],
    speedParameters: [1, 16, -1, 0.8, false, true],
    settings: {
      speed: 3,
      activeColor: 0
    }
  }
}
