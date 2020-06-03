/******************************************************************************
 *
 * 	Module: 		SOS
 *
 *	File Name: 		Sos.c
 *
 * 	Authors: 		Mahmoud Gaber
 *
 *	Created on:		Saturday, 11-JAN-20 10:15:18 PM
 *
 *******************************************************************************/

/************************************************************************/
/*	                        	 INCLUDES		            	        */
/************************************************************************/

#include "Sos_cfg.h"
#include "Sos.lcfg.h"
#include "Timer.h"
#include "Sleep_mode.h"
#include "Sos.h"

/************************************************************************/
/*				                   DEFINES	            		        */
/************************************************************************/
#define SOS_INITALIZED       (1)
#define SOS_NOT_INITALIZED   (0)

#define STARTED              (1)
#define NOT_STARTED          (0)

#define ONE                  (1)
#define ZERO                 (0)

#define TIMER_COUNTS_NEEDED  (250)




/**array of structure [Sos Buffer]**/
static   str_SOSfg_t gastr_SosBuffer[MAX_NUM_TASKS];


/************************************************************************/
/*		                     GLOBAL VARIABLES	                        */
/************************************************************************/

static   uint8  gu8_Sos_State   = SOS_NOT_INITALIZED;
volatile static uint8 gu8_BufferTop = ZERO;
static   uint8  gu8_timer_state = NOT_STARTED;
static   uint8  gu8_Ovf_Tflag        = LOW;
/** *******************************************************************
 * Input: NONE
 * Output:
 * In/Out:
 * Return: NONE.
 * Description: call back function set the flag to high  and reload the timer  .
 *
 ********************************************************************/
void Timer_ISR(void)
{
	gu8_Ovf_Tflag =HIGH;
	Timer_Set(gstr_Init.TIMER_NO,TIMER_COUNTS_NEEDED);
}

/*********************************************************************
 * Input: NONE.
 * Output:
 * In/Out:
 * Return: NONE.
 * Description: initialize all global objects.
 *
 * *******************************************************************/
void Clear_objects(void)
{
	uint8 u8_index =ZERO;
	/****initialize the elements of structure *********/
	for(u8_index = ZERO ; u8_index < MAX_NUM_TASKS ; u8_index++)
	{
		gastr_SosBuffer[u8_index].Task_State = IDEL;
		gastr_SosBuffer[u8_index].Sos_AppTask_cbk = NULL;
		gastr_SosBuffer[u8_index].counter = ZERO;
		gastr_SosBuffer[u8_index].periodicity = ZERO;
		gastr_SosBuffer[u8_index].Priority = ZERO;



	}
	gu8_Sos_State   =SOS_NOT_INITALIZED;
	gu8_BufferTop =ZERO;
	gu8_timer_state = NOT_STARTED;
	gu8_Ovf_Tflag=LOW;
}

/*********************************************************************
 * Input: NONE
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Initiates the Sos module.
 *
 ********************************************************************/
ERROR_STATUS Sos_Init(void)
{
	/***initialize all global objects***/
	Clear_objects();

	/*******error state*********/
	uint8 u8_error_val = SOS_ERROR_BASE + SOS_SUCSSECE;

	/****check if the SOS module is initialized or not****/
	if(gu8_Sos_State != SOS_INITALIZED )
	{
		/****initiate the timer channel****/
		Timer_cfg_s TIMER_Init_st;
		TIMER_Init_st.Timer_CH_NO = gstr_Init.TIMER_NO;
		TIMER_Init_st.Timer_Mode= TIMER_MODE;
		TIMER_Init_st.Timer_Polling_Or_Interrupt = TIMER_INTERRUPT_MODE;
		TIMER_Init_st.Timer_Cbk_ptr =Timer_ISR;

		/****switch on the timer channels****/
		switch(gstr_Init.TIMER_NO)
		{
		case TIMER_0:
			/******choose the prescaler*****/
			TIMER_Init_st.Timer_Prescaler=TIMER_PRESCALER_64;
			break;
		case TIMER_1:
			TIMER_Init_st.Timer_Prescaler=TIMER_PRESCALER_256;
			break;
		case TIMER_2:
			TIMER_Init_st.Timer_Prescaler=TIMER_PRESCALER_64;
			break;
		default:
			u8_error_val = SOS_ERROR_BASE + SOS_UNVALID_INPUT;

		}

		Timer_Init(&TIMER_Init_st);


		/*******error state*********/
		gu8_Sos_State = SOS_INITALIZED;
	}
	else
	{
		/*******error state*********/
		u8_error_val = SOS_ERROR_BASE + SOS_ALREADYINITALIZED;;
	}
	/*******return error state*********/
	return u8_error_val;
}

