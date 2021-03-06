#include "main.h"
#include "AutoBedLeveling.h"
#include "Test.h"

#define MAX_LENGTH 255

void Test1(const char fileName[]);
void Test2();
void Test3();
void Test4(const char fileName[]);

char buffer[MAX_LENGTH] = {};

int main(void) {
    //Test1("bottom_contour.nc"); //Parse G-code
    //Test2(); //Find intersection with lin3()
    //Test3(); //Find intersection with ComputeLine()
    Test4("bottom_contour.nc"); //
    return 0;
}

void Test1(const char fileName[]) {
    FILE* fp = fopen(fileName, "r");

    if (fp == NULL)
    {
        printf("Error: could not open file %s", fileName);
    }

    char printBuffer[MAX_LENGTH];
    ABL_LoadLinePointer(buffer);
    int16_t lineCounter = 0;
    Coord_t pos = { 0.0, 0.0, 0.0 };
    fgets(buffer, 4, fp);
    while (fgets(buffer, MAX_LENGTH, fp) != NULL) {
        double value = 0;
        char temp[MAX_LENGTH];
        printBuffer[0] = '\0';
        bool isCommandAvailable = false;

        if (true == ABL_ParseNumber('G', &value)) {
            isCommandAvailable = true;
            sprintf(temp, "G%02d ", (int) value);
            strcat(printBuffer, temp);
        }
        if (true == ABL_ParseNumber('F', &value)) {
            isCommandAvailable = true;
            sprintf(temp, "F%d ", (int) value);
            strcat(printBuffer, temp);
        }
        if (true == ABL_ParseNumber('T', &value)) {
            isCommandAvailable = true;
            sprintf(temp, "T%d ", (int) value);
            strcat(printBuffer, temp);
        }
        if (true == ABL_ParseNumber('M', &value)) {
            isCommandAvailable = true;
            sprintf(temp, "M%02d ", (int) value);
            strcat(printBuffer, temp);
        }
        if (true == ABL_ParseNumber('S', &value)) {
            isCommandAvailable = true;
            sprintf(temp, "S%d", (int) value);
            strcat(printBuffer, temp);
        }
        if (true == ABL_ParseNumber('X', &pos.x)) {
            isCommandAvailable = true;
            sprintf(temp, "X%.3f ", pos.x);
            strcat(printBuffer, temp);
        }
        if (true == ABL_ParseNumber('Y', &pos.y)) {
            isCommandAvailable = true;
            sprintf(temp, "Y%.3f ", pos.y);
            strcat(printBuffer, temp);
        }
        if (true == ABL_ParseNumber('Z', &pos.z)) {
            isCommandAvailable = true;
            sprintf(temp, "Z%.3f ", pos.z);
            strcat(printBuffer, temp);
        }
        if (false == isCommandAvailable) {
            buffer[(int) strlen(buffer) - 1] = '\0';
            strcat(printBuffer, buffer);
        }
#ifndef DEBUG
        if (strlen(printBuffer)) {
            //printf("%-30s",testData[lineCounter]);
            printf("%-35s", printBuffer);
#if 0
            if (lineCounter < 255 && strcmp(printBuffer, testData[lineCounter])) {
                printf(" - NOK\n");
            }
            else {
                printf(" - OK\n");
            }
            lineCounter++;
#else
            printf("\n");
#endif
        }
#endif
    }

    // close the file
    fclose(fp);
}

void Test2() {
    printf("line();\n");
    //line(0, 0);
    //line(7, 28);
    //line(2, 7);
    //line(28, 2);
    //line(0, 0);
}
void Test3() {
    printf("ComputeLine();\n");
    //while (!ComputeLine(0, 0, 0));
    //while (!ComputeLine(7, 28, 0));
    //while (!ComputeLine(2, 7, 0));
    //while (!ComputeLine(28, 2, 0));
    //while (!ComputeLine(0, 0, 0));
}
void Test4(const char _filename[]) {
    File myFile;
    myFile = SD.open(_filename);
    if (myFile) {
        while (myFile.available()) {
            char* lineBuffer = myFile.readUntil('\n');
            while (!ABL_ComputeLine(lineBuffer));
        }
        // close the file:
        myFile.close();
    }
}