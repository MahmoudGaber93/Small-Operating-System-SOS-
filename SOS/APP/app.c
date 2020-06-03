/******************************************************************************
 *
 * 	Module: 		APP
 *
 *	File Name: 		app.c
 *
 * 	Authors: 		Mahmoud Gaber
 *
 *	Created on:		Saturday, 11-JAN-20 10:15:18 PM
 *
 *******************************************************************************/

/************************************************************************/
/*	                        	 INCLUDES		            	        */
/************************************************************************/

#include "app.h"
#include "Sos.h"
#include "Keypad.h"
#include "char_lcd.h"
#include "ADC.h"


/*********************************************************************
 * Input: NONE.
 * Output:
 * In/Out:
 * Return: ERROR_STATUS.
 * Description: initialize the app.
 *
 * *******************************************************************/

/****error checking***/
/***to check the error read this array****/
uint8 gau8_errorcheck1[20] = {0};
uint8 gu8_checkerror_index = 0;
uint16 gu16_ADCValue = 0;
uint16 gu16_VoltageValue=0;
uint8 gu8_TEMP = 0;

uint8 gu8_KEY =0;
ERROR_STATUS App_Init(void)
{
	gau8_errorcheck1[gu8_checkerror_index++] = Sos_Init();
	Led_Init(LED_0);
	Led_Init(LED_1);
	Led_Init(LED_2);
	/****initiate the Keypad****/
	Keypad_Init();
	/****initiate the Lcd****/

	LCD_init();

	str_ADC_cfg_t str_ADC;
	str_ADC.PtrF_ADC_CBF = App_4;
	str_ADC.u8_ADC_TrigSource = FREERUN_MODE;
	str_ADC.u8_PreScaler = ADC_PRESCALER_16;
	str_ADC.u8_Single_AutoTrig = ADC0_SINGLE;
	str_ADC.u8_Interrupt_Polling = ADC_INTERRUPT;
	/****initiate the ADC****/

	ADC_Init(&str_ADC);


	str_SOSfg_t Fun1;
	Fun1.Sos_AppTask_cbk = App_1;
	Fun1.Task_State = IDEL;
	Fun1.counter=0;
	Fun1.periodicity =4;
	Fun1.Priority = 8;
	gau8_errorcheck1[gu8_checkerror_index++] = Sos_Creat_Task(&Fun1);

	str_SOSfg_t Fun2;
	Fun2.Sos_AppTask_cbk = App_2;
	Fun2.Task_State = IDEL;
	Fun2.counter=0;
	Fun2.periodicity =100;
	Fun2.Priority = 7;

	gau8_errorcheck1[gu8_checkerror_index++] = Sos_Creat_Task(&Fun2);

	str_SOSfg_t Fun3;
	Fun3.Sos_AppTask_cbk = App_3;
	Fun3.Task_State = IDEL;
	Fun3.counter=0;
	Fun3.periodicity =5;
	Fun3.Priority = 6;

	gau8_errorcheck1[gu8_checkerror_index++] = Sos_Creat_Task(&Fun3);

	str_SOSfg_t Fun_4;
	Fun_4.Sos_AppTask_cbk = App_4;
	Fun_4.Task_State = IDEL;
	Fun_4.counter=0;
	Fun_4.periodicity =10;
	Fun_4.Priority = 5;

	//gau8_errorcheck1[gu8_checkerror_index++] = Sos_Creat_Task(&Fun_4);

	str_SOSfg_t Fun_5;
	Fun_5.Sos_AppTask_cbk = App_5;
	Fun_5.Task_State = IDEL;
	Fun_5.counter=0;
	Fun_5.periodicity =10;
	Fun_5.Priority = 4;

	//gau8_errorcheck1[gu8_checkerror_index++] = Sos_Creat_Task(&Fun_5);

	return E_OK;

}


void App_1(void)
{
	Keypad_Update();

}

void App_2(void)
{
	Keypad_GetKeyPressed(&gu8_KEY);
	/*if(gu8_KEY == 1)
	{
		PORTC_DATA ^= 0b00000001;

	}
	else if(gu8_KEY == 2)
	{
		PORTC_DATA ^= 0b00000010;

	}
	else if(gu8_KEY == 3)
	{
		PORTC_DATA ^= 0b00000100;

	}
	else if(gu8_KEY == 4)
	{
		PORTC_DATA ^= 0b00001000;

	}
	else if(gu8_KEY == 5)
	{
		PORTC_DATA ^= 0b00010000;

	}
	else if(gu8_KEY == 6)
	{
		PORTC_DATA ^= 0b00100000;

	}
	else if(gu8_KEY == 7)
	{
		PORTC_DATA ^= 0b01000000;

	}
	else if(gu8_KEY == 8)
		{
		PORTC_DATA ^= 0b10000000;

		}*/
}
void App_3(void)
{
	LCD_send_char(gu8_KEY+48);

}



void App_4(void)
{

	ADC_Read(&gu16_ADCValue);
	gu16_VoltageValue = (uint16)(((uint32)gu16_ADCValue * 2560) /1024);
	gu8_TEMP = gu16_VoltageValue/10;

}
void App_5(void)
{

	LCD_send_command(Lcd_clear);
}
