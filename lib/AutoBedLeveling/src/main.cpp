#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "./AutoBedLeveling.h"

int main(void){
	printf("int main(void)\n");
	AutoBedLeveling();
	
	const char filename[] = "bottom_contour.nc";
    FILE *fp = fopen(filename, "r");

    if (fp == nullptr)
    {
        printf("Error: could not open file %s", filename);
        return 1;
    }

    // reading line by line, max 256 bytes
    const unsigned MAX_LENGTH = 256;
    char buffer[MAX_LENGTH];
    ABL_LoadLinePointer(buffer);

    while (fgets(buffer, MAX_LENGTH, fp)){
        printf("X=%.3f ", parseNumber('X', 0));
        printf("Y=%.3f ", parseNumber('Y', 0));
        printf("Z=%.3f\n", parseNumber('Z', 0));
    }
        
    // close the file
    fclose(fp);
	
	printf("Press ENTER to exit...");
	getchar();
	return 0;
}