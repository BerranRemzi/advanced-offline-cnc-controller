#include "Menu.h"

int selection = 0;
int lineCount = 0;

void Menu_Down()
{
    if (selection < lineCount - 1)
    {
        selection++;
    }
}
void Menu_Up()
{
    if (selection > 0)
    {
        selection--;
    }
}