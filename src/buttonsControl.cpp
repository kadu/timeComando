#include <OneButton.h>
#include "buttonsControl.h"

void setupButtonsControl(OneButton *button1,
  OneButton *button2,
  callbackFunction click1,
  callbackFunction longPress1,
  callbackFunction click2,
  callbackFunction longPress2) {

  button1->attachClick(click1);
  button1->attachLongPressStart(longPress1);
  button2->attachClick(click2);
  button2->attachLongPressStart(longPress2);

}

void loopButtonsControl(OneButton *button1, OneButton *button2) {
  button1->tick();
  button2->tick();
}