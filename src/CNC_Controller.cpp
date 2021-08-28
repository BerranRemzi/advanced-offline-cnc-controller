#include "Arduino.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Button.h"
#include "LCDMenu.h"
#include "SD.h"

/*******************************************************

  This program will test the LCD panel and the buttons
  Berran Remzi, Septeber 2020

********************************************************/

void ReadInputs(void);
void DrawMenu(void);
void UpdateScreen(void);
void RunFunction(void);

void MainScreen(void);

void Control(void);
void Setting(void);
void CardMenu(void);

void Control_MoveAxis(void);
void Control_GrblMode(void);
void Setting_BaudRate(void);
void Setting_Buzzer(void);
void Setting_MenuMemory(void);

void Control_MoveAxis_Move(void);

void SD_Init();
void printDirectory(File dir, int numTabs);

//void (*pPrintScreen[8])(void);

LiquidCrystal_I2C lcd(0x27, LCD_WIDTH, LCD_HEIGHT);

bool sdReady = false;
void setup()
{
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  lcd.createChar(0, (uint8_t *)arrowChar);  //We create the data to be sent later using lcd.write
  lcd.createChar(1, (uint8_t *)returnChar); //We create the data to be sent later using lcd.write

  Button_Init();
  screen_history[0].menu_function = &MainScreen;
  LCDMenu_Task();
  SD_Init();
}

void loop()
{
  if (IsSinglePressed())
  {
    LCDMenu_Task();
    LCDMenu_Task();
  }
  delay(10);
  ReadInputs();
}

int value = 0;
void MainScreen(void)
{
  START_MENU();
  STATIC_ITEM("Watch");
  SUBMENU("Control", Control);
  SUBMENU("Setting", Setting);
  if(sdReady){
    SUBMENU("Card Menu", CardMenu);
  }else{
    ACTION_ITEM("No Card", SD_Init);
  }
  //EDIT_ITEM_FAST("value", value);
  END_MENU();
}
void Control(void)
{
  START_MENU();
  BACK_ITEM("Back");
  ACTION_ITEM("Auto Home", RunFunction);
  ACTION_ITEM("Unlock Grbl", RunFunction);
  SUBMENU("Move Axis", Control_MoveAxis);
  ACTION_ITEM("Zero position", RunFunction);
  ACTION_ITEM("Spindle Speed", RunFunction);
  ACTION_ITEM("Set Origin", RunFunction);
  ACTION_ITEM("Go To Origin", RunFunction);
  SUBMENU("Grbl Mode", Control_GrblMode);
  ACTION_ITEM("Bed Leveling", RunFunction);
  ACTION_ITEM("Disable Steppers", RunFunction);
  END_MENU();
}

void Setting(void)
{
  START_MENU();
  BACK_ITEM("Back");
  SUBMENU("Baud Rate", Setting_BaudRate);
  SUBMENU("Buzzer", Setting_Buzzer);
  SUBMENU("Menu Memomry", Setting_MenuMemory);
  END_MENU();
}
void CardMenu(void)
{
  START_MENU();
  BACK_ITEM("Back");
  ACTION_ITEM("Refresh", RunFunction);

  File dir = SD.open("/");
  while (true)
  {
    File entry = dir.openNextFile();
    if (!entry)
    {
      // no more files
      break;
    }
    STATIC_ITEM(entry.name());
    entry.close();
  }
  dir.close();

  END_MENU();
}

void Control_MoveAxis(void)
{
  START_MENU();
  BACK_ITEM("Back");
  SUBMENU("Move 10mm", Control_MoveAxis_Move);
  SUBMENU("Move 1mm", Control_MoveAxis_Move);
  SUBMENU("Move 0.1mm", Control_MoveAxis_Move);
  END_MENU();
}
void Control_GrblMode(void)
{
  START_MENU();
  BACK_ITEM("Back");
  ACTION_ITEM("Spindle", RunFunction);
  ACTION_ITEM("Laser", RunFunction);
  END_MENU();
}

void Setting_BaudRate(void)
{
  START_MENU();
  BACK_ITEM("Back");
  ACTION_ITEM("9600", RunFunction);
  ACTION_ITEM("19200", RunFunction);
  ACTION_ITEM("38400", RunFunction);
  ACTION_ITEM("57600", RunFunction);
  ACTION_ITEM("115200", RunFunction);
  END_MENU();
}

void Setting_Buzzer(void)
{
  START_MENU();
  BACK_ITEM("Back");
  ACTION_ITEM("Yes", RunFunction);
  ACTION_ITEM("No", RunFunction);
  END_MENU();
}
void Setting_MenuMemory(void)
{
  START_MENU();
  BACK_ITEM("Back");
  ACTION_ITEM("Yes", RunFunction);
  ACTION_ITEM("No", RunFunction);
  END_MENU();
}

void Control_MoveAxis_Move(void)
{
  START_MENU();
  BACK_ITEM("Back");
  ACTION_ITEM("Move X", RunFunction);
  ACTION_ITEM("Move Y", RunFunction);
  ACTION_ITEM("Move Z", RunFunction);
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
void SD_Init()
{
   if (!SD.begin(10))
  {
    sdReady = false;
  }else{
    sdReady = true;
  }
}

void printDirectory(File dir, int numTabs)
{
  while (true)
  {
    File entry = dir.openNextFile();
    if (!entry)
    {
      // no more files
      break;
    }
    Serial.print(entry.name());
    entry.close();
  }
}