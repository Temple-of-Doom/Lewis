#ifndef motor_h
#define motor_h

const int Motor_Direction[] 	= {24, 25, 38, 39} ;
const int Motor_Enable[] 		= {5,  4,  1,  0} ;   // PWM output values

class motorClass
{
	public:
		motorClass()
        {
            for (int i = 0 ; i < 4; ++i) {
                pinMode(Motor_Direction[i], OUTPUT);
                digitalWrite(Motor_Direction[i], LOW);
                analogWrite(Motor_Enable[i], 0);
            }
        }

		void stop(int m) { analogWrite(Motor_Enable[m], 0); }
		void stop_all()
        {
            for (int i = 0; i < 4; ++i) {
                analogWrite(Motor_Enable[i], 0);
            }
        }
		void speed(int m, int s)
        {
            digitalWrite(Motor_Direction[m], (s >= 0 ? HIGH : LOW));
            analogWrite(Motor_Enable[m], abs(s));
        }
};

#endif
