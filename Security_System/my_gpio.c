#include "my_gpio.h"

void IntPortFHandler(void)
{
    uint32_t status = GPIOIntStatus(GPIO_PORTF_BASE, true);

    if (status & SW1_INT_PIN) {
        GPIOPinWrite(GPIO_PORTF_BASE, LED_RED_PIN, LED_RED_PIN);
        UARTprintf("***SW1 was pressed***\n");
    }
    if (status & SW2_INT_PIN) {
        GPIOPinWrite(GPIO_PORTF_BASE, LED_RED_PIN, 0);
        UARTprintf("***SW2 was pressed***\n");
    }

    GPIOIntClear(GPIO_PORTF_BASE, status); //clear flag
}

void IntPortBHandler(void)
{
    uint32_t status = GPIOIntStatus(GPIO_PORTB_BASE, true);
    if (status & INPUT_WINDOW)
    {
        UARTprintf("***Pin PB5 Interrupt***\n"); //Pin PB5
    }
    if (status & INPUT_DOOR)
    {
        UARTprintf("***Pin PB6 Interrupt***\n"); //Pin PB6
    }
    if (status & INPUT_GARAGE)
    {
        UARTprintf("***Pin PB7 Interrupt***\n"); //Pin PB7
    }

    GPIOIntClear(GPIO_PORTB_BASE, status); //clear flag
}
