/******************************************************************************
 *
 * 	Module: 		SOS
 *
 *	File Name: 		Sos.h
 *
 * 	Authors: 		Mahmoud Gaber
 *
 *	Created on:		Saturday, 11-JAN-20 10:15:18 PM
 *
 *******************************************************************************/
#ifndef SERVICE_LAYER_SOS_SOS_H_
#define SERVICE_LAYER_SOS_SOS_H_


/************************************************************************/
/*	                        	 INCLUDES		            	        */
/************************************************************************/

#include "std_types.h"
#include "common_macros.h"
#include "error_check.h"




/************************************************************************/
/*				                   DEFINES	            		        */
/************************************************************************/

#define ONE_SHOT 0
#define PERIODIC 1

#define IDEL     0
#define READY    1
#define WATING   2

/************************************************************************/
/*			                Structures Definitions		                */
/************************************************************************/

typedef struct str_SOSfg_t
{

uint8 counter;
uint8 periodicity;
gptrFun_cbk_t  Sos_AppTask_cbk;
uint8 Task_State;
uint8 Priority ;

}str_SOSfg_t;


/************************************************************************/
/*		         TIMER FUNCTIONS' PROTOTYPES		                    */
/************************************************************************/



/** *****************************************************************
 * Input: NONE
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Initiates the Sos module.
 *
 * *******************************************************************/
ERROR_STATUS Sos_Init(void);


/** ***************************************************************
 * Input: NONE.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Checking on the valid function in certain time and call it .
 *
 *******************************************************************/
ERROR_STATUS Sos_Run(void);

/********************************************************************
 * Input: Pointer to a structure contains the information needed to initialize the timer and start it .
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Initiates the timer and start it and store the new function in the buffer.
 *
 ********************************************************************/
ERROR_STATUS Sos_Creat_Task(str_SOSfg_t * ptrstr_SosFun);

/*********************************************************************
 * Input: Pointer to function.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: remove the function from the  Sos buffer.
 *
 **********************************************************************/
ERROR_STATUS Sos_Delete_Task( gptrFun_cbk_t Fun_Adrress);

/***********************************************************************
 * Input:  NONE.
 * Output:
 * In/Out:
 * Return:  The error status of the function.
 * Description: deInitiates the module.
 *
 ************************************************************************/
ERROR_STATUS Sos_Deinit(void);

#endif /* SERVICE_LAYER_SOS_SOS_H_ */
