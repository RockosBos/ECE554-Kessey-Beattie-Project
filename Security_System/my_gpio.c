#include "my_gpio.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"

#include "driverlib/pin_map.h" //Needed for UART
#include "driverlib/uart.h" //Needed for UART
#include "utils/uartstdio.h" //Needed for UART


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
    //SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_25MHZ |SYSCTL_OSC_MAIN);

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

void ADCFunc(void)
{
    //Clock
    //SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    //ADC modules
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    //Analog Inputs
    GPIOPinTypeADC(GPIO_PORTE_BASE,GPIO_PIN_0); //PE0

    //Sample Sequencer
    ADCSequenceConfigure(ADC0_BASE, ADC_Sequence_Number, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE,ADC_Sequence_Number,0, ADC_CTL_CH3 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE,ADC_Sequence_Number);

    //ADC
    ADCIntClear(ADC0_BASE, ADC_Sequence_Number);
}
