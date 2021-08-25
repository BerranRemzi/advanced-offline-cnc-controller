#pragma once

#define TASK(NAME, FUNCTION, PERIOD)                  \
    static uint32_t millis##NAME = 0;                 \
    if ((uint32_t)(millis() - millis##NAME) > PERIOD) \
    {                                                 \
        millis##NAME = millis();                      \
        FUNCTION;                                     \
    }