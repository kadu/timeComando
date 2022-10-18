#pragma once
#include <WS2812FX.h>
#include "settings.h"

void ledRespira(WS2812FX *ws2812fx);
void ledConfigura(WS2812FX *ws2812fx);
void ledStrobe(WS2812FX *ws2812fx);
void ledMultiStrobe(WS2812FX *ws2812fx);
void ledPerdeu(WS2812FX *ws2812fx);
void ledStop(WS2812FX *ws2812fx);
void setupLeds(WS2812FX *ws2812fx);
void loopLeds(WS2812FX *ws2812fx);
