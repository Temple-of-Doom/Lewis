#include "ilewis.h"


void run_lewis()
{
    start_display();
    initialize();
	lewis_arm_setup();
    lewis_tape();
    lewis_find_ir();
    lewis_tempel();
    lewis_go_home();
}

void lewis_tape()
{
    uint items = 0;

    tape_reset();
    set_speed(TAPE_SPEED);

    Timer2.set(PID_TIME, follow_tape);
    Timer2.start();
	//DEPERACTED
	/*
	while (items < 3) {
		if (is_idol()) {
			set_speed(TAPE_PICKUP_SPEED);
			set_speed(TAPE_SPEED);
			++items;
		}
	}
	*/
}
void lewis_arm_setup()
{
	//extend arm from start position
	servo_swerve();
}
void lewis_find_ir()
{
}

void lewis_tempel()
{
}

void lewis_go_home()
{
}


void shutdown() { end_display(); }
