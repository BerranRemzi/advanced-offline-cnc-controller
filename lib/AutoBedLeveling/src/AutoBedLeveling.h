#ifndef AUTO_BED_LEVELING_H
#define AUTO_BED_LEVELING_H

#include <stdint.h>

typedef struct{
    double x;
    double y;
    double z;
} Coordinates_t;

typedef struct {
    int x;
    int y;
} Index_t;

void AutoBedLeveling(void);
void ABL_LoadLinePointer(char* line);

bool ABL_ParseNumber(char code, double* pValue);

void line(float newx, float newy);
float bilinearInterpolation(float q11, float q12, float q21, float q22, float x1, float x2, float y1, float y2, float x, float y);
double linearInterpolation(double xp, double x0, double y0,double x1,double y1);
Index_t getGridIndex(Coordinates_t coordinates);
double GetX(double yp, Coordinates_t p0, Coordinates_t p1);
double GetY(double xp, Coordinates_t p0, Coordinates_t p1);
bool getIntersections(char* buffer, Coordinates_t current);

#endif /* AUTO_BED_LEVELING_H */
