#ifndef AUTO_BED_LEVELING_H
#define AUTO_BED_LEVELING_H

#include <stdint.h>

typedef enum
{
    LINE_INIT,
    LINE_BUSY,
    LINE_DONE
} ComputeLine_t;

typedef struct
{
    double x;
    double y;
    double z;
} Coord_t;

typedef struct
{
    int32_t x;
    int32_t y;
    int32_t z;
} CoordLong_t;

typedef struct
{
    int16_t x;
    int16_t y;
} Index_t;

#ifdef __cplusplus
extern "C"
{
#endif

    void AutoBedLeveling(void);
    void ABL_LoadLinePointer(char *line);

    bool ABL_ParseNumber(char code, double *pValue);

    double bilinearInterpolation(double q11, double q12, double q21, double q22, double x1, double x2, double y1, double y2, double x, double y);
    Index_t getGridIndex(const CoordLong_t *const coordinates);
    bool ComputeLine(void);
    bool ABL_ComputeLine(char lineBuffer[]);
#ifdef __cplusplus
}
#endif

#endif /* AUTO_BED_LEVELING_H */
