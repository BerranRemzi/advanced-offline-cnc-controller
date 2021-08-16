#include "./AutoBedLeveling.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define RATIO 10.0

char *serialBuffer;
int soFar = 255; // how much is in the buffer
double posX = 1.0;
double posY = 1.0; // location

void AutoBedLeveling(void)
{
    printf("void AutoBedLeveling(void)\n");
}

void ABL_LoadLinePointer(char *line)
{
    serialBuffer = line;
}

/**
 * Look for character /code/ in the buffer and read the float that immediately follows it.
 * @return the value found.  If nothing is found, /val/ is returned.
 * @param code the character to look for.
 * @param val the return value if /code/ is not found.
 **/
bool ABL_ParseNumber(char code, double *pValue)
{
    char *ptr = serialBuffer; // start at the beginning of buffer
    while ((long)ptr > 1 && (*ptr) && (long)ptr < (long)serialBuffer + soFar)
    { // walk to the end
        if (*ptr == code)
        {                            // if you find code on your walk,
            *pValue = atof(ptr + 1); // convert the digits that follow into a float and return it
            return true;
        }
        ptr = strchr(ptr, ' ') + 1; // take a step from here to the letter after the next space
    }
    return false; // end reached, nothing found, return default val.
}

void line(float newx, float newy)
{
    long i;
    long over = 0;

    long _posX = (long)(posX * RATIO);
    long _posY = (long)(posY * RATIO);

    long _newX = (long)(newx * RATIO);
    long _newY = (long)(newy * RATIO);

    long dx = (_newX - _posX);
    long dy = (_newY - _posY);
    long dirx = dx > 0 ? 1 : -1;
    long diry = dy > 0 ? 1 : -1; 
    dx = abs(dx);
    dy = abs(dy);

    if (dx > dy)
    {
        over = dx / 2;
        for (i = 0; i < dx; ++i)
        {
            //m1step(dirx);
            _posX += dirx;
            over += dy;
            if (over >= dx)
            {
                over -= dx;
                //m2step(diry);
                _posY += diry;
            }
            //pause(step_delay);
            if (_posX % (long)(10 * RATIO) == 0 || _posY % (long)(10 * RATIO) == 0)
                printf("X%.3f Y%.3f\n", _posX/RATIO , _posY/RATIO);
        }
    }
    else
    {
        over = dy / 2;
        for (i = 0; i < dy; ++i)
        {
            _posY += diry;
            over += dx;
            if (over >= dy)
            {
                over -= dy;
                _posX += dirx;
            }
            //pause(step_delay);
            if (_posX % (long)(10 * RATIO) == 0 || _posY % (long)(10 * RATIO) == 0)
                printf("X%.3f Y%.3f\n", _posX/RATIO, _posY/RATIO);
        }
    }

    printf("X%.3f Y%.3f\n", _posX/RATIO, _posY/RATIO);

    posX = newx;
    posY = newy;
}

float bilinearInterpolation(float q11, float q12, float q21, float q22, float x1, float x2, float y1, float y2, float x, float y) 
{
    float x2x1, y2y1, x2x, y2y, yy1, xx1;
    x2x1 = x2 - x1;
    y2y1 = y2 - y1;
    x2x = x2 - x;
    y2y = y2 - y;
    yy1 = y - y1;
    xx1 = x - x1;
    return 1.0 / (x2x1 * y2y1) * (
        q11 * x2x * y2y +
        q21 * xx1 * y2y +
        q12 * x2x * yy1 +
        q22 * xx1 * yy1
    );
}