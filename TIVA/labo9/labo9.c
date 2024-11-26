Nuevo: Combinaciones de teclas … Se actualizaron las combinaciones de teclas de Drive para que puedas navegar con las primeras letras.
#include <stdint.h>
#include "stdio.h"
#include "stdlib.h"
#include <stdbool.h>
#include <string.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.c"
#include "driverlib/timer.h"

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    while(1);
}
#endif

void timer0A_handler(void);
void timer1A_handler(void);

uint32_t FS = 120000000;
uint8_t counter0 = 0;

char motor_dc[15];
uint32_t dc_int;
uint32_t prev_FS;

int main(void)
{      
    //Configuracion del reloj
    SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),120000000);
        
    // Enable the GPIO port  uart gpioa N F
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, 0x03);
    
    	// Enable timer0 peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    
    //set timer
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

    TimerLoadSet(TIMER0_BASE, TIMER_A, FS);
    
    IntMasterEnable();
    
    IntEnable(INT_TIMER0A);

    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    
    TimerEnable(TIMER0_BASE, TIMER_A);
    
    	// Enable timer1 peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    
    //set timer
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);

    TimerLoadSet(TIMER1_BASE, TIMER_A, 12000000);
    
    IntEnable(INT_TIMER1A);

    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    
    TimerEnable(TIMER1_BASE, TIMER_A);
    
    UARTStdioConfig(0,9600,120000000);

    //verificar que los perifericos esten activados
    

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0))
    {
    }
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
    {
    }
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION))
    {
    }
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }
       // Configuracion de los gpio de salida
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);
    
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
    

    while(1)
    {

    }

}

void timer0A_handler(void){
	TimerIntClear(TIMER0_BASE, TIMER_A);
	switch(++counter0) {
        case 1: 
        	GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);
		GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, 0x0);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0x0);
        	break;
        case 2: 
        	GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);
		GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, 0x0);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);
        	break;
        case 3: 
        	GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);
		GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_PIN_4);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0x0);
        case 4: 
        	GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0x0);
		GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0x0);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, 0x0);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0x0);
        	counter0 = 0; 
        	break;
    }
        
}
void timer1A_handler(void){
	prev_FS = FS;

    	UARTgets(motor_dc,15);
    
        dc_int = atoi(motor_dc);
        FS=120000000*dc_int;
        
        if (FS != prev_FS)
	{
		TimerLoadSet(TIMER0_BASE, TIMER_A, FS);
	}
        
        TimerIntClear(TIMER1_BASE, TIMER_A);
}
