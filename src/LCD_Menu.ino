//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

#define MENU_START()        \
  int tempLine = 0;         \
  static int selection = 0; \
  int lineCount = 0;        \
  char buffer[30];          \
  char arrow = ' ';

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

const uint8_t arrow[8] = {0x00, 0x04, 0x06, 0x1f, 0x06, 0x04, 0x00}; //Send 0,4,6,1F,6,4,0 for the arrow
const uint8_t newLine[] = {0x01, 0x01, 0x01, 0x05, 0x09, 0x1F, 0x08, 0x04};

void setup()
{
  lcd.init(); // initialize the lcd
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, (uint8_t *)arrow);
  lcd.createChar(1, (uint8_t *)newLine);
}

void loop()
{
  MENU_START();
  MENU_BACK("Back");
  MENU_FOLDER("Folder", Func1);
  MENU_VALUE("Value:", selection * 100);
  MENU_FUNCTION("Function", Func1);
  MENU_END();
  if (selection < lineCount - 1)
  {
    selection++;
  }
  else
  {
    selection = 0;
  }
  delay(1000);
}

void Func1()
{
}
void Back()
{
}
