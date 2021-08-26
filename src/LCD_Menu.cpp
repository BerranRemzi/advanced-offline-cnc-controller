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
void SubMenu_Card(void);
void Menu_TestScreen(void);
void RunFunction(void);
void SubMenu_Prepare(void);
void SubMenu_Control(void);

void SD_Init();
void printDirectory(File dir, int numTabs);

void (*pPrintScreen[8])(void);

LiquidCrystal_I2C lcd(0x27, LCD_WIDTH, LCD_HEIGHT);

void setup()
{
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  lcd.createChar(0, (uint8_t *)arrowChar);  //We create the data to be sent later using lcd.write
  lcd.createChar(1, (uint8_t *)returnChar); //We create the data to be sent later using lcd.write

  Button_Init();
  pPrintScreen[menuDepth] = &Menu_TestScreen;
  UpdateScreen();
  SD_Init();
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
  STATIC_ITEM("Watch");
  SUBMENU("Prepare", SubMenu_Prepare);
  SUBMENU("Control", SubMenu_Control);
  SUBMENU("Card Menu", SubMenu_Card);
  //EDIT_ITEM_FAST("value", value);
  END_MENU();
}
void SubMenu_Card(void)
{
  START_MENU();
  BACK_ITEM("<<BACK");
  ACTION_ITEM("Refresh", RunFunction);
  END_MENU();
}
void SubMenu_Prepare(void)
{
  START_MENU();
  BACK_ITEM("<<BACK");
  ACTION_ITEM("Move Axis", RunFunction);
  ACTION_ITEM("Auto Home", RunFunction);
  ACTION_ITEM("Auto Home X", RunFunction);
  ACTION_ITEM("Auto Home Y", RunFunction);
  ACTION_ITEM("Auto Home Z", RunFunction);
  ACTION_ITEM("Bed Leveling", RunFunction);
  ACTION_ITEM("Disable Steppers", RunFunction);
  ACTION_ITEM("Set Home Offsets", RunFunction);
  END_MENU();
}

void SubMenu_Control(void)
{
  START_MENU();
  BACK_ITEM("<<BACK");
  ACTION_ITEM("Motion", RunFunction);
  ACTION_ITEM("Store Memory", RunFunction);
  ACTION_ITEM("Load Memomry", RunFunction);
  ACTION_ITEM("Restore Failsafe", RunFunction);
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
void SD_Init(){
  Serial.print("Initializing SD card...");

  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  root = SD.open("/");

  printDirectory(root, 0);

  Serial.println("done!");
}

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}