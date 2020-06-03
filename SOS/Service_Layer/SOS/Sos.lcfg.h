/******************************************************************************
 *
 * 	Module: 		SOS
 *
 *	File Name: 		Sos_lcfg.h
 *
 * 	Authors: 		Mahmoud Gaber
 *
 *	Created on:		Saturday, 11-JAN-20 10:15:18 PM
 *
 *******************************************************************************/
#ifndef SERVICE_LAYER_SOS_SOS_LCFG_H_
#define SERVICE_LAYER_SOS_SOS_LCFG_H_


/************************************************************************/
/*	                        	 INCLUDES		            	        */
/************************************************************************/
#include "std_types.h"
#include "common_macros.h"


/************************************************************************/
/*				                   DEFINES	            		        */
/************************************************************************/
#define TIMER_0   0
#define TIMER_1   1
#define TIMER_2   2

/************************************************************************/
/*			  Structures Definitions		                            */
/************************************************************************/

typedef struct str_SOSCfg_t
{

 uint8 TIMER_NO ;
 uint8 Timer_Resolution;

}str_SOSCfg_t;

extern str_SOSCfg_t  gstr_Init ;


#endif /* SERVICE_LAYER_SOS_SOS_LCFG_H_ */
