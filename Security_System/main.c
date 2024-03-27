//Includes needed for application functionality
#include "my_gpio.h"
#include "initialization.h"

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
    InitConsole(); // UART initialization
    InitGPIO(); //GPIO initialization
    ADCInit(); //ADC initialization
    SysTickInitialization();

    //UARTprintf("Hello world!!!\n");
    while (1)
    {
        GetADC();
        //Wait(1000);

        return 0;
    }
}
