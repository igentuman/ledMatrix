// крутые полноэкраные эффекты
// ******************* НАСТРОЙКИ *****************
// "масштаб" эффектов. Чем меньше, тем крупнее!
#define MADNESS_SCALE 100
#define CLOUD_SCALE 30
#define LAVA_SCALE 50
#define PLASMA_SCALE 30
#define RAINBOW_SCALE 30
#define RAINBOW_S_SCALE 20
#define ZEBRA_SCALE 30
#define FOREST_SCALE 120
#define OCEAN_SCALE 90

// ***************** ДЛЯ РАЗРАБОТЧИКОВ ******************
#if (USE_NOISE_EFFECTS == 1)
// The 16 bit version of our coordinates
static uint16_t x;
static uint16_t y;
static uint16_t z;

uint16_t speed = 20; // speed is set dynamically once we've started up
uint16_t scale = 30; // scale is set dynamically once we've started up

// This is the array that we keep our computed noise values in
#define MAX_DIMENSION (max(WIDTH, HEIGHT))
#if (WIDTH > HEIGHT)
uint8_t noise[WIDTH][WIDTH];
#else
uint8_t noise[HEIGHT][HEIGHT];
#endif

CRGBPalette16 currentPalette( PartyColors_p );
uint8_t colorLoop = 1;
uint8_t ihue = 0;

void madnessNoise(int mode) {
  if (loadingFlag) {
    loadingFlag = false;
    scale = MADNESS_SCALE;
    modeCode = mode;
  }
  fillnoise8();
  for (int i = 0; i < WIDTH; i++) {
    for (int j = 0; j < HEIGHT; j++) {
      CRGB thisColor = CHSV(noise[j][i], 255, noise[i][j]);
      drawPixelXY(i, j, thisColor);   //leds[getPixelNumber(i, j)] = CHSV(noise[j][i], 255, noise[i][j]);

      // You can also explore other ways to constrain the hue used, like below
      // leds[XY(i,j)] = CHSV(ihue + (noise[j][i]>>2),255,noise[i][j]);
    }
  }
  ihue += 1;
}
void rainbowNoise(int mode) {
  if (loadingFlag) {
    loadingFlag = false;
    currentPalette = RainbowColors_p;
    scale = RAINBOW_SCALE; colorLoop = 1;
    modeCode = mode;
  }
  fillNoiseLED();
}
void rainbowStripeNoise(int mode) {
  if (loadingFlag) {
    loadingFlag = false;
    currentPalette = RainbowStripeColors_p;
    scale = RAINBOW_S_SCALE; colorLoop = 1;
    modeCode = mode;
  }
  fillNoiseLED();
}
void zebraNoise(int mode) {
  if (loadingFlag) {
    loadingFlag = false;
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    scale = ZEBRA_SCALE; colorLoop = 1;
    modeCode = mode;
  }
  fillNoiseLED();
}
void forestNoise(int mode) {
  if (loadingFlag) {
    loadingFlag = false;
    currentPalette = ForestColors_p;
    scale = FOREST_SCALE; colorLoop = 0;
    modeCode = mode;
  }
  fillNoiseLED();
}
void oceanNoise(int mode) {
  if (loadingFlag) {
    loadingFlag = false;
    currentPalette = OceanColors_p;
    scale = OCEAN_SCALE; colorLoop = 0;
    modeCode = mode;
  }

  fillNoiseLED();
}
void plasmaNoise(int mode) {
  if (loadingFlag) {
    loadingFlag = false;
    currentPalette = PartyColors_p;
    scale = PLASMA_SCALE; colorLoop = 1;
    modeCode = mode;
  }
  fillNoiseLED();
}
void cloudNoise(int mode) {
  if (loadingFlag) {
    loadingFlag = false;
    currentPalette = CloudColors_p;
    scale = CLOUD_SCALE; colorLoop = 0;
    modeCode = mode;
  }
  fillNoiseLED();
}
void lavaNoise(int mode) {
  if (loadingFlag) {
    loadingFlag = false;
    currentPalette = LavaColors_p;
    scale = LAVA_SCALE; colorLoop = 0;
    modeCode = mode;
  }
  fillNoiseLED();
}

// ******************* СЛУЖЕБНЫЕ *******************
void fillNoiseLED() {
  uint8_t dataSmoothing = 0;
  if ( speed < 50) {
    dataSmoothing = 200 - (speed * 4);
  }
  for (int i = 0; i < MAX_DIMENSION; i++) {
    int ioffset = scale * i;
    for (int j = 0; j < MAX_DIMENSION; j++) {
      int joffset = scale * j;

      uint8_t data = inoise8(x + ioffset, y + joffset, z);

      data = qsub8(data, 16);
      data = qadd8(data, scale8(data, 39));

      if ( dataSmoothing ) {
        uint8_t olddata = noise[i][j];
        uint8_t newdata = scale8( olddata, dataSmoothing) + scale8( data, 256 - dataSmoothing);
        data = newdata;
      }

      noise[i][j] = data;
    }
  }
  z += speed;

  // apply slow drift to X and Y, just for visual variation.
  x += speed / 8;
  y -= speed / 16;

  for (int i = 0; i < WIDTH; i++) {
    for (int j = 0; j < HEIGHT; j++) {
      uint8_t index = noise[j][i];
      uint8_t bri =   noise[i][j];
      // if this palette is a 'loop', add a slowly-changing base value
      if ( colorLoop) {
        index += ihue;
      }
      // brighten up, as the color palette itself often contains the
      // light/dark dynamic range desired
      if ( bri > 127 ) {
        bri = 255;
      } else {
        bri = dim8_raw( bri * 2);
      }
      CRGB color = ColorFromPalette( currentPalette, index, bri);      
      drawPixelXY(i, j, color);   //leds[getPixelNumber(i, j)] = color;
    }
  }
  ihue += 1;
}

void fillnoise8() {
  for (int i = 0; i < MAX_DIMENSION; i++) {
    int ioffset = scale * i;
    for (int j = 0; j < MAX_DIMENSION; j++) {
      int joffset = scale * j;
      noise[i][j] = inoise8(x + ioffset, y + joffset, z);
    }
  }
  z += speed;
}

#else
void madnessNoise(int mode) {
  modeCode++;
  return;
}
void cloudNoise(int mode) {
  modeCode++;
  return;
}
void lavaNoise(int mode) {
  modeCode++;
  return;
}
void plasmaNoise(int mode) {
  modeCode++;
  return;
}
void rainbowNoise(int mode) {
  modeCode++;
  return;
}
void rainbowStripeNoise(int mode) {
  modeCode++;
  return;
}
void zebraNoise(int mode) {
  modeCode++;
  return;
}
void forestNoise(int mode) {
  modeCode++;
  return;
}
void oceanNoise(int mode) {
  modeCode++;
  return;
}
#endif
