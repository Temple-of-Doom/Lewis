#include "ilewis.h"

Servo253* Servo253::_servos[8];
uint8_t   Servo253::_count = 0;
int8_t    Servo253::_current = -1;
uint16_t  Servo253::_positionTicks = 0;
Servo253 RCServo1;

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
	spinning_jon_pulse();
}

#if DEBUG
void run_motor(uint v)
{
    motor.speed(pMOTOR_L, v);
    motor.speed(pMOTOR_R, v);
}
#endif

void follow_ir()
{
}
void rotate()
{
}

void servo_swerve()
{
	RCServo1.attach(RC_SERVO_OUTPUT);
	delay(1000);

	for (uint pos = 60; pos < 180; ++pos) {
		RCServo1.write(pos);
		delay(15);
	}
	RCServo1.detach();
}

inline void spinning_jon_pulse() {	motor.speed(pSPINNING_JON, SPINNING_JON_SPEED); }

#if DEBUG
void spinning_jon_DEBUG(uint v) { motor.speed(pSPINNING_JON, v); }
#endif
