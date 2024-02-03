#include"keypad.h"
#include"lcd.h"
#include"uart.h"
#include "avr/io.h"
#include"util/delay.h"
#include "timer1.h"

uint8 count=0;
/*timer1 call back function*/
void Delay_1()
{
	count++;
}
/*function to read password from user*/
void ReadPassword(uint8 * password)
{
	for(uint8 i = 0;i<5;i++)
	{
		password[i] = KEYPAD_getPressedKey();
		_delay_ms(500);
		LCD_displayCharacter('*');
	}
}
/*function to send password to control*/
void SendPassword(uint8 * password)
{
	for(uint8 i = 0;i<5;i++)
	{
		UART_sendByte(password[i]);

	}
}
/*display function while opening door*/
void OpenDoor (Timer1_ConfigType TIMER1_Ptr)
{
	LCD_clearScreen();
	LCD_displayString("Door is Unlocking");
	Timer1_init(&TIMER1_Ptr);
	while(count!=15);
	Timer1_deInit();
	count=0;
	LCD_clearScreen();
	LCD_displayString("Door Unlocked");
	Timer1_init(&TIMER1_Ptr);
	while(count!=3);
	Timer1_deInit();
	count=0;
	LCD_clearScreen();
	LCD_displayString("Door is locking");
	Timer1_init(&TIMER1_Ptr);
	while(count!=15);
	Timer1_deInit();
	count=0;
}

/************************************************************************************************
 *
 *                                  MAIN METHOD
 *
 ***********************************************************************************************/
int main(void)
{
	/*setting I bit to enable interrupts*/
	SREG|=(1<<7);
	/*LCD INTIALIZATION*/
	LCD_init();
	/*UART INTIALIZATION*/
	UART_ConfigType UART_PTR ={ID_8_BITS,Disabled,one_bit,9600};
	UART_init(&UART_PTR);
	/*TIMER1 INTIALIZATION*/
	Timer1_setCallBack(Delay_1);
	Timer1_ConfigType TIMER1_Ptr={0,7813,Prescaler1024,Compare};
	/*PROGRAM INTIALIZATIONS*/
	uint8 step =1,PrevStep=0;
	uint8 password[5];
	/*GETTING UART READY*/
	UART_sendByte(0xff);
	while(UART_recieveByte()!=0xff);
	/*MAIN PROGRAM CODE LOOP*/
	while(1)
	{

		/*Create a System Password*/
		 if(step==1)
		{
			LCD_clearScreen();
			LCD_displayString("please enter new");
			LCD_moveCursor(1,0);
			LCD_displayString("pass: ");
			LCD_moveCursor(1,6);
			ReadPassword(password);/*READ NEW PASS*/
			while(KEYPAD_getPressedKey()!= 13);
			_delay_ms(500);
			SendPassword(password);
			LCD_clearScreen();
			LCD_displayString("Please re-enter the");
			LCD_moveCursor(1,0);
			LCD_displayString("same Pass:");
			LCD_moveCursor(1,11);
			ReadPassword(password);/*RE CONFIRM NEW PASS*/
			while(KEYPAD_getPressedKey()!= 13);
			_delay_ms(500);
			SendPassword(password);
			if(UART_recieveByte())/*CHECK IF 2 PASS ARE EQUAL*/
			{
				step=2;/*IF TRUE GO TO MAIN MENU*/
			}
			else
			{
				step=1;/*ELSE RE ENTER NEW PASS*/
			}
		}

		/*Main Options*/
		if(step==2)
		{
			uint8 x;
			LCD_clearScreen();
			LCD_displayString("+ : Open Door");
			LCD_moveCursor(1,0);
			LCD_displayString("- : Change Pass");
			x=KEYPAD_getPressedKey();
			_delay_ms(500);
			if(x=='+')
			{
				UART_sendByte('+');
				step=3;
			}
			if(x=='-')
			{
				UART_sendByte('-');
				step=4;
			}
		}

		/*Open Door*/
		if(step==3)
		{
			LCD_clearScreen();
			LCD_displayString("plz enter pass:");
			LCD_moveCursor(1,0);
			ReadPassword(password);/*READ PASS*/
			while(KEYPAD_getPressedKey()!= 13);/*CLICK ENTER*/
			_delay_ms(500);
			SendPassword(password);
			if(UART_recieveByte())/*CHECK IF PASSWORD IS TRUE*/
			{
				OpenDoor (TIMER1_Ptr);/*IF TRUE OPEN DOOR*/
				step =2;
			}
			else/*IF NOT GO TO STEP 5 KNOWING THAT YOU WERE PREVIOUSLY AT STEP 3*/
			{
				PrevStep = 3;
				step=5;
			}
		}

		/*Change Password*/
		if(step==4)
		{
			LCD_clearScreen();
			LCD_displayString("plz enter pass:");
			LCD_moveCursor(1,0);
			ReadPassword(password);/*READ PASS*/
			while(KEYPAD_getPressedKey()!= 13);/*CLICK ENTER*/
			_delay_ms(500);
			SendPassword(password);/*CHECK IF PASSWORD IS TRUE*/
			if(UART_recieveByte())/*IF TRUE CHANGE PASS*/
			{
				step=1;
			}
			else/*IF NOT GO TO STEP 5 KNOWING THAT YOU WERE PREVIOUSLY AT STEP 4*/
			{
				PrevStep = 4;
				step = 5;
			}
		}

		/*password unmatched*/
		if(step==5)
		{
			for(uint8 i=0;i<2;i++)/*HAVE ONLY 2 TIMES TO GET TRUE PASS OR SYSTEM LL BE LOCKED*/
			{
				LCD_clearScreen();
				LCD_displayString("plz enter pass:");
				LCD_moveCursor(1,0);
				ReadPassword(password);/*READ PASS*/
				while(KEYPAD_getPressedKey()!= 13);/*CLICK ENTER*/
				_delay_ms(500);
				SendPassword(password);/*CHECK IF PASSWORD IS TRUE*/
				if(UART_recieveByte())/*IF CONTOL GIVES TRUE; DO THE PREVIOUS STEP FUNCTIONALITTY */
				{
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
				else if(i==1)/*IF THE 2 TIME FAILS LOCK SYSTEM*/
				{
					LCD_clearScreen();
					LCD_displayString("SYSTEM LOCKED!");
					Timer1_init(&TIMER1_Ptr);
					while(count!=60);
					Timer1_deInit();
					count=0;
					step=2;
				}
			}
		}
	}
}


