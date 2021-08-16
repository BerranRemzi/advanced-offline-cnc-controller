#include "./AutoBedLeveling.h"
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

char * serialBuffer;
int   sofar = 255;            // how much is in the buffer
double px, py;      // location

void AutoBedLeveling(void){
	printf("void AutoBedLeveling(void)\n");
}

void ABL_LoadLinePointer(char * line){
    serialBuffer = line;
}

int ABL_GetPosition(char axis){
	int position = -1;
	//char temp[32];
	for(int i = 0; serialBuffer[i] != '\0';i++){
		if (serialBuffer[i] == axis) {
			position = 0;
		}
	}
	return position;
}

/**
 * Look for character /code/ in the buffer and read the float that immediately follows it.
 * @return the value found.  If nothing is found, /val/ is returned.
 * @input code the character to look for.
 * @input val the return value if /code/ is not found.
 **/
double parseNumber(char code, float val) {
    char* ptr = serialBuffer;  // start at the beginning of buffer
    while ((long) ptr > 1 && (*ptr) && (long) ptr < (long) serialBuffer + sofar) {  // walk to the end
        if (*ptr == code) {  // if you find code on your walk,
            return atof(ptr + 1);  // convert the digits that follow into a float and return it
        }
        ptr = strchr(ptr, ' ') + 1;  // take a step from here to the letter after the next space
    }
    return val;  // end reached, nothing found, return default val.
}