#pragma once
#include "Arduino.h"

#define MENU_START() \
  int tempLine = 0;  \
  lineCount = 0;     \
  char buffer[30];

#define MENU_ITEM(TEXT)           \
  lcd.setCursor(0, tempLine);     \
  if (selection == tempLine)      \
  {                               \
    lcd.write(0);                 \
  }                               \
  else                            \
  {                               \
    lcd.print(' ');               \
  }                               \
  sprintf(buffer, "%-12s", TEXT); \
  lcd.print(buffer);              \
  tempLine++;                     \
  lineCount++;

#define MENU_VALUE(TEXT, VAR)             \
  lcd.setCursor(0, tempLine);             \
  if (selection == tempLine)              \
  {                                       \
    lcd.write(0);                         \
  }                                       \
  else                                    \
  {                                       \
    lcd.print(' ');                       \
  }                                       \
  sprintf(buffer, "%-12s%7d", TEXT, VAR); \
  lcd.print(buffer);                      \
  tempLine++;                             \
  lineCount++;

#define MENU_FOLDER(TEXT, FUNC) \
  lcd.setCursor(19, tempLine);  \
  lcd.write(1);                 \
  MENU_ITEM(TEXT);              \
  FUNC()

#define MENU_FUNCTION(TEXT, FUNC) \
  lcd.setCursor(19, tempLine);    \
  MENU_ITEM(TEXT);                \
  FUNC()

#define MENU_BACK(TEXT) MENU_FOLDER(TEXT, Back);
#define MENU_END()

extern int selection;
extern int lineCount;

void Menu_Up();
void Menu_Down();