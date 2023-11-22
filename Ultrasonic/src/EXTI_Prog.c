/**
 * @file EXTI_Prog.c
 * @author Abdelrahman Amin
 * 				Amr Khaled
 * 				ElAmir Galal
 * 				Fady Taha
 * 				Mohammed Hassan
 * 				Mohammed Reda
 * @brief
 * @version 0.3
 * @date 2023-11-8
 * @copyright Copyright (c) 2023
 * */
#include "LBIT_MATH.h"
#include "LSTD_types.h"
#include <stdint.h>

#include "EXTI_Interface.h"
#include "EXTI_Private.h"

static void (*EXTI_CallBack[EXTI_NUM])(void);
/**
 * @brief Configures the trigger mode for the specified EXTI line.
 * @param Copy_u8EXTI_Line The EXTI line number to configure.
 * @param Copy_u8TriggeringMode The triggering mode to set (RISING_EDGE_TRIGGERING, FALLING_EDGE_TRIGGERING, ON_CHANGE_TRIGGERING).
 * @return Error state.
 */
u8 EXTI_u8ConfigLine (u8 Copy_u8EXTI_Line , u8 Copy_u8TriggeringMode)
{
  u8 Local_u8ErrorState= E_OK;
  
	if (Copy_u8EXTI_Line > 18) 
	{
		Local_u8ErrorState=E_NOK;
	}
	else{
		switch (Copy_u8TriggeringMode)
		{
			case RISING_EDGE_TRIGGERING :
			  SETBIT(EXTI->RTSR , Copy_u8EXTI_Line);
			  CLRBIT(EXTI->FTSR , Copy_u8EXTI_Line); break;
			case FALLING_EDGE_TRIGGERING:
			  SETBIT(EXTI->FTSR , Copy_u8EXTI_Line);
			  CLRBIT(EXTI->RTSR , Copy_u8EXTI_Line); break;
			case ON_CHANGE_TRIGGERING   :
			  SETBIT(EXTI->RTSR , Copy_u8EXTI_Line);
			  SETBIT(EXTI->FTSR , Copy_u8EXTI_Line); break;
			default                     :
			  Local_u8ErrorState=E_NOK;
		}
    }  
  return Local_u8ErrorState;
}
/**
 * @brief Sets the trigger for the specified EXTI line.
 * @param Copy_EXTI_Num The EXTI line number.
 * @param Copy_TriggerState The trigger state to set (RISING_EDGE_TRIGGERING, FALLING_EDGE_TRIGGERING, ON_CHANGE_TRIGGERING).
 * @return Error state.
 */
u8 EXTI_u8SetTrigger(u8 Copy_EXTI_Num ,u8 Copy_TriggerState ){
	u8 Local_ErrorState= E_OK;
	if(18 <= Copy_EXTI_Num )
	{
		Local_ErrorState= E_NOK;
	}
	else{
		switch(Copy_TriggerState)
			{
				case RISING_EDGE_TRIGGERING: 	EXTI->RTSR |= 1<< Copy_EXTI_Num;
											EXTI->FTSR &= ~(1<<Copy_EXTI_Num); break;
				case FALLING_EDGE_TRIGGERING: 	EXTI->FTSR |= 1<< Copy_EXTI_Num;
											EXTI->RTSR &= ~(1<< Copy_EXTI_Num); break;
				case ON_CHANGE_TRIGGERING: EXTI->RTSR |= 1<< Copy_EXTI_Num;
											EXTI->FTSR |= 1<< Copy_EXTI_Num; 	  break;
				default: Local_ErrorState = E_NOK;
			}
	}

	return Local_ErrorState;
}
/**
 * @brief Clears the pending flag for the specified EXTI line.
 * @param Copy_EXTI_Num The EXTI line number.
 * @return Error state.
 */
u8 EXTI_u8ClearPendingFlag(u8 Copy_EXTI_Num){
	u8 Local_ErrorState= E_OK;
	if(18 <= Copy_EXTI_Num )
	{
		Local_ErrorState=E_NOK;
	}
	else{
		/*Clear Pending Flag*/
		EXTI->PR = 1<<Copy_EXTI_Num; /* 0 doesn't affect */
	}

	return Local_ErrorState;
}

/**
 * @brief Gets the pending flag for the specified EXTI line.
 * @param Copy_EXTI_Num The EXTI line number.
 * @param Copy_PenValue Pointer to store the pending flag value.
 * @return Error state.
 */
u8 EXTI_u8GetPendingFlag(u8 Copy_EXTI_Num , u8 *Copy_PenValue){
	u8 Local_ErrorState= E_OK;
	if(18 <= Copy_EXTI_Num )
	{
		Local_ErrorState=E_NOK;
	}
	else{
		/* Set bit */
		*Copy_PenValue= (EXTI->PR >> Copy_EXTI_Num) & 1 ;
	}

	return Local_ErrorState;
}

/**
 * @brief Enables the interrupt for the specified EXTI line.
 * @param Copy_EXTI_Num The EXTI line number.
 * @return Error state.
 */
u8 EXTI_u8IntEnable(u8 Copy_EXTI_Num){
	u8 Local_ErrorState= E_OK;
	if(18 <= Copy_EXTI_Num )
	{
		Local_ErrorState=E_NOK;
	}
	else{
		/* Set bit enable  */
		EXTI->IMR |= 1<<Copy_EXTI_Num;
	}

	return Local_ErrorState;
}
/**
 * @brief Disables the interrupt for the specified EXTI line.
 * @param Copy_EXTI_Num The EXTI line number.
 * @return Error state.
 */

u8 EXTI_u8IntDisable(u8 Copy_EXTI_Num){
	u8 Local_ErrorState= E_OK;
	if(18 <= Copy_EXTI_Num )
	{
		Local_ErrorState= E_NOK;
	}
	else{
		/* Set bit enable  */
		EXTI->IMR &= ~(1<<Copy_EXTI_Num);
	}
	return Local_ErrorState;
}
/**
 * @brief Sets the callback function for the specified EXTI line.
 * @param Copy_u8EXTI_Line The EXTI line number.
 * @param pvoidfCallback Pointer to the callback function.
 * @return Error state.
 */
u8 EXTI_voidSetCallBackFn (u8 Copy_u8EXTI_Line , void (*pvoidfCallback)(void))
{
	u8 Local_u8ErrorState= E_OK;
	if(NULL_POINTER == pvoidfCallback)
	{
		Local_u8ErrorState=E_NULL_POINTER;
	}
	else
	{
		if (Copy_u8EXTI_Line > 18)
		{
			Local_u8ErrorState=E_NOK;
		}
		else
		{
			EXTI_CallBack[Copy_u8EXTI_Line] = pvoidfCallback;
		}
	}
	return Local_u8ErrorState;
}

/******************************************************************************/
/********************* Handlers definition for EXTI IRQs *********************/
/******************************************************************************/
#ifdef EXTI0_LINE
/**
 * @brief EXTI0 line interrupt handler.
 */
void EXTI0_IRQHandler(void){
	(EXTI_CallBack[EXTI0_LINE])();
//	SETBIT( EXTI->PR , EXTI0_LINE );
}
#endif


#ifdef EXTI9_5_LINES_IRQ
/**
 * @brief EXTI9_5 lines interrupt handler.
 */
void EXTI9_5_IRQHandler(void){
//	(EXTI_CallBack[EXTI9_5_LINES_IRQ])();
	(EXTI_CallBack[EXTI7_LINE])();
	SETBIT( EXTI->PR , EXTI9_5_LINES_IRQ );
}
#endif

