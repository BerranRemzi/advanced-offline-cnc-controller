#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "AutoBedLeveling.h"
#include "Test.h"

#define MAX_LENGTH 255

int main(void) {
    const char filename[] = "bottom_contour.nc";
    FILE* fp = fopen(filename, "r");

    if (fp == nullptr)
    {
        printf("Error: could not open file %s", filename);
        return 1;
    }

    char buffer[MAX_LENGTH];
    char printBuffer[MAX_LENGTH];
    ABL_LoadLinePointer(buffer);
    int lineCounter = 0;
    Coordinates_t pos = {0.0, 0.0, 0.0};
    while (fgets(buffer, MAX_LENGTH, fp)) {
        double value = 0;
        char temp[MAX_LENGTH];
        printBuffer[0] = '\0';

        if (true == ABL_ParseNumber('G', &value)) {
            sprintf(temp,"G%02d", (int) value);
            strcat(printBuffer, temp);
            if (true == ABL_ParseNumber('F', &value)) {
                sprintf(temp," F%d", (int) value);
                strcat(printBuffer, temp);
            }
            if (true == ABL_ParseNumber('X', &pos.x)) {
                sprintf(temp," X%.2f", pos.x);
                strcat(printBuffer, temp);
            }
            if (true == ABL_ParseNumber('Y', &pos.y)) {
                sprintf(temp," Y%.2f", pos.y);
                strcat(printBuffer, temp);
            }
            if (true == ABL_ParseNumber('Z', &pos.z)) {
                sprintf(temp," Z%.2f", pos.z);
                strcat(printBuffer, temp);
            }
            //printf("%0.2f, %0.2f, %0.2f\n", pos.x,pos.y,pos.z);
        }
        else if (true == ABL_ParseNumber('M', &value)) {
            sprintf(temp,"M%02d", (int) value);
            strcat(printBuffer, temp);

            if (true == ABL_ParseNumber('S', &value)) {
                sprintf(temp," S%d", (int) value);
                strcat(printBuffer, temp);
            }
        }
        #ifndef DEBUG
        if(strlen(printBuffer)){
            //printf("%-30s",testData[lineCounter]);
            printf("%-30s",printBuffer);
            if(strcmp(printBuffer, testData[lineCounter])){
                printf(" - NOK\n");
            }else{
                printf(" - OK\n");
            }
            lineCounter++;
        }
        #endif
    }

    // close the file
    fclose(fp);

    //line(9, 25);
    //float z = bilinearInterpolation(0,0,55,09,5,0,10,0,10,5,5);
    printf("%0.2f\n",linearInterpolation(20, 27, 8, 0, 0));
    printf("%0.2f\n",linearInterpolation(10, 27, 8, 0, 0));
    //printf("Press ENTER to exit...");
    //getchar();
    Coordinates_t coordtest;
    printf("%d\n",getIntersections(nullptr, coordtest));

    line(0,0);
    line(7,28);
    line(2,7);
    line(28,2);
    return 0;
}
