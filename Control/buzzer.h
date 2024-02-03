#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h"
#include "common_macros.h"
#define BUZZER_PORT PORTD_ID
#define BUZZER_PIN  PIN3_ID
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*DISCRPTION:
 * Setup the direction for the buzzer pin as output pin through the GPIO driver.
 * Turn off the buzzer through the GPIO.
 * */
void Buzzer_init(void);
/*Function to enable the Buzzer through the GPIO.*/
void Buzzer_on(void);
/*Function to disable the Buzzer through the GPIO.*/
void Buzzer_off(void);

#endif
