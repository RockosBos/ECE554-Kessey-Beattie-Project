#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.c"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.c"
#include "driverlib/gpio.h"

#define RED_LED_PIN GPIO_PIN_1
#define BLUE_LED_PIN GPIO_PIN_2
#define GREEN_LED_PIN GPIO_PIN_3
#define SW1_PIN GPIO_PIN_4
#define SW2_PIN GPIO_PIN_0 ///////////////////////////////////////////

#define SW1_INT_PIN GPIO_INT_PIN_4

void PortFIntHandler(void);
void InitBoard(void);

void InitBoard(void)
{
//Set the clock to 80Mhz
SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL
|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
SysCtlDelay(3);
GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,
RED_LED_PIN|BLUE_LED_PIN|GREEN_LED_PIN);
GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, SW1_PIN|SW2_PIN);

GPIOPadConfigSet(GPIO_PORTF_BASE, SW1_PIN|SW2_PIN,
GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
GPIOIntTypeSet(GPIO_PORTF_BASE, SW1_PIN, GPIO_FALLING_EDGE);
GPIOIntRegister(GPIO_PORTF_BASE, PortFIntHandler);
GPIOIntEnable(GPIO_PORTF_BASE, SW1_INT_PIN);
}
void PortFIntHandler(void)
{
static bool led_on = false;
uint32_t status = GPIOIntStatus(GPIO_PORTF_BASE, true);
if (status & SW1_INT_PIN)
{
if (led_on) {
GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED_PIN, 0);
led_on = false;
}
else {
GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED_PIN, GREEN_LED_PIN);
led_on = true;
}
}
GPIOIntClear(GPIO_PORTF_BASE, status);
}

int main(void)
{
InitBoard();
while (1)
{}
return 0;
}

