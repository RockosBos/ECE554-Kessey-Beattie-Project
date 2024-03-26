#include "initialization.h"

int ADCInitComplete=0;
int ConsoleInitComplete=0;
int GPIOInitComplete=0;

void InitGPIO(void)
{
    //Port F***************************************************************************************
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

    //PortB***************************************************************************************
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
        ;
    //Set CR manually to allow changes to PF4-0
    //GPIO_PORTB_LOCK_R = 0x4C4F434B;
    //GPIO_PORTB_CR_R = INPUT_WINDOW|INPUT_DOOR|INPUT_GARAGE;
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, INPUT_WINDOW|INPUT_DOOR|INPUT_GARAGE);
    //Set internal pull-up register
    GPIOPadConfigSet(GPIO_PORTB_BASE, INPUT_WINDOW|INPUT_DOOR|INPUT_GARAGE, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
    //Interrupt
    GPIOIntTypeSet(GPIO_PORTB_BASE, INPUT_WINDOW|INPUT_DOOR|INPUT_GARAGE, GPIO_FALLING_EDGE);
    GPIOIntRegister(GPIO_PORTB_BASE, IntPortBHandler);
    GPIOIntEnable(GPIO_PORTB_BASE, INPUT_WINDOW|INPUT_DOOR|INPUT_GARAGE);

    GPIOInitComplete = 1;
    InitializationCheck(ADCInitComplete,ConsoleInitComplete,GPIOInitComplete);
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
    ConsoleInitComplete = 1;
    InitializationCheck(ADCInitComplete,ConsoleInitComplete,GPIOInitComplete);
}

void ADCInit(void) //ADC Measurements
{
    //ADC modules
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    //Analog Inputs
    GPIOPinTypeADC(GPIO_PORTE_BASE,GPIO_PIN_0); //PE0
    GPIOPinTypeADC(GPIO_PORTE_BASE,GPIO_PIN_1); //

    //Sample Sequencer
    ADCSequenceConfigure(ADC0_BASE, ADC_Sequence_Number, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE,ADC_Sequence_Number,0, ADC_CTL_CH3 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE,ADC_Sequence_Number);

    //ADC
    ADCIntClear(ADC0_BASE, ADC_Sequence_Number);

    ADCInitComplete = 1;
    InitializationCheck(ADCInitComplete,ConsoleInitComplete,GPIOInitComplete);
}

void InitializationCheck(ADCInitComplete,ConsoleInitComplete,GPIOInitComplete)
{
    if (ADCInitComplete & ConsoleInitComplete & GPIOInitComplete)
    {
        UARTprintf("***** Initialization Complete ******\n");
    }
    else
    {
        UARTprintf("Initializing...\n");
    }
}
