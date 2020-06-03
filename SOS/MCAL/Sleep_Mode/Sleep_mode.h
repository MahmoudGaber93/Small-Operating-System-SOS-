/******************************************************************************
 *
 * 	Module: 		Sleep_Mode
 *
 *	File Name: 		Sleep_Mode.h
 *
 * 	Authors: 		Mahmoud Gaber
 *
 *	Created on:		Saturday, 11-JAN-20 10:15:18 PM
 *
 *******************************************************************************/
#ifndef MCAL_SLEEP_MODE_H_
#define MCAL_SLEEP_MODE_H_


/************************************************************************/
/*	                        	 INCLUDES		            	        */
/************************************************************************/

#include "registers.h"


/************************************************************************/
/*		         TIMER FUNCTIONS' PROTOTYPES		                    */
/************************************************************************/

/**
 * Input: NONE.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: start the sleep mode.
 *
 */
ERROR_STATUS Sleep_Cpu(void);

#endif /* MCAL_SLEEP_MODE_H_ */
