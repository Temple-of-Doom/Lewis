/*
  Servo253.h - Servo253 library for Arduino & Wiring
  Based on Hernando Barragan's original C implementation
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  EDITED by: ninjaspongebob
*/

#ifndef Servo253_h
#define Servo253_h

#include <inttypes.h>
#include "WProgram.h"

#define SERVO_MINIMUM 10
#define SERVO_MAXIMUM 40
#define SERVO_PERIOD  500

class Servo253
{
    private:
        uint8_t _pin;
        uint16_t _duty;
        static Servo253* servo[];
    
        static void start()
        {
            timer2Init();
            TIMSK &= ~_BV(OCIE2);
            timer2SetPrescaler(TIMER_CLK_DIV1024);
            timerAttach(TIMER2OUTCOMPARE_INT, service);
            OCR2 += SERVO_PERIOD;
            TIMSK |= _BV(OCIE2);
        }
    
        static void end()
        {
            TIMSK &= ~_BV(OCIE2);
            timerDetach(TIMER2OUTCOMPARE_INT);
        }
    
        static void service()
        {
            digitalWrite(servo[0]->_pin, HIGH);
            OCR2 += servo[0]->_duty;
        }
    
    public:
        Servo253() {
            servo[0] = this;
            _duty    = SERVO_MINIMUM;
        }
    
        void attach(int pin)
        {
          pinMode(_pin = pin, OUTPUT);
          start();
        }
    
        void detach() { end(); }
    
        void write(int angle)
        {
            // position is scaled according to the max and min of servo position
            uint16_t position = (uint16_t)(SERVO_MINIMUM + (((float)angle / 180) * 30));
    
            // bind to limits
            position = position < SERVO_MINIMUM
                ? SERVO_MINIMUM
                : (position > SERVO_MAXIMUM ? SERVO_MAXIMUM : position);
            // set position
            _duty = position;
        }
    
        uint8_t read() { return (uint8_t)(((float)(_duty - SERVO_MINIMUM) / 30) * 180); }
};
#endif
