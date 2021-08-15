#include <stdio.h>
#include "AutoBedLeveling.h"

int main(void){
	printf("int main(void)\n");
	AutoBedLeveling();
	
	char filename[] = "bottom_contour.nc";
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Error: could not open file %s", filename);
        return 1;
    }

    // reading line by line, max 256 bytes
    const unsigned MAX_LENGTH = 256;
    char buffer[MAX_LENGTH];

    while (fgets(buffer, MAX_LENGTH, fp))
        printf("%s", buffer);

    // close the file
    fclose(fp);
	
	printf("Press ENTER to exit...");
	getchar();
	return 0;
}