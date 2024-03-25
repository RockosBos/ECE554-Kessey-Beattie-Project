//Includes needed for application functionality
#include "my_gpio.h"
#include "initialization.h"

//Initializations
int alert = 1; //active

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_25MHZ |SYSCTL_OSC_MAIN);
    InitGPIO(); //GPIO initialization
    ADCInit(); //ADC initialization
    InitConsole(); // UART initialization

    UARTprintf("Hello world!!!\n");
    while (1)
    {
        ADCProcessorTrigger(ADC0_BASE, ADC_Sequence_Number); //Triggers ADC conversion
        while(!ADCIntStatus(ADC0_BASE, ADC_Sequence_Number, false)) // Wait for conversion to be completed.
        {}
        ADCIntClear(ADC0_BASE, ADC_Sequence_Number);  // Clear the ADC interrupt flag.

        uint32_t adc_Val;
        ADCSequenceDataGet(ADC0_BASE, ADC_Sequence_Number, &adc_Val);  // Read ADC Values.
        float Voltage = (adc_Val*ADC_REF_VOL)/ADC_MAX;
        //float Voltage2 =
        //UARTprintf("Voltage = %f\n",Voltage);

        if(Voltage>=1.0)
        {
            UARTprintf("***** INTRUDER ALERT, GET OUT OF MY HOUSE ******\n");
            GPIOPinWrite(GPIO_PORTF_BASE, LED_BLUE_PIN, 0);
            GPIOPinWrite(GPIO_PORTF_BASE, LED_RED_PIN, LED_RED_PIN);
            uint32_t clk = SysCtlClockGet();
            SysCtlDelay(SysCtlClockGet()/10);
            GPIOPinWrite(GPIO_PORTF_BASE, LED_RED_PIN, 0);
            GPIOPinWrite(GPIO_PORTF_BASE, LED_BLUE_PIN, LED_BLUE_PIN);
            SysCtlDelay(SysCtlClockGet()/10);
            alert = 1;
        }
        else if (Voltage<1 & alert == 1)
        {
            GPIOPinWrite(GPIO_PORTF_BASE, LED_RED_PIN, 0);
            GPIOPinWrite(GPIO_PORTF_BASE, LED_BLUE_PIN, 0);
            alert = 0;
        }
    }

    return 0;
}
