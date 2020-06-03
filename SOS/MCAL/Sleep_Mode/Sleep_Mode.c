/******************************************************************************
 *
 * 	Module: 		Sleep_Mode
 *
 *	File Name: 		Sleep_Mode.c
 *
 * 	Authors: 		Mahmoud Gaber
 *
 *	Created on:		Saturday, 11-JAN-20 10:15:18 PM
 *
 *******************************************************************************/


/************************************************************************/
/*	                        	 INCLUDES		            	        */
/************************************************************************/

#include "Sleep_mode.h"



/************************************************************************/
/*		             TIMER FUNCTIONS' IMPLIMENTATION		            */
/************************************************************************/
/**
 * Input: NONE.
 *
 * Return: The error status of the function.
 *
 * Description: start the sleep mode.
 *
 */
ERROR_STATUS Sleep_Cpu(void)
{

	/******Idle sleep mode******/
     MCUCR = 0x8f;
     asm("SLEEP");
     return E_OK;
}
