/***********************************************************************
 *  Module Name : Keypad
 *
 *	File Name   : Keypad.h
 *
 * 	Authors     : Mahmoud Gaber
 *
 *	Created on  : MON, 19-JAN-20 10:15:18 PM
 *
 ************************************************************************/
#ifndef ECUAL_KEYPAD_KEYPAD_H_
#define ECUAL_KEYPAD_KEYPAD_H_

/************************************************************************/
/*				 INCLUDES			                                    */
/************************************************************************/
#include "DIO.h"


/************************************************************************/
/*		         ADC FUNCTIONS PROTOTYPES		        */
/************************************************************************/


/**
 * Input:NONE
 * Output:NONE
 * In/Out:
 * Return: The error status of the function.
 * Description: Initiates the module.
 *
 */
ERROR_STATUS Keypad_Init(void);



/**
 * Input:NONE
 * Output:NONE
 * In/Out:
 * Return: The error status of the function.
 * Description: update the module.
 *
 */
ERROR_STATUS Keypad_Update(void);


/**
 * Input:NONE
 * Output:NONE
 * In/Out:
 * Return: The error status of the function.
 * Description: get the value of the pressed key.
 *
 */
ERROR_STATUS Keypad_GetKeyPressed(uint8 * key);




#endif /* ECUAL_KEYPAD_KEYPAD_H_ */
