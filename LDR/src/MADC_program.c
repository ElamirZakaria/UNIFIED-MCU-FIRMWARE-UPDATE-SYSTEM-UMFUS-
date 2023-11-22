/**
 * @file MADC.c
 * @author Abdelrahman Amin
 * 				Amr Khaled
 * 				ElAmir Galal
 * 				Fady Taha
 * 				Mohammed Hassan
 * 				Mohammed Reda
 * @brief  ADC Software Driver.
 *        This driver provides functions to initialize ADC, enable/disable ADC, start ADC conversion, 
 *        read ADC values, and poll the end of conversion (EOC) flag.
 * @version 0.3
 * @date 2023-11-8
 * @copyright Copyright (c) 2023
 *
 */
/********* Libraries Includes *********/
#include <LSTD_types.h>
#include <LBIT_math.h>

/********* ADC Header Files Includes *********/
#include <MADC_interface.h>
#include <MADC_private.h>
#include <MADC_config.h>
/**
 * @brief Initializes the ADC module.
 *        Configures conversion trigger to an external event and channel selection.
 */
void	MADC_voidInit(void)
{

	/*Set Conversion Trigger to External Event*/
	MADC->CR2 |= (1<<20);

	/*External Event selection pointed to SWSTART bit*/
	MADC->CR2 |= (7<<17);

	/*Select Channel*/
	MADC->SQR3 |= (0<<0);  // SEQ1 for Channel 1


}
/**
 * @brief Enables the ADC module.
 */
void		MADC_voidEnable(void)
{
	SET_BIT(MADC->CR2, 0);
}
/**
 * @brief Disables the ADC module.
 */
void		MADC_voidDisable(void)
{

	CLEAR_BIT(MADC->CR2, 0);
}	
/**
 * @brief Starts ADC conversion.
 *        Clears the status register and sets the start conversion bit.
 */
void		MADC_voidStartConversion(void)
{
	MADC->SR = 0;
	SET_BIT(MADC->CR2, 22);
}
/**
 * @brief Reads the ADC value from the data register.
 * 
 * @param ARG_pu16ADCRead   Pointer to store the ADC read value.
 * @return STD_error_t      Error state: E_OK if successful, E_NULL_POINTER if the pointer is NULL.
 */
STD_error_t	MADC_stderrReadAdc(u16 *ARG_pu16ADCRead)
{
	STD_error_t L_stderrState=E_OK;
	if(ARG_pu16ADCRead != NULL_POINTER)
	{
		*ARG_pu16ADCRead = MADC->DR;
	}
	else
	{
		L_stderrState=E_NULL_POINTER;
	}
	return L_stderrState;
}
/**
 * @brief Polls the end of conversion (EOC) flag.
 *        Waits until the EOC flag is set.
 */
void 		MADC_voidEOCFlagPolling(void)
{
	while(!GET_BIT(MADC->SR, 1));
}
