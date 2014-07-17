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

extern "C" {
    extern void pinMode(uint8_t, uint8_t);
    extern uint8_t digitalRead(uint8_t);
    extern void digitalWrite(uint8_t, uint8_t);

    extern void timer2Init(void);
    extern void timer2SetPrescaler(uint8_t);
    extern void timerAttach(uint8_t, void(*)(void));
    extern void timerDetach(uint8_t);
}

#define SERVO_MINIMUM 10
#define SERVO_MAXIMUM 40
#define SERVO_PERIOD  500

class Servo253
{
    private:
        uint8_t _index;
        uint8_t _pin;
        uint16_t _duty;
    
        static uint8_t _count;
        static Servo253* _servos[];
        static int8_t _current;
        static uint16_t _positionTicks;
    
        static void start()
        {
            //digitalWrite(48, HIGH);
            timer2Init();
            // disble timer2 output compare interrupt
            TIMSK &= ~_BV(OCIE2);
            // set the prescaler for timer2
            timer2SetPrescaler(TIMER_CLK_DIV1024);
            // attach the software PWM service routine to timer2 output compare
            timerAttach(TIMER2OUTCOMPARE_INT, service);
            // update output compare register
            OCR2 += SERVO_PERIOD;
            // enable the timer2 output compare interrupt
            TIMSK |= _BV(OCIE2);
            // reset position ticks
            _positionTicks = 0;
            // reset current servo
            _current = -1;
        }
    
        static void end()
        {
            TIMSK &= ~_BV(OCIE2);
            timerDetach(TIMER2OUTCOMPARE_INT);
        }
    
        static void service()
        {
            uint16_t ticks;
            
            // turn off current servo, if not "non servo"
            if (0 <= _current) {
                digitalWrite(_servos[_current]->_pin, LOW);
            }
            
            // advance to next servo
            while (_current < 7) {
                if (NULL != _servos[++_current]) { break; }
              }
            
            // if current is not a servo
            if (NULL == _servos[_current]) {
                // reset and wait out the period
                _current = -1;
                ticks = SERVO_PERIOD - _positionTicks;
            } else {
                // turn on new servo
                digitalWrite(_servos[_current]->_pin, HIGH);
                // schedule ttl
                ticks = _servos[_current]->_duty;
            }
            // schedule next interrupt
            OCR2 += ticks;
            // keep track of this period
            _positionTicks += ticks;
            if (_positionTicks >= SERVO_PERIOD) { _positionTicks = 0; }
        }
    
    public:
        Servo253()
        {
            _index = 0;
            _duty  = SERVO_MINIMUM;
        }
    
        uint8_t attach(int pin)
        {
          // if list is already full, return a failure
          if (8 < _count) { return 0; }
          // record pin number, set pin to output
          pinMode(_pin = pin, OUTPUT);
          // if already in list, return a success
          if (this == _servos[_index]) { return 1; }
          // if first servo, ready the list
          if (0 == _count) {
            for (uint8_t i = 0; i < 8; ++i) { _servos[i] = NULL; }
          }
          // find empty slot, add servo to list
          for (uint8_t i = 0; i < 8; ++i) {
            if (NULL == _servos[i]) {
                _servos[i] = this;
                _index = i;
                ++_count;
                break;
            }
          }
          // init servos if this is the first
          if (1 == _count) { start(); }
          // return a success
          return 1;
        }
    
        void detach()
        {
          // if already in list
          if (this == _servos[_index]) {
            // remove from list
            _servos[_index] = NULL;
            // adjust count and end timer if this was last servo
            if(0 < _count) {
              --_count;
              if (0 == _count) { end(); }
            }
          }
        }
    
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
