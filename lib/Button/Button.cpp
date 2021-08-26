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

static bool isSinglePressed = false;

bool IsPressed(int8_t _num)
{
  bool returnValue = false;
  bool currentState = digitalRead(btn[_num].pin);
  if (currentState == BTN_PRESSED && currentState != btn[_num].lastState)
  {
    returnValue = true;
    isSinglePressed = true;
  }
  btn[_num].lastState = currentState;
  return returnValue;
}

bool IsSinglePressed()
{
  bool returnValue = false;

  if (true == isSinglePressed)
  {
    returnValue = true;
    isSinglePressed = false;
  }

  return returnValue;
}