#ifndef MY_GPIO_H_
#define MY_GPIO_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#define ONE_SEC         8000000 /* in 80 MHz */

#define LED_RED_PIN             GPIO_PIN_1 /* 0000 0010 --> bit 1 for red LED */
#define LED_BLUE_PIN            GPIO_PIN_2 /* 0000 0100 --> bit 2 for blue LED */
#define LED_GREEN_PIN           GPIO_PIN_3 /* 0000 1000 --> bit 3 for green LED */

//SW1, SW2 support
#define SW1_PIN                 GPIO_PIN_4
#define SW2_PIN                 GPIO_PIN_0


/*
 * author: jrkwon
 * 4/5/2023
 * SW2 is not properly working with TivaWare GPIO library
 * Cause: PORTF_CR is not properly set in SysCtlPeripheralEnable()
 * Solution: Need to access PORTF_CR directly to set properly.
 *           To access PORTF_CR, first, PORTF_LOCK must be set
 *           this specific value, 0x4C4F434B
 */
#define GPIO_PORTF_LOCK_R   (*((volatile int*)0x40025520))
#define GPIO_PORTF_CR_R     (*((volatile int*)0x40025524))

//Interrupt for sw1 and sw2 */
#define SW1_INT_PIN             GPIO_INT_PIN_4
#define SW2_INT_PIN             GPIO_INT_PIN_0

//Function prototypes
extern void InitGPIO(void);


#endif /* MY_GPIO_H_ */