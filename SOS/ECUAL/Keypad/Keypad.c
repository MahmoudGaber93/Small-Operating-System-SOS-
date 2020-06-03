/***********************************************************************
 *  Module Name : Keypad
 *
 *	File Name   : Keypad.c
 *
 * 	Authors     : Mahmoud Gaber
 *
 *	Created on  : MON, 19-JAN-20 10:15:18 PM
 *
 ************************************************************************/
/************************************************************************/
/*				 INCLUDES			                                    */
/************************************************************************/
#include "Keypad.h"

/************************************************************************/
/*				 DEFINES			        */
/************************************************************************/
#define PRESSED     1
#define NOT_PRESSED 0

#define ROWS 3
#define COL  3

#define  Switch_Pressed    0
#define  Switch_depouncing 1

#define KEYPAD_NOT_INITIALIZED  0
#define KEYPAD_INITIALIZED      1

/************************************************************************/
/*				 GLOBALE VARIABLES		        */
/************************************************************************/

uint8 arr_col[3]= {BIT0,BIT1,BIT2};
uint8 arr_row[3]= {BIT3,BIT4,BIT5};

uint8 gu8_ROW =0;
uint8 gu8_COL =0;
uint8 gu8_state = Switch_Pressed;

uint8 ga8_keypadnum[9] = {1,2,3,4,5,6,7,8,9};
uint8 ga8_Keypad_state[9] = {0};

uint8 gu8_Keypad_initState = KEYPAD_NOT_INITIALIZED;
uint8 gu8_keyState=NOT_PRESSED;


/************************************************************************/
/*		         FUNCTIONS IMPLIMENTATION           		        */
/************************************************************************/



/**
 * Input:NONE
 * Output:NONE
 * In/Out:
 * Return: The error status of the function.
 * Description: Initiates the module.
 *
 */

ERROR_STATUS Keypad_Init(void)
{
	DIO_Cfg_s str_keypadRow_cfg;
	str_keypadRow_cfg.GPIO = GPIOD;
	str_keypadRow_cfg.pins = 0b00111000;
	str_keypadRow_cfg.dir = INPUT;
	DIO_init(&str_keypadRow_cfg);
	DIO_Write(GPIOD ,BIT3|BIT4|BIT5 ,HIGH);


	DIO_Cfg_s str_keypadCol_cfg;
	str_keypadCol_cfg.GPIO = GPIOD;
	str_keypadCol_cfg.pins = 0b00000111;
	str_keypadCol_cfg.dir = OUTPUT;
	DIO_init(&str_keypadCol_cfg);
	DIO_Write(GPIOD ,BIT0|BIT1|BIT2 ,HIGH);

	gu8_Keypad_initState =KEYPAD_INITIALIZED;

	/**return error state***/
	return E_OK;

}

/**
 * Input:NONE
 * Output:NONE
 * In/Out:
 * Return: The error status of the function.
 * Description: update the module.
 *
 */
ERROR_STATUS Keypad_Update(void)
{
	if(gu8_Keypad_initState ==KEYPAD_INITIALIZED)
	{
		uint8 count1 =0;
		uint8 count2 =0;
		uint8 value_button=HIGH;
		uint8 index=0;
		switch(gu8_state)
		{
		case Switch_Pressed:
			for(count1 =0; value_button != LOW && count1 < COL; count1++)
			{
				DIO_Write(GPIOD ,arr_col[count1] , LOW);
				count2 = 0;
				while( value_button != LOW && count2 < ROWS)
				{
					DIO_Read(GPIOD , arr_row[count2] , &value_button);
					if(value_button == LOW)
					{
						gu8_state = Switch_depouncing;
					}
					else
					{


					}
					count2++;
				}
				DIO_Write(GPIOD, arr_col[count1] , HIGH);
			}
			break;
		case Switch_depouncing:
			for(count1 =0; value_button != LOW && count1 < COL; count1++)
			{
				DIO_Write(GPIOD ,arr_col[count1] , LOW);
				count2 = 0;
				while( value_button != LOW && count2 < ROWS)
				{
					DIO_Read(GPIOD , arr_row[count2] , &value_button);
					if(value_button == LOW)
					{
						//index = count2 +count1+(count1*2);
						index= count1+(count2*COL);
						ga8_Keypad_state[index] = PRESSED;
						gu8_keyState=PRESSED;
					}
					else
					{
						ga8_Keypad_state[index] = NOT_PRESSED;
					}
					count2++;
				}
				DIO_Write(GPIOD , arr_col[count1] , HIGH);
			}
			gu8_state = Switch_Pressed;

			break;
		default:
			return E_NOK;
			break;

		}

	}
	else
	{
		return E_NOK;
	}

	/**return error state***/
	return E_OK;

}

/**
 * Input:NONE
 * Output:NONE
 * In/Out:
 * Return: The error status of the function.
 * Description: get the value of the pressed key.
 *
 */
ERROR_STATUS Keypad_GetKeyPressed(uint8 * key)
{
	if(gu8_Keypad_initState ==KEYPAD_INITIALIZED)
	{
		if(gu8_keyState==PRESSED)
		{
			gu8_keyState=NOT_PRESSED;
			volatile uint8 i ;
			for(i =0 ; i<9 ;i++)
			{
				if(ga8_Keypad_state[i] == PRESSED)
				{
					*key = ga8_keypadnum[i];
					ga8_Keypad_state[i] = NOT_PRESSED;

				}
				else
				{
					/***not pressed***/

				}
			}
		}
		else
		{
			*key = 0xab;
		}
	}
	else
	{
		return E_NOK;
	}
	/**return error state***/
	return E_OK;

}
