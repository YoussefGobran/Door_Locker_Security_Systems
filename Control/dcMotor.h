
#ifndef DCMOTOR_H_
#define DCMOTOR_H_

#include "std_types.h"
/*Motor states*/
#define STOP 0
#define CW 1
#define A_CW 2
#define MOTOR_PORT PORTB_ID
#define first_pin PIN0_ID
#define second_pin PIN1_ID

typedef uint8 DcMotor_State;
/*
	 *1. The Function responsible for setup the direction
	 * for the two motor pins through the GPIO driver.
	 *
	 *2.Stop at the DC-Motor at the beginning through the GPIO driver.
	 *
	 * */
void DcMotor_Init(void);
/*The Function responsible for controlling DC motor*/
void DcMotor_Rotate(DcMotor_State state,uint8 speed);

#endif /* DCMOTOR_H_ */
