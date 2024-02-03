#include "gpio.h"
#include "pwm.h"
#include"dcMotor.h"

void DcMotor_Init(void)
{
	/*
	 *1. The Function responsible for setup the direction
	 * for the two motor pins through the GPIO driver.
	 *
	 *2.Stop at the DC-Motor at the beginning through the GPIO driver.
	 *
	 * */
	GPIO_setupPinDirection(MOTOR_PORT, first_pin, PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_PORT, second_pin, PIN_OUTPUT);
	GPIO_writePin(MOTOR_PORT, first_pin,LOGIC_LOW);
	GPIO_writePin(MOTOR_PORT, second_pin,LOGIC_LOW);
}
/*The Function responsible for controlling DC motor*/
void DcMotor_Rotate(DcMotor_State state,uint8 speed)
{
	if(state == STOP)
	{
		GPIO_writePin(MOTOR_PORT, first_pin,LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT, second_pin,LOGIC_LOW);
	}

	else if(state == A_CW)
	{
		GPIO_writePin(MOTOR_PORT, first_pin,LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT, second_pin,LOGIC_HIGH);
	}

	else if(state == CW)
	{
		GPIO_writePin(MOTOR_PORT, first_pin,LOGIC_HIGH);
		GPIO_writePin(MOTOR_PORT, second_pin,LOGIC_LOW);
	}
	/*speed Controlling*/
	PWM_Timer0_Start(speed);

}

