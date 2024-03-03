/*
 * my_gpio.c
 *
 *  Created on: Apr 10, 2023
 *      Author: arche
 */

#include "my_gpio.h"

void IntPortFHandler(void)
{
    uint32_t status = GPIOIntStatus(GPIO_PORTF_BASE, true);

    if (status & SW1_INT_PIN) {
        GPIOPinWrite(GPIO_PORTF_BASE, LED_RED_PIN, LED_RED_PIN);
    }
    if (status & SW2_INT_PIN) {
        GPIOPinWrite(GPIO_PORTF_BASE, LED_RED_PIN, 0);
    }

    /* clear flag */
    GPIOIntClear(GPIO_PORTF_BASE, status);
}

void InitGPIO(void)
{
    /* set the system clock 80MHz */
    SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    /* gpio configuration */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
        ;

    /* Set CR manually to allow changes to PF4-0 */
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = LED_RED_PIN|LED_BLUE_PIN|LED_GREEN_PIN|SW1_PIN|SW2_PIN;

    /* GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); */ /* 1110 */
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_RED_PIN|LED_BLUE_PIN|LED_GREEN_PIN);

    /* SW1, SW2 support */
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, SW1_PIN|SW2_PIN);
    /* Set internal pull-up register */
    GPIOPadConfigSet(GPIO_PORTF_BASE, SW1_PIN|SW2_PIN, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);

    /* interrupt */
    GPIOIntTypeSet(GPIO_PORTF_BASE, SW1_PIN|SW2_PIN, GPIO_RISING_EDGE);
    GPIOIntRegister(GPIO_PORTF_BASE, IntPortFHandler);
    GPIOIntEnable(GPIO_PORTF_BASE, SW1_INT_PIN|SW2_INT_PIN);
}
/* these are unnecessary now due to interrupt implementation
bool Switch1Pressed(void)
{
    int32_t raw;
    raw = GPIOPinRead(GPIO_PORTF_BASE, SW1_PIN);
    if (raw == 0)
        return true;
    else
        return false;
}

bool Switch2Pressed(void)
{
    int32_t raw;
    raw = GPIOPinRead(GPIO_PORTF_BASE, SW2_PIN);
    if (raw == 0)
        return true;
    else
        return false;
}

*/

void TurnOnRedLED(void)
{
    GPIOPinWrite(GPIO_PORTF_BASE, LED_RED_PIN, LED_RED_PIN);
}
void TurnOffRedLED(void)
{
    GPIOPinWrite(GPIO_PORTF_BASE, LED_RED_PIN,0);
}

void TurnOnBlueLED(void)
{
    GPIOPinWrite(GPIO_PORTF_BASE, LED_BLUE_PIN, LED_BLUE_PIN);
}
void TurnOffBlueLED(void)
{
    GPIOPinWrite(GPIO_PORTF_BASE, LED_BLUE_PIN, 0);
}

void TurnOnGreenLED(void)
{
    GPIOPinWrite(GPIO_PORTF_BASE, LED_GREEN_PIN, LED_GREEN_PIN);
}
void TurnOffGreenLED(void)
{
    GPIOPinWrite(GPIO_PORTF_BASE, LED_GREEN_PIN, 0);
}
