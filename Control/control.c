#include"uart.h"
#include"buzzer.h"
#include"dcMotor.h"
#include"twi.h"
#include"external_eeprom.h"
#include"util/delay.h"
#include "avr/io.h"
#include "timer1.h"

uint8 count=0;
/*timer1 call back function*/
void Delay_1()
{
	count++;
}
/*function to read password from hmi*/
void readPassword(uint8*password)
{
	for(uint8 i = 0;i<5;i++)
	{
		password[i]=UART_recieveByte();

	}
}
/*function to check password if true*/
uint8 password_check(uint8*password_temp)
{

	uint8 password[5];
	for(uint8 i=0;i<5;i++)
	{
		EEPROM_readByte((0x311+i),&password[i]);
	}

	for(uint8 i = 0;i<5;i++)
	{
		if(password[i]!=password_temp[i])
		{
			return 0;
		}
	}

	return 1;
}
/*function to open door*/
void OpenDoor (Timer1_ConfigType TIMER1_Ptr)
{
	DcMotor_Rotate(CW,100);
	Timer1_init(&TIMER1_Ptr);
	while(count!=15);
	Timer1_deInit();
	count=0;
	DcMotor_Rotate(STOP,0);
	Timer1_init(&TIMER1_Ptr);
	while(count!=3);
	Timer1_deInit();
	count=0;
	DcMotor_Rotate(A_CW,100);
	Timer1_init(&TIMER1_Ptr);
	while(count!=15);
	Timer1_deInit();
	count=0;
	DcMotor_Rotate(STOP,0);
}
/************************************************************************************************
 *
 *                                  MAIN METHOD
 *
 ***********************************************************************************************/
int main()
{
	/*setting I bit to enable interrupts*/
	SREG|=(1<<7);
	/*MOTOR INTIALIZATION*/
	DcMotor_Init();
	/*BUZZER INTIALIZATION*/
	Buzzer_init();
	/*I2C INTIALIZATION*/
	TWI_ConfigType I2C={0x01,0x02};
	TWI_init(&I2C);
	/*UART INTIALIZATION*/
	UART_ConfigType UART_PTR ={ID_8_BITS,Disabled,one_bit,9600};
	UART_init(&UART_PTR);
	/*TIMER1 INTIALIZATION*/
	Timer1_setCallBack(Delay_1);
	Timer1_ConfigType TIMER1_Ptr={0,7813,Prescaler1024,Compare};
	/*PROGRAM INTIALIZATION*/
	uint8 step =1,PrevStep=0,flag=0,check;
	uint8 password[5];
	uint8 password_temp[5];
	/*GETTING UART READY*/
	while(UART_recieveByte()!=0xff);
	UART_sendByte(0xff);
	/*MAIN PROGRAM CODE LOOP*/
	while(1)
	{
		/*Create a System Password*/
		if(step==1)
		{
			flag=0;
			readPassword(password);
			readPassword(password_temp);
			for(uint8 i = 0;i<5;i++)
			{
				if((password[i])!=(password_temp[i]))/*IF RE ENTERED PASS IS NOT EQUAL*/
				{
					step =1;
					flag =1;
					UART_sendByte(0);/*SEND TO HMI 2 PASSES ARE NOT EQUAL*/
					break;
				}
			}
			/*IF RE ENTERED PASS IS EQUAL ==>> STORE PASS IN EEPROM*/
			if(flag==0)
			{
				EEPROM_writeByte((0x310),1);/*FLAG THAT CAN BE USED TO CHECK IF EEPROM STORE PASS*/
				_delay_ms(10);
				for(uint8 i=0;i<5;i++)
				{
					EEPROM_writeByte((0x311+i),password[i]);
					_delay_ms(10);
				}
				UART_sendByte(1);/*SEND TO HMI 2 PASSES ARE EQUAL*/
				step=2;
			}
		}
		/*Main Options*/
		if(step==2)
		{
			check = UART_recieveByte();
			if(check=='+')
			{
				step=3;
			}
			if(check=='-')
			{
				step=4;
			}
		}
		/*Open Door*/
		if(step==3)
		{
			readPassword(password_temp);
			check=password_check(password_temp);/*CHECK IF PASSWORD IS TRUE*/
			if(check==1)
			{
				UART_sendByte(1);
				OpenDoor (TIMER1_Ptr); /*IF TRUE OPEN DOOR*/
				step=2;
			}
			else
			{
				UART_sendByte(0);/*IF NOT GO TO STEP 5 KNOWING THAT YOU WERE PREVIOUSLY AT STEP 3*/
				PrevStep=3;
				step=5;
			}
		}
		/*Change Password*/
		if(step==4)
		{
			readPassword(password_temp);
			check=password_check(password_temp);/*CHECK IF PASSWORD IS TRUE*/
			if(check==1)
			{
				UART_sendByte(1);/*IF TRUE CHANGE PASS*/
				step=1;
			}
			else
			{
				UART_sendByte(0);/*IF NOT GO TO STEP 5 KNOWING THAT YOU WERE PREVIOUSLY AT STEP 4*/
				PrevStep=4;
				step=5;
			}
		}
		/*password unmatched*/
		if(step==5)
		{
			for(uint8 i=0;i<2;i++){ /*HAVE ONLY 2 TIMES TO GET TRUE PASS OR SYSTEM LL BE LOCKED*/
				readPassword(password_temp);
				check=password_check(password_temp);/*CHECK IF PASSWORD IS TRUE*/
				if(check==1)/*IF TRUE DO THE PREVIOUS STEP FUNCTIONALITTY */
				{
					UART_sendByte(1);/*ACK TO HMI ITS TRUE*/
					if(PrevStep == 3)/*OPEN DOOR DIRECTLY*/
					{
						OpenDoor (TIMER1_Ptr);
						step =2;
					}
					else if(PrevStep == 4)/*CHANGE PASS DIRECTLY*/
					{
						step=1;
					}
					break;
				}
				else
				{
					UART_sendByte(0);/*ACK TO HMI ITS FALSE*/
				}
				if(i==1)/*IF THE 2 TIME FAILS LOCK SYSTEM*/
				{
					Buzzer_on();
					Timer1_init(&TIMER1_Ptr);
					while(count!=60);
					Timer1_deInit();
					count=0;
					Buzzer_off();
					step =2;
				}
			}
		}

	}

}
