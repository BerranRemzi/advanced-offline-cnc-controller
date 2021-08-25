#include "Arduino.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Button.h>
#include <Task.h>
#include <Menu.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

const uint8_t arrow[8] = {0x00, 0x04, 0x06, 0x1f, 0x06, 0x04, 0x00};
const uint8_t newLine[] = {0x01, 0x01, 0x01, 0x05, 0x09, 0x1F, 0x08, 0x04};

void Func1()
{
}
void Back()
{
}

void MenuMovement()
{
  if (IsPressed(DOWN_PIN))
  {
    Menu_Down();
  }
  if (IsPressed(UP_PIN))
  {
    Menu_Up();
  }
}
void setup()
{
  lcd.init(); // initialize the lcd
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, (uint8_t *)arrow);
  lcd.createChar(1, (uint8_t *)newLine);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
}
void MainMenu()
{
  MENU_START();
  MENU_BACK("Back");
  MENU_FOLDER("Folder", Func1);
  MENU_VALUE("Value:", selection * 100);
  MENU_FUNCTION("Function", Func1);
  MENU_END();
}
void loop()
{
  TASK(MENU, MainMenu(), 100);
  TASK(BUTTONS, MenuMovement(), 50);
}
