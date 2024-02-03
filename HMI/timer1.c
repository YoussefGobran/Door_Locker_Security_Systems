#include "timer1.h"

#include "avr/io.h"
#include "avr/interrupt.h"
#include"gpio.h"

static volatile void (*g_callBackPtr)(void) = NULL_PTR;

ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		(*g_callBackPtr)();
	}
}

ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		(*g_callBackPtr)();
	}
}

void Timer1_init(const Timer1_ConfigType * Config_Ptr)
{
	TCNT1 = (Config_Ptr->initial_value);
	TCCR1A = (1<<FOC1A);
	if((Config_Ptr->mode)==Compare)
	{
		OCR1A = (Config_Ptr->compare_value);
		TCCR1B = (1<<WGM12) | (Config_Ptr->prescaler);
		TIMSK = (1<<OCIE1A);
	}
	if((Config_Ptr->mode)==Normal)
	{
		TCCR1B = (Config_Ptr->prescaler);
		TIMSK = (1<<TOIE1);
	}
}
void Timer1_deInit(void)
{
	TCNT1 = 0;
	TCCR1A =0;
	OCR1A = 0;
	TCCR1B = 0;
	TIMSK = 0;
}
void Timer1_setCallBack(void(*a_ptr)(void))
{
	g_callBackPtr = a_ptr;
}
