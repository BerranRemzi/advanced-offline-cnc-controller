#ifndef AUTO_BED_LEVELING_H
#define AUTO_BED_LEVELING_H

#include <stdint.h>

typedef enum {
    LINE_INIT,
    LINE_BUSY,
    LINE_DONE
}ComputeLine_t;

typedef struct {
    double x;
    double y;
    double z;
} Coordinates_t;

typedef struct {
    int32_t x;
    int32_t y;
} CoordinatesLong_t;

typedef struct {
    int16_t x;
    int16_t y;
} Index_t;

#ifdef __cplusplus
extern "C" {
#endif

    void AutoBedLeveling(void);
    void ABL_LoadLinePointer(char* line);

    bool ABL_ParseNumber(char code, double* pValue);

    void line(double newx, double newy);
    double bilinearInterpolation(double q11, double q12, double q21, double q22, double x1, double x2, double y1, double y2, double x, double y);
    double linearInterpolation(double xp, double x0, double y0, double x1, double y1);
    Index_t getGridIndex(Coordinates_t  const* coordinates);
    double GetX(double xp, Coordinates_t const* p0, Coordinates_t const* p1);
    double GetY(double xp, Coordinates_t const* p0, Coordinates_t const* p1);
    bool getIntersections(char* buffer, Coordinates_t current);
    bool ComputeLine(double newx, double newy);

#ifdef __cplusplus
}
#endif

#endif /* AUTO_BED_LEVELING_H */
