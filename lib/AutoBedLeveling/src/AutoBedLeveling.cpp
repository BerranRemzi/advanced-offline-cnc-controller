#define _CRT_SECURE_NO_WARNINGS

#include "AutoBedLeveling.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define RATIO 10000.0
#define GRID_SIZE_MM 10
#define GRID_POINTS_X 3
#define GRID_POINTS_Y 3

static char* serialBuffer;

static Coord_t nextPos = { 0.0, 0.0, 0.0 };
static int16_t soFar = 255; // how much is in the buffer

void AutoBedLeveling(void)
{
    printf("void AutoBedLeveling(void)\n");
}

void ABL_LoadLinePointer(char* line)
{
    serialBuffer = line;
}

/**
 * Look for character /code/ in the buffer and read the double that immediately follows it.
 * @return the value found.  If nothing is found, /val/ is returned.
 * @param code the character to look for.
 * @param val the return value if /code/ is not found.
 **/
bool ABL_ParseNumber(char code, double* pValue)
{
    char* ptr = serialBuffer; // start at the beginning of buffer
    while ((int32_t) ptr > 1 && (*ptr) && (int32_t) ptr < (int32_t) serialBuffer + soFar)
    { // walk to the end
        if (*ptr < 'A' || *ptr > 'Z')
        {
            break;
        }
        if (*ptr == code)
        {                            // if you find code on your walk,
            *pValue = atof(ptr + 1); // convert the digits that follow into a double and return it
            return true;
        }
        // ptr = strchr(ptr, ' ') + 1; // take a step from here to the letter after the next space
        while (*ptr && !isupper(*++ptr))
            ;
    }
    return false; // end reached, nothing found, return default val.
}

double bilinearInterpolation(double q11, double q12, double q21, double q22, double x1, double x2, double y1, double y2, double x, double y)
{
    double x2x1 = x2 - x1;
    double y2y1 = y2 - y1;
    double x2x = x2 - x;
    double y2y = y2 - y;
    double yy1 = y - y1;
    double xx1 = x - x1;
    return 1.0 / (x2x1 * y2y1) * (q11 * x2x * y2y + q21 * xx1 * y2y + q12 * x2x * yy1 + q22 * xx1 * yy1);
}

Index_t getGridIndex(const CoordLong_t* const coord)
{
    Index_t temp;
    temp.x = (int16_t) (coord->x / (GRID_SIZE_MM * RATIO));
    temp.y = (int16_t) (coord->y / (GRID_SIZE_MM * RATIO));

    if (temp.x < 0)
        temp.x = 0;
    else if (temp.x > GRID_POINTS_X)
        temp.x = GRID_POINTS_X;

    if (temp.y < 0)
        temp.y = 0;
    else if (temp.y > GRID_POINTS_Y)
        temp.y = GRID_POINTS_Y;

    return temp;
}

