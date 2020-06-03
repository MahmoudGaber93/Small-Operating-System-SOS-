/***********************************************************************
 *  Module Name : ADC
 *
 *	File Name   : ADC.c
 *
 * 	Authors     : Mahmoud Gaber
 *
 *	Created on  : MON, 19-JAN-20 10:15:18 PM
 *
 ************************************************************************/

#include "registers.h"
#include "ADC.h"




uint8 ga8_Prescaler_LUT[7] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06};
uint8 ga8_TriggerSources_LUT[7] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06};
uint8 ga8_SingleAutotrigger_LUT[8] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};


gptrFun_cbk_t ADC_CBF = NULL;
/************************************************************************/
/*		         ADC FUNCTIONS IMPLIMENTATION           		        */
/************************************************************************/


/**
 * Input: Pointer to a structure contains the information needed to initialize the ADC.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Initiates the module, it sets the pre-scaler, Right/Left adjustment, auto trigger, channels to work on.
 *
 */
extern ERROR_STATUS ADC_Init(str_ADC_cfg_t* str_ADC_cfg)
{
	DIO_Cfg_s  str_ADC;
	str_ADC.GPIO = GPIOA;
	str_ADC.pins = FULL_PORT;
	str_ADC.dir = INPUT;
	DIO_init(&str_ADC);

	ADCSRA |= ga8_Prescaler_LUT[str_ADC_cfg->u8_PreScaler];

	switch(str_ADC_cfg->u8_Interrupt_Polling)
	{
	case ADC_INTERRUPT:
		ADCSRA |=  0b00001000;
		break;
	case ADC_POLLING:
		ADCSRA &=  0b11110111;
		break;
	default:
		/***return error**/
		return E_NOK;
		break;
	}
	/***sitting trigger source***/

	SFIOR |=  ga8_TriggerSources_LUT[str_ADC_cfg->u8_ADC_TrigSource];


	ADMUX |= ga8_SingleAutotrigger_LUT[str_ADC_cfg->u8_Single_AutoTrig];

	ADC_CBF = str_ADC_cfg->PtrF_ADC_CBF;

	return E_OK;
}

/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function enables the ADC
 **/
extern ERROR_STATUS ADC_Enable(void)
{
	ADCSRA |= 0b10000000;
	return E_OK;

}
/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function starts the ADC conversion one time whenever called
 **/
extern ERROR_STATUS ADC_Start_Polling(uint8 u8_ADC_Ch, uint16 *ADC_Read)
{
	return E_OK;

}
/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function starts the ADC conversion one time whenever called
 * and uses module isr
 **/
extern ERROR_STATUS ADC_Start_Interrupt(uint8 u8_ADC_Ch)
{
	ADCSRA |= 0b01000000;
	return E_OK;


}

/**
 * Input: Read variable pointer
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function Reads the ADC register
 **/
extern ERROR_STATUS ADC_Read(uint16* ADC_Read)
{
	if(ADC_Read != NULL)
	{
		*ADC_Read = ADC;
	}
	else
	{
		return E_NOK;
	}
	return E_OK;

}


/**
 * Input: Auto Trigger Source for the ADC
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function sets the Auto Trigger Source for the ADC
 **/

extern ERROR_STATUS ADC_Set_AutoTrig(uint8 u8_ADC_Ch, uint8 u8_AutoTrigSource)
{
	ADCSRA |= 0b00100000;
	return E_OK;

}

/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function stops the auto triggered ADC conversion
 *
 */

extern ERROR_STATUS ADC_Stop_AutoTrig(void)
{
	ADCSRA &= 0b11011111;
	return E_OK;

}

/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function stops the call of the given function.
 *
 */
extern ERROR_STATUS ADC_Stop(void)
{
	return E_OK;

}

/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function Disables the ADC conversion one time whenever called
 **/
extern ERROR_STATUS ADC_Disable(void)
{
	ADCSRA &= 0b0111111;


	return E_OK;

}


ISR(ADC_vect)
{
	ADC_CBF();

}
