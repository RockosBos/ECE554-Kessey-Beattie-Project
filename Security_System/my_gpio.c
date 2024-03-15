#include "my_gpio.h"
#include "utils/uartstdio.h"

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

    //clear flag
    GPIOIntClear(GPIO_PORTF_BASE, status);
}

void InitGPIO(void)
{
    //Set the system clock to 80MHz
    SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    //GPIO configuration
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
        ;

    //Set CR manually to allow changes to PF4-0
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = LED_RED_PIN|LED_BLUE_PIN|LED_GREEN_PIN|SW1_PIN|SW2_PIN;

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_RED_PIN|LED_BLUE_PIN|LED_GREEN_PIN);

    //SW1, SW2 support
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, SW1_PIN|SW2_PIN);
    //Set internal pull-up register
    GPIOPadConfigSet(GPIO_PORTF_BASE, SW1_PIN|SW2_PIN, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);

    //Interrupt
    GPIOIntTypeSet(GPIO_PORTF_BASE, SW1_PIN|SW2_PIN, GPIO_RISING_EDGE);
    GPIOIntRegister(GPIO_PORTF_BASE, IntPortFHandler);
    GPIOIntEnable(GPIO_PORTF_BASE, SW1_INT_PIN|SW2_INT_PIN);
}