/*********************************************************************
 * Input: NONE.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Checking on the valid function in certain time and call it .
 *
 * *******************************************************************/

ERROR_STATUS Sos_Run(void)
{
	/*******error state*********/
	uint8 u8_error_val = SOS_ERROR_BASE + SOS_SUCSSECE;

	/****local variable used to loop on the buffer*******/
	volatile  uint8 u8_count=ZERO;
	volatile  uint8 u8_count2=ZERO;
	str_SOSfg_t u8_temp;

	/****sort the tasks according to the highest piriority***/
	for(u8_count =ZERO; u8_count < gu8_BufferTop ;u8_count++)
	{
		for(u8_count2 =u8_count+1; u8_count2 < gu8_BufferTop ;u8_count2++)
		{
			if(gastr_SosBuffer[u8_count].Priority  < gastr_SosBuffer[u8_count2].Priority)
			{
				u8_temp = gastr_SosBuffer[u8_count];
				gastr_SosBuffer[u8_count]  = gastr_SosBuffer[u8_count2];
				gastr_SosBuffer[u8_count2] = u8_temp;

			}
		}

	}
	/***run all tasks once for the first time***/
	for(u8_count =ZERO; u8_count < gu8_BufferTop ;u8_count++)
	{
		gastr_SosBuffer[u8_count].Sos_AppTask_cbk();
		gastr_SosBuffer[u8_count].Task_State = WATING;


	}

	if(gu8_timer_state != STARTED)
	{
		Timer_Start(gstr_Init.TIMER_NO,TIMER_COUNTS_NEEDED);
		gu8_timer_state = STARTED;
	}
	else
	{

	}
	if(gu8_Sos_State == SOS_INITALIZED)
	{
		while(1)
		{

			/*****check if the ISR fire or not ******/
			if(gu8_Ovf_Tflag == HIGH)
			{

				/********loop on the buffer to check if the functions ready to serve or not *******/
				for(u8_count =ZERO; u8_count < gu8_BufferTop ;u8_count++)
				{
					/****increasing the counter till it reach to the required time for this function to be serve when it reach*****/
					/*****every function have its counter to calculate its delay******/
					gastr_SosBuffer[u8_count].counter++;

					if(gastr_SosBuffer[u8_count].counter == gastr_SosBuffer[u8_count].periodicity )
					{

						/***make the state of the task READY***/
						gastr_SosBuffer[u8_count].Task_State = READY;

						if(gastr_SosBuffer[u8_count].Task_State == READY)
						{
							gastr_SosBuffer[u8_count].Sos_AppTask_cbk();
							gastr_SosBuffer[u8_count].Task_State = WATING;
						}
						else
						{

						}

						/****put zero on the counter for the next time ****/
						gastr_SosBuffer[u8_count].counter = ZERO;
					}
				}
				/****make the flag low to make sure that it will go thiow this function in the next interrupt firing *****/
				gu8_Ovf_Tflag = LOW;
			}


			/****put CPU on sleep mode****/
			Sleep_Cpu();

		}
	}
	else
	{
		u8_error_val = SOS_ERROR_BASE + SOS_NOTINITALIZED ;

		/*****return the error state****/


	}
	return u8_error_val;
}


/*********************************************************************
 * Input: Pointer to a structure contains the information needed to initialize the timer and start it .
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Initiates the timer and start it and store the new function in the buffer.
 *
 * *******************************************************************/
