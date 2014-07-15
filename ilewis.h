#ifndef LEWIS_H
#define LEWIS_H

#include "LiquidCrystal.h"
#include "HardwareTimer2.h"
#include "imotor.h"

// -------------------------------------------------------
// Servo Stuff here for now. Move into Servo.h once needed
const int RCServo0Output    	= 35 ;
const int RCServo1Output    	= 31 ;
const int RCServo2Output    	= 34 ;
// -------------------------------------------------------

// Typedefs
typedef unsigned int uint;
typedef unsigned long uint32;


// ---------- DEBUG ----------

#define DEBUG true 

#if DEBUG
extern uint TAPE_THRESH;
extern uint TAPE_KP;
extern uint TAPE_KD;
extern uint TAPE_SPEED;
void print_follow_tape(int);
#endif

// --------------------------


// TINAH analog pins
#define aQRD_L 7
#define aQRD_R 5

// TINAH PWM
#define pMOTOR_L 3
#define pMOTOR_R 1

// Speed
#define TAPE_SPEED 250
#define TAPE_PICKUP_SPEED 0
#define TEMPLE_SPEED 150
#define TEMPLE_PICKUP_SPEED 0
#define ROTATE_SPEED 100

// PID Timer in ms
#define PID_TIME 50

// PID - Tape
#define TAPE_ERROR_S 1
#define TAPE_ERROR_B 5
#define TAPE_THRESH 250
#define TAPE_KP 50
#define TAPE_KD (10 / PID_TIME)

// Time Limit in millisec
#define TIME_LIMIT 120000

// GLOBAL
extern uint32 startTime;

// lewis.c
void run_lewis();
void lewis_tape();
void lewis_find_ir();
void lewis_tempel();
void lewis_go_home();

// helper.c
bool is_idol();
bool is_ir();
bool is_tape();
bool is_home();

// menu.c
void start_display();
void initialize();
void end_display();

// move.c
void set_speed(int);
void follow_tape();
void pickup_idol();
void follow_ir();
void rotate();

// move_helper.c: PID
void tape_reset();
float tape_error();
int tape_adjustment();

#endif