
/*
*------------------------------------------------------------------------------
* main.c
*
*------------------------------------------------------------------------------
*/






/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/

#include <timers.h>				// Timer library functions
#include <delays.h>				// Delay library functions
#include <string.h>				// String library functions
#include "board.h"				// board setup
#include "timer.h"
#include "heartbeat.h"
#include "app.h"
#include "rtc_driver.h"
#include "i2c_driver.h"
#include "digitdisplay.h"
#include "linearkeypad.h"
/*
*------------------------------------------------------------------------------
* Private Defines
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Processor config bits
*------------------------------------------------------------------------------
*/


#pragma config FOSC 	= INTIO67
#pragma config PLLCFG 	= OFF
#pragma config PRICLKEN = OFF
#pragma config FCMEN 	= OFF
#pragma config IESO 	= OFF
#pragma config PWRTEN   = OFF
#pragma config BOREN 	= OFF
#pragma config BORV 	= 285
#pragma config WDTEN    = OFF
#pragma config WDTPS 	= 512
#pragma config CCP2MX 	= PORTC1
#pragma config PBADEN 	= OFF

#pragma config MCLRE 	= EXTMCLR
#pragma config STVREN 	= OFF
#pragma config XINST 	= OFF
#pragma config LVP      = OFF
#pragma config CP0      = OFF
#pragma config CP1      = OFF
#pragma config CP2      = OFF
#pragma config CP3      = OFF
#pragma config CPB      = OFF
#pragma config CPD      = OFF
#pragma config WRT0     = OFF
#pragma config WRT1     = OFF
#pragma config WRT2     = OFF
#pragma config WRT3     = OFF
#pragma config WRTB     = OFF       
#pragma config WRTC     = OFF
#pragma config WRTD     = OFF
#pragma config EBTR0    = OFF
#pragma config EBTR1    = OFF
#pragma config EBTR2    = OFF
#pragma config EBTR3    = OFF
#pragma config EBTRB    = OFF
/*
*------------------------------------------------------------------------------
* Private Macros
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Data Types
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Variables
*------------------------------------------------------------------------------
*/
extern UINT16 heartBeatCount;
extern UINT16 keypadUpdate_count;
extern UINT16 comUpdateCount;

/*

*------------------------------------------------------------------------------
* Private Variables (static)
*------------------------------------------------------------------------------
*/
UINT8 message[]="IDEONICS";
UINT8 tempBuffer[]={'3','6','2','7'};
/*
*------------------------------------------------------------------------------
* Public Constants
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Constants (static)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Function Prototypes (static)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Functions
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Functions
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* void main(void)

* Summary	: Application specifc main routine. Initializes all port and
*			: pheriperal and put the main task into an infinite loop.
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
#define DIGIT_REFRESH_PERIOD	(65535 - 2000)
#define TICK_PERIOD	(65535 - 4000)
#define TIMER2_RELOAD (30) //250us

void main(void)
{
	UINT8 count=0, i, uartData;
	UINT8 blink = 0;

	BRD_init();			//board initialization
	InitializeI2C();
	InitializeRtc();	//RTC Initialization
	
	DigitDisplay_init(7); //Digit Display initialization

	TMR0_init(TICK_PERIOD,0);		//initialize timer0
//	TMR1_init(DIGIT_REFRESH_PERIOD,DigitDisplay_task);	// Timer1 initialization	
	TMR2_init(TIMER2_RELOAD,DigitDisplay_task);		//initialize timer2			

	APP_init();
	EnableInterrupts();		//Interrupts initialization


	while(1)
	{

	//	COM_task();

	`	if(heartBeatCount >= 500 )
		{			
			HB_task();
			heartBeatCount = 0;
		}

		if(keypadUpdate_count >= 10)
		{
			LinearKeyPad_scan();
			keypadUpdate_count = 0;
			count++;
		}
		
		if(count >= 10)
		{
			APP_task();	
			count = 0;
		}
		
	}


}

/*
*  End of main.c
*/
