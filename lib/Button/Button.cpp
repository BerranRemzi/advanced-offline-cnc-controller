#include "Button.h"

typedef struct Button
{
  int8_t pin;
  bool lastState;
};

Button btn[4] = {
  {5},
  {2},
  {4},
  {2}
};

bool IsPressed(int8_t _num)
{
  bool returnValue = false;
  bool currentState = digitalRead(btn[_num].pin);
  if (currentState == BTN_PRESSED && currentState != btn[_num].lastState){
    returnValue = true;
  }
  btn[_num].lastState = currentState;
  return returnValue;
}