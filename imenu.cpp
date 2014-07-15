#include "ilewis.h"


// ----- Phys253.h stuff -----

inline int knob6() { return analogRead(6); }
inline int knob7() { return analogRead(7); }
inline int startbutton() { return !digitalRead(50); }
inline int stopbutton()  { return !digitalRead(49); }

LiquidCrystal LCD(26,27,28,2);

// ---------------------------

#if DEBUG
void print_lewis(int e)
{
   LCD.clear(); LCD.home();
   LCD.setCursor(0,0); LCD.print(e);
}
#endif

void start_display()
{
    LCD.clear(); LCD.home();
    LCD.setCursor(0,0); LCD.print(">>>> LEWIS <<<<");
    delay(2500);

    LCD.clear(); LCD.home();
    LCD.setCursor(0,0); LCD.print(">>>> LEWIS <<<<");
    LCD.setCursor(0,1); LCD.print("WELCOME TO HELL");

    while (!startbutton()) { delay(150); }
}

#if DEBUG
#undef TAPE_THRESH
#undef TAPE_KP
#undef TAPE_KD
#undef TAPE_SPEED
uint TAPE_THRESH = 250;
uint TAPE_KP     = 50;
uint TAPE_KD     = 15;
uint TAPE_SPEED  = 300;
#endif

void initialize()
{
    delay(500);
    while (!startbutton()) {
        LCD.clear(); LCD.home();
        LCD.setCursor(0,0); LCD.print("L: "); LCD.print(analogRead(aQRD_L));
        LCD.setCursor(0,1); LCD.print("R: "); LCD.print(analogRead(aQRD_R));
        delay(200);
    }
    delay(500);

#if DEBUG
    uint startDelay  = 150;
    uint setDelay    = 200;
    uint nextDelay   = 500;

    uint knobScale   = 3;
    uint knobPos     = knob6();
    uint option;

    // Choose options
    // 0. Whole Course
    // 1. Tape new Parameters
    // 2. Tape default Parameters
    while (!startbutton()) {
         LCD.clear(); LCD.home();
         LCD.setCursor(0,0); LCD.print("What to do?");
         LCD.setCursor(0,1); LCD.print(knobPos);
         knobPos = knob6() / 128;
         option = knobPos;
         delay(setDelay);
    }
    delay(nextDelay);

    // Run the whole course
    if (option == 0) {
        LCD.clear(); LCD.home();
        LCD.setCursor(0,0); LCD.print("Run All");
        LCD.setCursor(0,1); LCD.print("Start");
        while (!startbutton()) {
            if (stopbutton()) { initialize(); }
            delay(startDelay);
        }
        return;
    }
    // Run tape with new parameters
    if (option == 1) {
        LCD.clear(); LCD.home();
        LCD.setCursor(0,0); LCD.print("Run Tape, Param");
        LCD.setCursor(0,1); LCD.print("Start");
        while (!startbutton()) {
            if (stopbutton()) { initialize(); }
            delay(startDelay);
        }
        // Set Tape Thresh
        LCD.clear(); LCD.home();
        LCD.setCursor(0,0); LCD.print("Set Tape Thresh");
        LCD.setCursor(0,1); LCD.print("START");

        while (!startbutton()) { delay(startDelay); }
        
        while (!stopbutton()) {
            LCD.clear(); LCD.home();
            LCD.setCursor(0,0); LCD.print("Thresh: "); LCD.print(knobPos);
            knobPos = knob6() / knobScale;
            delay(setDelay);
        }
        TAPE_THRESH = knobPos;
        delay(nextDelay);

        // Set Tape KP
        LCD.clear(); LCD.home();
        LCD.setCursor(0,0); LCD.print("Set Tape KP");
        LCD.setCursor(0,1); LCD.print("START");

        while (!startbutton()) { delay(startDelay); }
        
        while (!stopbutton()) {
            LCD.clear(); LCD.home();
            LCD.setCursor(0,0); LCD.print("KP: "); LCD.print(knobPos);
            knobPos = knob6() / knobScale;
            delay(setDelay);
        }
        TAPE_KP = knobPos;
        delay(nextDelay);

        // Set Tape KD
        LCD.clear(); LCD.home();
        LCD.setCursor(0,0); LCD.print("Set Tape KD");
        LCD.setCursor(0,1); LCD.print("START");

        while (!startbutton()) { delay(startDelay); }
        
        while (!stopbutton()) {
            LCD.clear(); LCD.home();
            LCD.setCursor(0,0); LCD.print("KD: "); LCD.print(knobPos);
            knobPos = knob6() / knobScale;
            delay(setDelay);
        }
        TAPE_KD = knobPos / PID_TIME;
        delay(nextDelay);

        // Set Tape Speed
        LCD.clear(); LCD.home();
        LCD.setCursor(0,0); LCD.print("Set Tape Speed");
        LCD.setCursor(0,1); LCD.print("START");

        while (!startbutton()) { delay(startDelay); }
        
        while (!stopbutton()) {
            LCD.clear(); LCD.home();
            LCD.setCursor(0,0); LCD.print("SPEED: "); LCD.print(knobPos);
            knobPos  = knob6() / knobScale;
            delay(setDelay);
        }
        TAPE_SPEED = knobPos;
        delay(nextDelay);

        // Recap
        LCD.clear(); LCD.home();
        LCD.setCursor(0,0);
        LCD.print("T: "); LCD.print(TAPE_THRESH);
        LCD.print(" V: "); LCD.print(TAPE_SPEED);

        LCD.setCursor(0,1);
        LCD.print("P: "); LCD.print(TAPE_KP);
        LCD.print(" D: "); LCD.print(TAPE_KD);

        while (!startbutton()) { delay(startDelay); }

        while (!stopbutton()) { lewis_tape(); }
        initialize();

    }
    // Run tape with default parameters
    if (option == 2) {
        LCD.clear(); LCD.home();
        LCD.setCursor(0,0); LCD.print("Run Tape");
        LCD.setCursor(0,1); LCD.print("START");

        while (!startbutton()) { delay(startDelay); }
 
        // Set Tape Speed
        LCD.clear(); LCD.home();
        LCD.setCursor(0,0); LCD.print("Set Tape Speed");
        LCD.setCursor(0,1); LCD.print("START");

        while (!startbutton()) { delay(startDelay); }
        
        while (!stopbutton()) {
            LCD.clear(); LCD.home();
            LCD.setCursor(0,0); LCD.print("SPEED: "); LCD.print(knobPos);
            knobPos  = knob6();
            delay(setDelay);
        }
        TAPE_SPEED = knobPos;
        delay(nextDelay);

        LCD.clear(); LCD.home();
        LCD.setCursor(0,0); LCD.print("Let's Go!!!");
        LCD.setCursor(0,1); LCD.print("START");

        while (!startbutton()) {
            if (stopbutton()) { initialize(); }
            delay(startDelay);
        }
        while (!stopbutton()) { lewis_tape(); }
        initialize();
    }
    LCD.clear(); LCD.home();
    LCD.setCursor(0,0); LCD.print("Sorry");
    LCD.setCursor(0,1); LCD.print("Try Again");
    delay(2000);
    initialize();
#endif
}