bool ComputeLine(void)
{
    bool returnValue = false;

    static ComputeLine_t state = LINE_INIT;

    static Index_t prevIndex;
    static Index_t currentIndex;

    static int32_t i;

    static CoordLong_t _currentPos;
    static CoordLong_t _nextPos;
    
    static Coord_t currentPos;

    static int32_t dx;
    static int32_t dy;

    static int8_t dirx;
    static int8_t diry;

    static int32_t over = 0;

    switch (state)
    {
    case LINE_INIT:
        _currentPos.x = (int32_t) (currentPos.x * RATIO);
        _currentPos.y = (int32_t) (currentPos.y * RATIO);

        _nextPos.x = (int32_t) (nextPos.x * RATIO);
        _nextPos.y = (int32_t) (nextPos.y * RATIO);

        dx = (_nextPos.x - _currentPos.x);
        dy = (_nextPos.y - _currentPos.y);
        dirx = dx > 0 ? 1 : -1;
        diry = dy > 0 ? 1 : -1;

        dx = abs(dx);
        dy = abs(dy);
        i = 0;

        if (dx > dy)
        {
            over = dx / 2;
        }
        else
        {
            over = dy / 2;
        }

        state = LINE_BUSY;

        break;
    case LINE_BUSY:
        if (dx > dy)
        {
            for (; i < dx; ++i)
            {
                _currentPos.x += dirx;
                over += dy;
                if (over >= dx)
                {
                    over -= dx;
                    _currentPos.y += diry;
                }

                currentIndex = getGridIndex(&_currentPos);

                if ((prevIndex.x != currentIndex.x) || (prevIndex.y != currentIndex.y))
                {
                    currentPos.x = _currentPos.x / RATIO;
                    currentPos.y = _currentPos.y / RATIO;
                    printf("X%.3f Y%.3f\n", currentPos.x, currentPos.y);
                    prevIndex = currentIndex;
                    break;
                }
            }
            if (dx == i)
            {
                currentPos.x = nextPos.x;
                currentPos.y = nextPos.y;
                state = LINE_DONE;
            }
        }
        else
        {
            for (; i < dy; ++i)
            {
                _currentPos.y += diry;
                over += dx;
                if (over >= dy)
                {
                    over -= dy;
                    _currentPos.x += dirx;
                }

                currentIndex = getGridIndex(&_currentPos);

                if ((prevIndex.x != currentIndex.x) || (prevIndex.y != currentIndex.y))
                {
                    currentPos.x = _currentPos.x / RATIO;
                    currentPos.y = _currentPos.y / RATIO;
                    printf("X%.3f Y%.3f\n", currentPos.x, currentPos.y);
                    prevIndex = currentIndex;
                    break;
                }
            }
            if (dy == i)
            {
                currentPos.x = nextPos.x;
                currentPos.y = nextPos.y;
                state = LINE_DONE;
            }
        }
        break;
    case LINE_DONE:
        if ((currentPos.x != nextPos.x) ||
            (currentPos.y != nextPos.y))
        {
            state = LINE_INIT;
        }
        else
        {
            returnValue = true;
            printf("X%.3f Y%.3f\n", nextPos.x, nextPos.y);
        }
        break;
    default:
        break;
    }

    return returnValue;
}

bool ABL_ComputeLine(char buffer[])
{
    double value = 0;
    char temp[255];
    char printBuffer[255];
    printBuffer[0] = '\0';
    bool isCommandAvailable = false;
    bool xyzAvailable = false;
    ABL_LoadLinePointer(buffer);

    if (true == ABL_ParseNumber('G', &value))
    {
        isCommandAvailable = true;
        sprintf(temp, "G%02d ", (int) value);
        strcat(printBuffer, temp);
    }
    if (true == ABL_ParseNumber('F', &value))
    {
        isCommandAvailable = true;
        sprintf(temp, "F%d ", (int) value);
        strcat(printBuffer, temp);
    }
    if (true == ABL_ParseNumber('T', &value))
    {
        isCommandAvailable = true;
        sprintf(temp, "T%d ", (int) value);
        strcat(printBuffer, temp);
    }
    if (true == ABL_ParseNumber('M', &value))
    {
        isCommandAvailable = true;
        sprintf(temp, "M%02d ", (int) value);
        strcat(printBuffer, temp);
    }
    if (true == ABL_ParseNumber('S', &value))
    {
        isCommandAvailable = true;
        sprintf(temp, "S%d", (int) value);
        strcat(printBuffer, temp);
    }
    if (true == ABL_ParseNumber('X', &nextPos.x))
    {
        isCommandAvailable = true;
        xyzAvailable = true;
        // sprintf(temp, "X%.3f ", pos.x);
        // strcat(printBuffer, temp);
    }
    if (true == ABL_ParseNumber('Y', &nextPos.y))
    {
        isCommandAvailable = true;
        xyzAvailable = true;
        // sprintf(temp, "Y%.3f ", pos.y);
        // strcat(printBuffer, temp);
    }
    if (xyzAvailable)
    {
        while (!ComputeLine())
            ;
    }

    if (true == ABL_ParseNumber('Z', &nextPos.z))
    {
        isCommandAvailable = true;
        sprintf(temp, "Z%.3f ", nextPos.z);
        strcat(printBuffer, temp);
    }
    if (false == isCommandAvailable)
    {
        buffer[(int) strlen(buffer) - 1] = '\0';
        strcat(printBuffer, buffer);
    }
    printf("%s\n", printBuffer);
    return 1;
}