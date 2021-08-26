#pragma once
#include "Arduino.h"

#define UP_PIN 0
#define DOWN_PIN 1
#define RIGHT_PIN 2
#define LEFT_PIN 3

#define BTN_PRESSED LOW

void Button_Init();
bool IsPressed(int8_t _num);
bool IsSinglePressed();
