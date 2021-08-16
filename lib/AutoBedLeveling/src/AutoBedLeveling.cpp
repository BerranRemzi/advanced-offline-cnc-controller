#include "./AutoBedLeveling.h"
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

char* serialBuffer;
int   soFar = 255;            // how much is in the buffer
double posX;
double posY;      // location

void AutoBedLeveling(void) {
    printf("void AutoBedLeveling(void)\n");
}

void ABL_LoadLinePointer(char* line) {
    serialBuffer = line;
}

/**
 * Look for character /code/ in the buffer and read the float that immediately follows it.
 * @return the value found.  If nothing is found, /val/ is returned.
 * @input code the character to look for.
 * @input val the return value if /code/ is not found.
 **/
bool ABL_ParseNumber(char code, double* pValue) {
    char* ptr = serialBuffer;  // start at the beginning of buffer
    while ((long) ptr > 1 && (*ptr) && (long) ptr < (long) serialBuffer + soFar) {  // walk to the end
        if (*ptr == code) {  // if you find code on your walk,
            *pValue = atof(ptr + 1);  // convert the digits that follow into a float and return it
            return true;
        }
        ptr = strchr(ptr, ' ') + 1;  // take a step from here to the letter after the next space
    }
    return false;  // end reached, nothing found, return default val.
}