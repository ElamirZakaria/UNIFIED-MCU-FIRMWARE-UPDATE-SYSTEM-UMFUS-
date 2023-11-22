/**
 * @file MTIM1.c
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
 *
 */
#include <LSTD_types.h>
#include <LBIT_math.h>

#include <MTIM1_interface.h>
#include <MTIM1_private.h>

/**
 * @brief Initializes Timer 1 for PWM generation.
 * 
 * @param Copy_u16ARRValue    Auto-reload value for Timer 1.
 * @param Copy_u16Prescalar   Prescaler value for Timer 1.
 * @return STD_error_t        Error state: E_OK if successful, E_NOK if an error occurred.
 */
STD_error_t MTIM1_stderrInit(u16 Copy_u16ARRValue,u16 Copy_u16Prescalar)
{
	STD_error_t L_stderrState=E_OK;
	if((Copy_u16ARRValue < 65536) && (Copy_u16Prescalar < 65536))
	{
	// Configure Timer 1 for PWM generation
	TIM1_CR1 = 0; // Clear control register 1
	TIM1_CR1 = 0; // Clear control register 2
	TIM1_CCMR1 |= (6 << 4) | (1 << 3); // PWM mode
	TIM1_CCER |= (1 << 0); // Enable capture/compare channel 1 output
	TIM1_BDTR |= (1 << 15);
	TIM1_PSC = Copy_u16Prescalar; // prescaler Value
	TIM1_ARR = Copy_u16ARRValue; // Auto-reload value
	TIM1_CCR1 = 999; // Initial duty cycle value
	}
	else
	{
		L_stderrState=E_NOK;
	}
	return L_stderrState;
}

/**
 * @brief Enables Timer 1.
 */
void 	MTIM1_voidEnable(void)
{
	// Enable Timer 1 Enable
	TIM1_CR1 |= (1 << 0);
}
/**
 * @brief Disables Timer 1.
 */
void 	MTIM1_voidDisable(void)
{
	// Disable Timer 1 Enable
	CLEAR_BIT(TIM1_CR1,0);
}
/**
 * @brief Sets the duty cycle for Timer 1 PWM output.
 * 
 * @param Copy_u16DCValue     Duty cycle value to be set for Timer 1.
 * @return STD_error_t        Error state: E_OK if successful, E_NOK if an error occurred.
 */
STD_error_t	MTIM1_stderrSetDutyCycle(u16 Copy_u16DCValue)
{
	STD_error_t L_stderrState=E_OK;
	if(Copy_u16DCValue<65536)
	{
	TIM1_CCR1 = Copy_u16DCValue;
	}
	else
	{
		//Return Error
		L_stderrState=E_NOK;
	}
	return L_stderrState;
}
