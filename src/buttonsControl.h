#pragma once
#include <OneButton.h>

void setupButtonsControl(OneButton *button1,
  OneButton *button2,
  callbackFunction click1,
  callbackFunction longPress1,
  callbackFunction click2,
  callbackFunction longPress2,
  callbackFunction duringLongPress1,
  callbackFunction duringLongPress2);

void loopButtonsControl(OneButton *button1, OneButton *button2);