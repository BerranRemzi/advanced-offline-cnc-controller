#ifndef AUTO_BED_LEVELING_H
#define AUTO_BED_LEVELING_H

#include <stdint.h>

void AutoBedLeveling(void);
void ABL_LoadLinePointer(char* line);

bool ABL_ParseNumber(char code, double* pValue);

void line(float newx, float newy);
float bilinearInterpolation(float q11, float q12, float q21, float q22, float x1, float x2, float y1, float y2, float x, float y);

#endif /* AUTO_BED_LEVELING_H */