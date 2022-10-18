#include <Arduino.h>
#include <WS2812FX.h>
#include "settings.h"
#include "ledControl.h"

void ledRespira(WS2812FX *ws2812fx) {
  ws2812fx->setColor(WHITE);
  ws2812fx->setMode(FX_MODE_BREATH);
}

void ledConfigura(WS2812FX *ws2812fx) {
  ws2812fx->setColor(YELLOW);
  ws2812fx->setMode(FX_MODE_SCAN);
}

void ledStrobe(WS2812FX *ws2812fx) {
  ws2812fx->setColor(WHITE);
  ws2812fx->setMode(FX_MODE_STROBE);
}

void ledMultiStrobe(WS2812FX *ws2812fx) {
  ws2812fx->setColor(WHITE);
  ws2812fx->setMode(FX_MODE_MULTI_STROBE);
}

void ledPerdeu(WS2812FX *ws2812fx) {
  ws2812fx->setColor(RED);
  ws2812fx->setMode(FX_MODE_STATIC);
}

void ledStop(WS2812FX *ws2812fx) {
  ws2812fx->setColor(BLACK);
  ws2812fx->setMode(FX_MODE_STATIC);
}

void setupLeds(WS2812FX *ws2812fx) {
  ws2812fx->init();
  ws2812fx->setBrightness(50);
  ws2812fx->setSpeed(1000);
  ws2812fx->setColor(GREEN);
  ws2812fx->setMode(FX_MODE_STATIC);
  ws2812fx->start();
}

void loopLeds(WS2812FX *ws2812fx) {
  ws2812fx->service();
}