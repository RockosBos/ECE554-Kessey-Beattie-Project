#include "my_gpio.h" //originally here
#include "driverlib/pin_map.h" //Needed for UART
#include "driverlib/uart.h" //Needed for UART
#include "utils/uartstdio.h" //Needed for UART

//Prototypes
void InitConsole(void);

int main(void)
{
    InitGPIO();
    // UART initialization
    InitConsole();
    UARTprintf("Hello world!!!\n");
    while (1)
    {
        /* */
    }

    return 0;
}

void InitConsole(void)
{
    // Enable GPIO port A which is used for UART0 pins.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Configure the pin muxing for UART0 functions on port A0 and A1.
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    // Enable UART0 so that we can configure the clock.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    // Use the internal 16MHz oscillator as the UART clock source.
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

     // Select the alternate (UART) function for these pins.
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Initialize the UART for console I/O. 9600 BAUD
    UARTStdioConfig(0, 9600, 16000000);
}
