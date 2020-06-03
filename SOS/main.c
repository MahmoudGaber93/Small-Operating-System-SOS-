/***********************************************************************
 *
 *	File Name: 		main.c
 *
 * 	Authors: 		Mahmoud Gaber
 *
 *	Created on:		Saturday, 11-JAN-20 10:15:18 PM
 *
 ************************************************************************/


/************************************************************************/
/*		                     INCLUDES	                                */
/************************************************************************/


#include "interrupt.h"
#include "app.h"
#include "Keypad.h"

/****error checking***/
/***to check the error read this array****/
uint8 gu8_checkerror_index2 =  SOS_ERROR_BASE + SOS_SUCSSECE ;
uint8 gau8_errorcheck2[256] = {0};


void main(void)
{
	//uint8 x;
	/****enable global interrupt ****/
	sei();
	//PORTB_DIR = 0xff;
	PORTC_DIR = 0xff;

	/****Initiate the app***/
	App_Init();
	/****call the scheduler of the os  ****/
	//ADC_Enable();
	//ADC_Start_Interrupt();

	//gu8_checkerror_index2=Sos_Run();
Sos_Run();

	while(1)
	{

	}
}

