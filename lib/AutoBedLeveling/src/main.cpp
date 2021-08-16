#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "./AutoBedLeveling.h"

int main(void) {
    //printf("int main(void)\n");
    //AutoBedLeveling();

    const char filename[] = "bottom_contour.nc";
    FILE* fp = fopen(filename, "r");

    if (fp == nullptr)
    {
        printf("Error: could not open file %s", filename);
        return 1;
    }

    // reading line by line, max 256 bytes
    const unsigned MAX_LENGTH = 256;
    char buffer[MAX_LENGTH];
    ABL_LoadLinePointer(buffer);

    while (fgets(buffer, MAX_LENGTH, fp)) {
        double value = 0;
        if (true == ABL_ParseNumber('G', &value)) {
            printf("G%02d ", (int) value);

            if (true == ABL_ParseNumber('F', &value)) {
                printf("F%d ", (int) value);
            }
            if (true == ABL_ParseNumber('X', &value)) {
                printf("X%.3f ", value);
            }
            if (true == ABL_ParseNumber('Y', &value)) {
                printf("Y%.3f ", value);
            }
            if (true == ABL_ParseNumber('Z', &value)) {
                printf("Z%.3f", value);
            }
            printf("\n");
        }
        else if (true == ABL_ParseNumber('M', &value)) {
            printf("M%02d ", (int) value);

            if (true == ABL_ParseNumber('S', &value)) {
                printf("S%d ", (int) value);
            }
            printf("\n");
        }
    }

    // close the file
    fclose(fp);

    line(9, 25);
    float z = bilinearInterpolation(0,0,5,5,0,10,0,10,5,5);
    printf("%f",z);

    printf("Press ENTER to exit...");
    getchar();
    return 0;
}