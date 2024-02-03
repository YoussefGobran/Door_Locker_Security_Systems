#include<avr/io.h>
#include"pwm.h"

void PWM_Timer0_Start(uint8 duty_cycle)
{
	TCNT0 = 0; // Set Timer Initial Value to 0
	char actual_duty_cycle = (uint8)((((float)duty_cycle)/100)*255);
	OCR0  = actual_duty_cycle; // Set Compare Value(duty cycle)

	DDRB  |= (1<<PB3); // Configure PB3/OC0 as output pin

	/*
	 * TCCR0 register description
	 * WGM01 and WGM00 = 1 to set timer0 at fast pwm mode
	 * COM01 = 1 to set pwm to non inverting mood (oc0 is cleared after compare)
	 * CS01 = 1 to set it to prescaler 8
	 */
	TCCR0 = (1<<WGM01)|(1<<WGM00)|(1<<COM01)|(1<<CS01);

}
