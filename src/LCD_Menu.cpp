#include "Arduino.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Button.h"
#include "LCDMenu.h"

/*******************************************************

  This program will test the LCD panel and the buttons
  Berran Remzi, Septeber 2020

********************************************************/

void ReadInputs(void);
void DrawMenu(void);
void UpdateScreen(void);
void SubMenu_1(void);
void Menu_TestScreen(void);
void RunFunction(void);

void (*pPrintScreen[8])(void);

LiquidCrystal_I2C lcd(0x27, LCD_WIDTH, LCD_HEIGHT);

void setup()
{
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  lcd.createChar(0, (uint8_t *)arrowChar);  //We create the data to be sent later using lcd.write
  lcd.createChar(1, (uint8_t *)returnChar); //We create the data to be sent later using lcd.write
  
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);

  pPrintScreen[menuDepth] = &Menu_TestScreen;
  UpdateScreen();
}

void loop()
{

  //DrawMenu2();
  if (IsSinglePressed())
  {
    UpdateScreen();
    UpdateScreen();
  }
  delay(10);
  ReadInputs();
}

void UpdateScreen(void)
{
  clear_screen();
  if (*pPrintScreen[menuDepth] != NULL)
  {
    pPrintScreen[menuDepth]();
  }
}
int value = 0;
void Menu_TestScreen(void)
{
  START_MENU();
  SUBMENU("SubMenu_1", SubMenu_1);
  EDIT_ITEM_FAST("value", value);
  ACTION_ITEM("RunFunction", RunFunction);
  STATIC_ITEM("Bernar 1");
  STATIC_ITEM("Berran 2");
  STATIC_ITEM("Gulcan 3");
  STATIC_ITEM("Static text 4");
  STATIC_ITEM("Static text 5");
  STATIC_ITEM("Static text 6");
  STATIC_ITEM("Static text 7");
  STATIC_ITEM("Static text 8");
  END_MENU();
}
void SubMenu_1(void)
{
  START_MENU();
  BACK_ITEM("<<BACK");
  STATIC_ITEM("Static text 1");
  END_MENU();
}

void RunFunction(void)
{
  Serial.println(millis());
  Serial.println(VALUE_FORMAT(LCD_WIDTH));
}

void ReadInputs(void)
{
  if (IsPressed(DOWN_PIN))
  {
    LCDMenu_Down();
  }
  if (IsPressed(UP_PIN))
  {
    LCDMenu_Up();
  }
  if (IsPressed(RIGHT_PIN))
  {
    LCDMenu_Select();
  }
}