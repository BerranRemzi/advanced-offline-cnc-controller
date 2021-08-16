#ifndef AUTO_BED_LEVELING_H
#define AUTO_BED_LEVELING_H

#include <stdint.h>

void AutoBedLeveling(void);
void ABL_LoadLinePointer(char* line);

bool ABL_ParseNumber(char code, double* pValue);

#endif /* AUTO_BED_LEVELING_H */