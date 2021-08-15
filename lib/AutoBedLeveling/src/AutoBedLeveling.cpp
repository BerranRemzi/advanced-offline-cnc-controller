#include "./AutoBedLeveling.h"
#include <stdio.h>

char * lastLine;

void AutoBedLeveling(void){
	printf("void AutoBedLeveling(void)\n");
}

void ABL_LoadLinePointer(char * line){
	lastLine = line;
}

int ABL_GetPosition(char axis){
	(void)axis;
	return -1;
}