/******************************************************************************
 *
 * 	Module: 		SOS
 *
 *	File Name: 		Sos_lcfg.c
 *
 * 	Authors: 		Mahmoud Gaber
 *
 *	Created on:		Saturday, 11-JAN-20 10:15:18 PM
 *
 *******************************************************************************/

/************************************************************************/
/*	                        	 INCLUDES		            	        */
/************************************************************************/
#include "Sos.lcfg.h"

#include "Timer.h"


/*****Creat  the post build structure that contain the timer channel and the resolution*****/

  str_SOSCfg_t  gstr_Init =
 {
		 TIMER_0,
		 1
 };