ERROR_STATUS Sos_Creat_Task(str_SOSfg_t * ptrstr_SosFun)
{
	/*****error state****/
	uint8 u8_error_val = SOS_ERROR_BASE + SOS_SUCSSECE;

	if(ptrstr_SosFun->Sos_AppTask_cbk != NULL)
	{
		if(gu8_Sos_State == SOS_INITALIZED)
		{
			gastr_SosBuffer[gu8_BufferTop] .Task_State       = ptrstr_SosFun->Task_State;
			gastr_SosBuffer[gu8_BufferTop] .Sos_AppTask_cbk = ptrstr_SosFun->Sos_AppTask_cbk;
			gastr_SosBuffer[gu8_BufferTop] .counter         = ptrstr_SosFun->counter;
			gastr_SosBuffer[gu8_BufferTop] .periodicity           = ptrstr_SosFun->periodicity;
			gastr_SosBuffer[gu8_BufferTop] .Priority           = ptrstr_SosFun->Priority;

			gu8_BufferTop++;

			/***check if the timer started or not and if not then start it *****/

		}
		else
		{
			/*****error state****/
			u8_error_val = SOS_ERROR_BASE + SOS_NOTINITALIZED;
		}
	}

	else
	{
		/*****error state****/
		u8_error_val = SOS_ERROR_BASE + SOS_NULL_POINTER;

	}
	/*****return the error state****/
	return u8_error_val;
}

/*********************************************************************
 * Input: Pointer to function.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: remove the function from the  SOS buffer.
 *
 * *******************************************************************/
ERROR_STATUS Sos_Delete_Task(gptrFun_cbk_t Task_Adrress)
{
	volatile uint8 u8_count=ZERO;

	/*****error state****/
	uint8 u8_error_val = SOS_ERROR_BASE + SOS_SUCSSECE;

	if(Task_Adrress != NULL)
	{
		if(gu8_BufferTop != ZERO)
		{
			/*****try to find the index of the required function  in the buffer***/
			while(Task_Adrress!= gastr_SosBuffer[u8_count].Sos_AppTask_cbk  &&  u8_count < gu8_BufferTop)
			{
				u8_count++;
			}

			/****in case function has been found  and its index equal to the top ****/
			if(u8_count == gu8_BufferTop - ONE)
			{
				/****clear all data and decrease the top by one***/
				gastr_SosBuffer[u8_count].Sos_AppTask_cbk =NULL;
				gastr_SosBuffer[u8_count].Task_State = FALSE;
				gastr_SosBuffer[u8_count].counter =ZERO;
				gastr_SosBuffer[u8_count].periodicity =ZERO;

				/***decrease the buffer top by 1***/
				gu8_BufferTop--;

			}
			/****in case function has been found  and its index less than the top ****/

			else if(u8_count < gu8_BufferTop - ONE)
			{
				/***Equal the this  element by the last one****/
				for(;u8_count < gu8_BufferTop - ONE;u8_count++)
				{
					gastr_SosBuffer[u8_count]=gastr_SosBuffer[u8_count + ONE];

				}
				/***decrease the buffer by 1***/
				gu8_BufferTop--;

			}
			else
			{		/*****error state****/
				u8_error_val = SOS_ERROR_BASE + SOS_UNVALID_INPUT;
				/*the count bigger than index*/
			}
		}
		else
		{
			/*****error state****/
			u8_error_val = SOS_ERROR_BASE + SOS_EMPTY_BUFFER;
		}
	}

	else
	{		/*****error state****/
		u8_error_val = SOS_ERROR_BASE + SOS_NULL_POINTER;

	}
	/*****return the error state****/
	return u8_error_val;
}



/*********************************************************************
 * Input: NONE.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: deInitiates the module.
 *
 * *******************************************************************/
ERROR_STATUS Sos_Deinit(void)
{
	/*****error state****/
	uint8 u8_error_val = SOS_ERROR_BASE + SOS_SUCSSECE;

	/***check if the SOS initiated or not *****/
	if(gu8_Sos_State  != SOS_NOT_INITALIZED )
	{
		/****deinitite the timer****/
		Timer_deinit(gstr_Init.TIMER_NO);

		/***initialize all global objects ***/
		Clear_objects();
		/***make the state of the SOS not initialized****/
		gu8_Sos_State = SOS_NOT_INITALIZED;

	}
	else
	{
		/*****error state****/
		u8_error_val = SOS_ERROR_BASE + SOS_NOTINITALIZED;

	}
	/*****return the error state****/
	return u8_error_val;
}
