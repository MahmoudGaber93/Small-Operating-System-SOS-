/***********************************************************************
 *  Module Name : ADC
 *
 *	File Name   : ADC.h
 *
 * 	Authors     : Mahmoud Gaber
 *
 *	Created on  : MON, 19-JAN-20 10:15:18 PM
 *
 ************************************************************************/
#ifndef MCAL_ADC_ADC_H_
#define MCAL_ADC_ADC_H_

/************************************************************************/
/*				 INCLUDES			                                    */
/************************************************************************/

#include "std_types.h"
#include "common_macros.h"
#include "interrupt.h"
#include "DIO.h"

/************************************************************************/
/*				 DEFINES			        */
/************************************************************************/

#define ADC_PRESCALER_2			0
#define ADC_PRESCALER_4			1
#define ADC_PRESCALER_8			2
#define ADC_PRESCALER_16		3
#define ADC_PRESCALER_32		4
#define ADC_PRESCALER_64		5
#define ADC_PRESCALER_128		6

#define ADC_POLLING				(0U)
#define ADC_INTERRUPT			(1U)

#define ADC_SINGLE				0
#define ADC_AUTO_TRIG			1


#define ADC0_SINGLE		0
#define ADC1_SINGLE		1
#define ADC2_SINGLE		2
#define ADC3_SINGLE		3
#define ADC4_SINGLE		4
#define ADC5_SINGLE		5
#define ADC6_SINGLE		6
#define ADC7_SINGLE		7


#define FREERUN_MODE	0
#define ANALOGCOMP		1
#define EX_INTERRUPT	2
#define TIMER0			4
#define TIMER1			6

/************************************************************************/
/*			  Structures Declarations		                            */
/************************************************************************/
//typedef void (*PtrFunc)(void);
typedef struct str_ADC_cfg_t
{
	uint8 u8_Interrupt_Polling;
	uint8 u8_Single_AutoTrig;
	uint8 u8_PreScaler;
	uint8 u8_ADC_TrigSource;
	gptrFun_cbk_t PtrF_ADC_CBF;
}str_ADC_cfg_t;



/************************************************************************/
/*		         ADC FUNCTIONS PROTOTYPES		        */
/************************************************************************/


/**
 * Input: Pointer to a structure contains the information needed to initialize the ADC.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Initiates the module, it sets the pre-scaler, Right/Left adjustment, auto trigger, channels to work on.
 *
 */
extern ERROR_STATUS ADC_Init(str_ADC_cfg_t* str_ADC_cfg);

/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function enables the ADC
 **/
extern ERROR_STATUS ADC_Enable(void);
/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function starts the ADC conversion one time whenever called
 **/
extern ERROR_STATUS ADC_Start_Polling(uint8 u8_ADC_Ch, uint16 *ADC_Read);
/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function starts the ADC conversion one time whenever called
 * and uses module isr
 **/
extern ERROR_STATUS ADC_Start_Interrupt(uint8 u8_ADC_Ch);

/**
 * Input: Read variable pointer
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function Reads the ADC register
 **/
extern ERROR_STATUS ADC_Read(uint16* ADC_Read);


/**
 * Input: Auto Trigger Source for the ADC
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function sets the Auto Trigger Source for the ADC
 **/

extern ERROR_STATUS ADC_Set_AutoTrig(uint8 u8_ADC_Ch, uint8 u8_AutoTrigSource);

/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function stops the auto triggered ADC conversion
 *
 */

extern ERROR_STATUS ADC_Stop_AutoTrig(void);

/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function stops the call of the given function.
 *
 */
extern ERROR_STATUS ADC_Stop(void);

/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function Disables the ADC conversion one time whenever called
 **/
extern ERROR_STATUS ADC_Disable(void);


#endif /* MCAL_ADC_ADC_H_ */
