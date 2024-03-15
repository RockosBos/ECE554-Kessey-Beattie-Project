/*
 * manin.h
 *
 *  Created on: Apr 7, 2023
 *      Author: arche
 */

#ifndef MAIN_H_
#define MAIN_H_


#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#define ONE_SEC         4000000 /* in 40 MHz */

#define LED_RED             0x02 /* 0000 0010 --> bit 1 for red LED */
#define LED_BLUE            0x04 /* 0000 0100 --> bit 2 for blue LED */
#define LED_GREEN           0x08 /* 0000 1000 --> bit 3 for green LED */

/* function prototypes */
extern void InitGPIO(void);
extern void TurnOnRedLED(void);
extern void TurnOffRedLED(void);
extern void TurnOnBlueLED(void);
extern void TurnOffBlueLED(void);
extern void TurnOnGreenLED(void);
extern void TurnOffGreenLED(void);


void InitGPIO(void)
{
    /* set the system clock 40 MHz */
        SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

        /* gpio configuration */
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); /* 1110 */
}

void TurnOnRedLED(void)
{
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1, LED_RED);
}
void TurnOffRedLED(void)
{
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,0);
}

void TurnOnBlueLED(void)
{
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2, LED_BLUE);
}
void TurnOffBlueLED(void)
{
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2, 0);
}

void TurnOnGreenLED(void)
{
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3, LED_GREEN);
}
void TurnOffGreenLED(void)
{
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3, 0);
}



#endif /* MAIN_H_ */
