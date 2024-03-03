#include <stdint.h>
#include <stdbool.h>
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"

#define BAUD_RATE   9600

/* - UART0 peripheral
   - GPIO Port A peripheral (for UART0 pins)
   - UART0RX - PA0
   - UART0TX - PA1
*/

void InitUART0(void)
{
    /* Init UART0 */

    /*Enable GPIO Port A */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    /* Config pin 0 and 1 of Port A */
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    /* Enable UART0 */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    /* Set clock */
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    /* Select pins for UART */
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    /* Set baudrate: BAUD_RATE */
    UARTStdioConfig(0, BAUD_RATE, 16000000);

}

void UART_PutChar(uint8_t ch)
{
    UARTCharPut(UART0_BASE, ch);
}

uint8_t UART_GetChar(void)
{
   /* return UARTCharGetNonBlocking(UART0_BASE); */
    return UARTCharGet(UART0_BASE);
}

/**
 * main.c
 */
int main(void)
{
    uint8_t cmd;

    /* System clock settings */
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ); //changed from SYSCTL_SYSDIV_10 (CM)

    InitUART0();

    while(1)
    {
        cmd = UART_GetChar();
        if (cmd == 'r')
        {
            UART_PutChar('*');
        }
        else if (cmd == 'g')
        {
            UART_PutChar('-');
        }
    }

	return 0;
}
