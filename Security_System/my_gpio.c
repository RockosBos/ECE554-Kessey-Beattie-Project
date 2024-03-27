#include "my_gpio.h"


volatile uint32_t timer_count = 0; ////////////////////////////////////////////////////////////////////////////////////////////////
volatile uint32_t millisecond = 0;
volatile uint32_t second = 0;
volatile uint32_t minute = 0;

//Initializations
int alert = 1; //active

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

void SysTick_Handler(void)
{
    //SysTickIntDisable();
    timer_count++;
    millisecond = timer_count;
    if (millisecond == 1000)
    {
        timer_count = 0;
        millisecond = 0;
        second++;
        PrintTime();
    }
    if (second == 60)
    {
        second = 0;
        minute++;
    }
    //SysTickIntEnable();
}

void Wait(uint32_t time) //Delay function
{
    uint32_t temp = timer_count;
    while( (timer_count-temp) < time)
    {
    }
}

void PrintTime()
{
    SysTickIntDisable();
    UARTprintf("%2d:%2d:%2d\n", minute, second, millisecond);
    SysTickIntEnable();
}

//ADC Measurement
void GetADC()
{
    ADCProcessorTrigger(ADC0_BASE, ADC_Sequence_Number); //Triggers ADC conversion
    while(!ADCIntStatus(ADC0_BASE, ADC_Sequence_Number, false)) // Wait for conversion to be completed.
    {}
    ADCIntClear(ADC0_BASE, ADC_Sequence_Number);  // Clear the ADC interrupt flag.

    uint32_t adc_Val;
    ADCSequenceDataGet(ADC0_BASE, ADC_Sequence_Number, &adc_Val);  // Read ADC Values.
    float Voltage = (adc_Val*ADC_REF_VOL)/ADC_MAX;
    //float Voltage2 =
    UARTprintf("adc read");

    if(Voltage>=1.0)
    {
        UARTprintf("***** INTRUDER ALERT, GET OUT OF MY HOUSE ******\n");
        GPIOPinWrite(GPIO_PORTF_BASE, LED_BLUE_PIN, 0);
        GPIOPinWrite(GPIO_PORTF_BASE, LED_RED_PIN, LED_RED_PIN);
        uint32_t clk = SysCtlClockGet();
        //SysCtlDelay(SysCtlClockGet()/10);
        GPIOPinWrite(GPIO_PORTF_BASE, LED_RED_PIN, 0);
        GPIOPinWrite(GPIO_PORTF_BASE, LED_BLUE_PIN, LED_BLUE_PIN);
        //SysCtlDelay(SysCtlClockGet()/10);
        alert = 1;
    }
    else if (Voltage<1 & alert == 1)
    {
        GPIOPinWrite(GPIO_PORTF_BASE, LED_RED_PIN, 0);
        GPIOPinWrite(GPIO_PORTF_BASE, LED_BLUE_PIN, 0);
        alert = 0;
    }
}
