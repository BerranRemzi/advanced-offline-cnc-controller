#include "AutoBedLeveling.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define RATIO 1000.0

char* serialBuffer;
Coordinates_t previousCoord;
Coordinates_t midCoord;
int16_t soFar = 255; // how much is in the buffer
double posX = 1.0;
double posY = 1.0; // location

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
        if (*ptr == code)
        {                            // if you find code on your walk,
            *pValue = atof(ptr + 1); // convert the digits that follow into a double and return it
            return true;
        }
        ptr = strchr(ptr, ' ') + 1; // take a step from here to the letter after the next space
    }
    return false; // end reached, nothing found, return default val.
}

void line(double newx, double newy)
{
    int32_t i;
    int32_t over = 0;

    int32_t _posX = (int32_t) (posX * RATIO);
    int32_t _posY = (int32_t) (posY * RATIO);

    int32_t _newX = (int32_t) (newx * RATIO);
    int32_t _newY = (int32_t) (newy * RATIO);

    int32_t dx = (_newX - _posX);
    int32_t dy = (_newY - _posY);
    int32_t dirx = dx > 0 ? 1 : -1;
    int32_t diry = dy > 0 ? 1 : -1;

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
            Coordinates_t pos = { _posX / RATIO, _posY / RATIO };
            currentIndex = getGridIndex(&pos);

            if ((prevIndex.x != currentIndex.x) || (prevIndex.y != currentIndex.y)) {
                printf("X%.2f Y%.2f\n", _posX / RATIO, _posY / RATIO);
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
            Coordinates_t pos = { _posX / RATIO, _posY / RATIO };
            currentIndex = getGridIndex(&pos);

            if ((prevIndex.x != currentIndex.x) || (prevIndex.y != currentIndex.y)) {
                printf("X%.2f Y%.2f\n", _posX / RATIO, _posY / RATIO);
            }
            prevIndex = currentIndex;
        }
    }

    printf("X%.2f Y%.2f\n", _posX / RATIO, _posY / RATIO);

    posX = newx;
    posY = newy;
}

double bilinearInterpolation(double q11, double q12, double q21, double q22, double x1, double x2, double y1, double y2, double x, double y)
{
    double x2x1;
    double y2y1;
    double x2x;
    double y2y;
    double yy1;
    double xx1;
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
double linearInterpolation(double xp, double x0, double y0, double x1, double y1) {
    return y0 + ((y1 - y0) / (x1 - x0)) * (xp - x0);
}

Index_t getGridIndex(Coordinates_t  const* coordinates) {
    Index_t temp;
    temp.x = (int16_t) (coordinates->x / 10);
    temp.y = (int16_t) (coordinates->y / 10);

    if (temp.x < 0) temp.x = 0;
    else if (temp.x > 3) temp.x = 3;

    if (temp.y < 0) temp.y = 0;
    else if (temp.y > 3) temp.y = 3;

    return temp;
}

double GetX(double yp, Coordinates_t const* p0, Coordinates_t const* p1) {
    return linearInterpolation(yp, p0->y, p0->x, p1->y, p1->x);
}

double GetY(double xp, Coordinates_t const* p0, Coordinates_t const* p1) {
    return linearInterpolation(xp, p0->x, p0->y, p1->x, p1->y);
}

bool getIntersections(char* buffer, Coordinates_t const* current) {
    bool value = false;

    if (current->x == midCoord.x && current->y == midCoord.y) {
        previousCoord = *current;
        value = true;
    }
    else {
        //Bresenham
    }

    return value;
}

bool ComputeLine(double newx, double newy)
{
    bool returnValue = false;

    static ComputeLine_t state = LINE_INIT;

    static Index_t prevIndex;
    static Index_t currentIndex;

    static int32_t i;

    static int32_t _posX;
    static int32_t _posY;

    static int32_t _newX;
    static int32_t _newY;

    static int32_t dx;
    static int32_t dy;

    static int32_t dirx;
    static int32_t diry;

    static int32_t over = 0;

    switch (state) {
    case LINE_INIT:
        _posX = (int32_t) (posX * RATIO);
        _posY = (int32_t) (posY * RATIO);

        _newX = (int32_t) (newx * RATIO);
        _newY = (int32_t) (newy * RATIO);

        dx = (_newX - _posX);
        dy = (_newY - _posY);
        dirx = dx > 0 ? 1 : -1;
        diry = dy > 0 ? 1 : -1;

        dx = abs(dx);
        dy = abs(dy);
        i = 0;

        if (dx > dy)
        {
            over = dx / 2;
        }
        else {
            over = dy / 2;
        }

        state = LINE_BUSY;

        break;
    case LINE_BUSY:
        if (dx > dy)
        {
            for (; i < dx; ++i)
            {
                _posX += dirx;
                over += dy;
                if (over >= dx)
                {
                    over -= dx;
                    _posY += diry;
                }
                Coordinates_t pos = { _posX / RATIO, _posY / RATIO };
                currentIndex = getGridIndex(&pos);

                if ((prevIndex.x != currentIndex.x) || (prevIndex.y != currentIndex.y)) {
                    printf("X%.2f Y%.2f\n", _posX / RATIO, _posY / RATIO);
                    prevIndex = currentIndex;
                    break;
                }
            }
            if (dx == i) {
                posX = newx;
                posY = newy;
                state = LINE_DONE;
            }
        }
        else
        {
            for (; i < dy; ++i)
            {
                _posY += diry;
                over += dx;
                if (over >= dy)
                {
                    over -= dy;
                    _posX += dirx;
                }
                Coordinates_t pos = { _posX / RATIO, _posY / RATIO };
                currentIndex = getGridIndex(&pos);

                if ((prevIndex.x != currentIndex.x) || (prevIndex.y != currentIndex.y)) {
                    printf("X%.2f Y%.2f\n", _posX / RATIO, _posY / RATIO);
                    prevIndex = currentIndex;
                    break;
                }
            }
            if (dy == i) {
                posX = newx;
                posY = newy;
                state = LINE_DONE;
            }
        }
        break;
    case LINE_DONE:
        if (posX != newx ||
            posY != newy) {
            state = LINE_INIT;
        }
        else {
            returnValue = true;
            printf("X%.2f Y%.2f\n", _posX / RATIO, _posY / RATIO);
        }
        break;
    default: break;
    }

    return returnValue;
}
