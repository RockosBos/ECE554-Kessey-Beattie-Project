#include "main.h"
/**
 * main.c
 */
int main(void)
{
    InitGPIO();

    while(1)
    {
        TurnOnGreenLED();
        SysCtlDelay(ONE_SEC);
        TurnOffGreenLED();
        SysCtlDelay(ONE_SEC);
    }

	return 0;
}
