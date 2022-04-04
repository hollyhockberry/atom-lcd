// Copyright (c) 2022 Inaba
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#include <M5Atom.h>

#define LGFX_USE_V1
#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_SSD1306 _panel;
  lgfx::Bus_I2C _bus;

 public:
  LGFX(void) {
    {
      auto cfg = _bus.config();
      cfg.i2c_port = 1;
      cfg.freq_write = 400000;
      cfg.freq_read = 400000;
      cfg.pin_sda = 25;
      cfg.pin_scl = 21;
      cfg.i2c_addr = 0x3c;

      _bus.config(cfg);
      _panel.setBus(&_bus);
    }
    {
      auto cfg = _panel.config();
      cfg.memory_width = 128;
      cfg.memory_height = 64;
      _panel.config(cfg);
    }
    setPanel(&_panel);
  }
} display;

static LGFX_Sprite canvas(&display);
static LGFX_Sprite eye(&canvas);
static LGFX_Sprite eye2(&canvas);

void setup() {
  M5.begin();
  display.init();
  canvas.createSprite(display.width(), display.height());
  eye.createSprite(64, 64);
  eye.fillCircle(32, 32, 28, TFT_WHITE);
  eye.fillCircle(32, 32, 22, TFT_BLACK);
  eye.fillCircle(18, 32, 14, TFT_WHITE);

  eye2.createSprite(64, 64);
  eye2.fillCircle(32, 32, 28, TFT_WHITE);
  eye2.fillCircle(32, 32, 22, TFT_BLACK);
  eye2.fillRect(4, 29, 28 * 2, 6, TFT_WHITE);
}

bool ensleep() {
  M5.update();
  if (M5.Btn.isPressed()) {
    canvas.fillScreen(TFT_BLACK);
    for (auto e = 0; e < 2; ++e) {
      eye2.pushSprite(e * 64, 0);
    }
    canvas.pushSprite(0, 0);
    while (!M5.Btn.wasReleased()) {
      M5.update();
      ::delay(1);
    }
    return true;
  }
  return false;
}

void loop() {
  static int i = 0;
  canvas.fillScreen(TFT_BLACK);
  i = 1 - i;
  eye.pushRotateZoomWithAA(32, 32, i * 180.f, 1.f, 1.f);
  eye.pushRotateZoomWithAA(32+ 64, 32, i * 180.f, 1.f, 1.f);
  canvas.pushSprite(0, 0);
  ::delay(500);
  const auto now = ::millis();
  while ((::millis() - now) < 500) {
    if (ensleep()) break;
    ::delay(1);
  }
}
