#include "ilewis.h"


// -------------------------- TAPE FOLLOWING -------------------------- //

static float tapeLastError;

void tape_reset() { tapeLastError = 0.0f; }

float tape_error()
{
    bool leftOnTape  = (analogRead(aQRD_L) > TAPE_THRESH ? true : false);
    bool rightOnTape = (analogRead(aQRD_R) > TAPE_THRESH ? true : false);

    if (leftOnTape && rightOnTape) {
        return 0;
    }
    if (leftOnTape) {
        return -TAPE_ERROR_S;
    }
    if (rightOnTape) {
        return TAPE_ERROR_S;
    }
    else {
        return (tapeLastError > 0 ? TAPE_ERROR_B : -TAPE_ERROR_B);
    }
}

int tape_adjustment()
{
    float currentError = tape_error();

    float p = TAPE_KP * currentError;
    float d = (float)TAPE_KD * (float)(currentError - tapeLastError);

    tapeLastError = currentError;
    return (int)(p + d);
}
