#ifndef AUTO_BED_LEVELING_H
#define AUTO_BED_LEVELING_H

#include <stdint.h>

void AutoBedLeveling(void);
void ABL_LoadLinePointer(char * line);
int ABL_GetPosition(char axis);

#endif /* AUTO_BED_LEVELING_H */