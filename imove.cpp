#include "ilewis.h"

motorClass motor;

static volatile uint speed;
static volatile bool moving;

void set_speed(int v)
{
    if (v == 0) {
        speed = 0;
        moving = false;
        motor.stop(pMOTOR_L);
        motor.stop(pMOTOR_R);
    }
    else {
        speed = v;
        moving = true;
    }
}

void follow_tape()
{
    int speedAdj = tape_adjustment();

    if (!moving) { return; }

    motor.speed(pMOTOR_L, (speed + speedAdj));
    motor.speed(pMOTOR_R, (speed - speedAdj));
}

void pickup_idol()
{
    delay(2000);
}

void follow_ir()
{
}

void rotate()
{
}
