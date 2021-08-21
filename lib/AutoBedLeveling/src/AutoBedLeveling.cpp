#include "./AutoBedLeveling.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define RATIO 1000.0

char *serialBuffer;
Coordinates_t previousCoord;
Coordinates_t midCoord;
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

    static Index_t prevIndex;
    Index_t currentIndex;

    dx = abs(dx);
    dy = abs(dy);

    if (dx > dy)
    {
        over = dx / 2;
        for (i = 0; i < dx; ++i)
        {
            _posX += dirx;
            over += dy;
            if (over >= dx)
            {
                over -= dx;
                _posY += diry;
            }
            Coordinates_t pos = {_posX/RATIO, _posY/RATIO};
            currentIndex = getGridIndex(pos);

            if ((prevIndex.x != currentIndex.x) || (prevIndex.y != currentIndex.y)){
                printf("X%.2f Y%.2f\n", _posX/RATIO , _posY/RATIO);
            }
            prevIndex = currentIndex;
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
            Coordinates_t pos = {_posX/RATIO, _posY/RATIO};
            currentIndex = getGridIndex(pos);

            if ((prevIndex.x != currentIndex.x) || (prevIndex.y != currentIndex.y)){
                printf("X%.2f Y%.2f\n", _posX/RATIO , _posY/RATIO);
            }
            prevIndex = currentIndex;
        }
    }

    printf("X%.2f Y%.2f\n", _posX/RATIO, _posY/RATIO);

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
double linearInterpolation(double xp, double x0, double y0,double x1,double y1){
    return y0 + ((y1-y0)/(x1-x0)) * (xp - x0);
}

Index_t getGridIndex(Coordinates_t coordinates) {
    Index_t temp;
    temp.x = coordinates.x / 10;
    temp.y = coordinates.y / 10;

    if (temp.x < 0) temp.x = 0;
    else if (temp.x > 3) temp.x = 3;

    if (temp.y < 0) temp.y = 0;
    else if (temp.y > 3) temp.y = 3;

    return temp;
}

double GetX(double yp, Coordinates_t p0, Coordinates_t p1){
    return linearInterpolation(yp, p0.y, p0.x, p0.y, p0.x);
}

double GetY(double xp, Coordinates_t p0, Coordinates_t p1){
    return linearInterpolation(xp, p0.x, p0.y, p0.x, p0.y);
}

bool getIntersections(char* buffer, Coordinates_t current){
    bool value = false;

    if (current.x == midCoord.x && current.y == midCoord.y) {
        previousCoord = current;
        value = true;
    } else {
        //Bresenham
    }

    return value;
}
