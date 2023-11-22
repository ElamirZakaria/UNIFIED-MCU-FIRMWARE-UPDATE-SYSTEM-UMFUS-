//#include "Stm32F103xx.h"
/**
 * @file NVIC.c
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
#include "LBIT_MATH.h"
#include "LSTD_types.h"

#include <stdint.h>

#include "NVIC_Interface.h"
#include "NVIC_Private.h"

/**
 * @file NVIC_Interface.h
 * @brief STM32 NVIC (Nested Vectored Interrupt Controller) interface functions.
 *        These functions enable/disable interrupts, set pending flags, and manage interrupt priorities.
 */
 /**
 * @brief Enables a specific interrupt in the NVIC.
 * 
 * @param IRQ_u8Num The interrupt number to be enabled.
 * @return uint8_t Error state: E_OK if successful, E_NOK if an error occurred.
 */
uint8_t NVIC_u8EnableINT(uint8_t IRQ_u8Num)
{
	uint8_t Local_u8ErrorState=E_OK;
	if(IRQ_u8Num <= OTG_FS_IRQ)
	{
		uint8_t IRQ_u8RegNum = IRQ_u8Num / IRQ_REGSIZE;
		uint8_t IRQ_u8BitNum = (IRQ_u8Num % IRQ_REGSIZE);
		NVIC->ISER[IRQ_u8RegNum] = IRQ_SETBIT<<IRQ_u8BitNum; /* set bit */
	}
	else
	{
		Local_u8ErrorState=E_NOK;
	}
	return Local_u8ErrorState;
}

/**
 * @brief Disables a specific interrupt in the NVIC.
 * 
 * @param IRQ_u8Num The interrupt number to be disabled.
 * @return uint8_t Error state: E_OK if successful, E_NOK if an error occurred.
 */
uint8_t NVIC_u8DisableINT(uint8_t IRQ_u8Num)
{
	uint8_t Local_u8ErrorState=E_OK;
	if(IRQ_u8Num <= OTG_FS_IRQ)
	{
		uint8_t IRQ_u8RegNum = IRQ_u8Num / IRQ_REGSIZE;
		uint8_t IRQ_u8BitNum = (IRQ_u8Num % IRQ_REGSIZE);
		NVIC->ICER[IRQ_u8RegNum] = IRQ_SETBIT<<IRQ_u8BitNum; /* set bit */
	}
	else
	{
		Local_u8ErrorState=E_NOK;
	}
	return Local_u8ErrorState;
}

/**
 * @brief Sets a pending flag for a specific interrupt in the NVIC.
 * 
 * @param IRQ_u8Num The interrupt number to set the pending flag for.
 * @return uint8_t Error state: E_OK if successful, E_NOK if an error occurred.
 */
uint8_t NVIC_u8SetPendingFlag(uint8_t IRQ_u8Num)
{
	uint8_t Local_u8ErrorState=E_OK;
	if(IRQ_u8Num <= OTG_FS_IRQ)
	{
		uint8_t IRQ_u8RegNum = IRQ_u8Num / IRQ_REGSIZE;
		uint8_t IRQ_u8BitNum = (IRQ_u8Num % IRQ_REGSIZE);
		NVIC->ISPR[IRQ_u8RegNum] = IRQ_SETBIT<<IRQ_u8BitNum; /* set bit */
	}
	else
	{
		Local_u8ErrorState=E_NOK;
	}
	return Local_u8ErrorState;	
}
/**
 * @brief Clears a pending flag for a specific interrupt in the NVIC.
 * 
 * @param IRQ_u8Num The interrupt number to clear the pending flag for.
 * @return uint8_t Error state: E_OK if successful, E_NOK if an error occurred.
 */
uint8_t NVIC_u8ClearPendingFlag(uint8_t IRQ_u8Num)
{
	uint8_t Local_u8ErrorState=E_OK;
	if(IRQ_u8Num <= OTG_FS_IRQ)
	{
		uint8_t IRQ_u8RegNum = IRQ_u8Num / IRQ_REGSIZE;
		uint8_t IRQ_u8BitNum = (IRQ_u8Num % IRQ_REGSIZE);
		NVIC->ICPR[IRQ_u8RegNum] = IRQ_SETBIT<<IRQ_u8BitNum; /* set bit */
	}
	else
	{
		Local_u8ErrorState=E_NOK;
	}
	return Local_u8ErrorState;		
}

/**
 * @brief Gets the active flag status for a specific interrupt in the NVIC.
 * 
 * @param IRQ_u8Num       The interrupt number to get the active flag status for.
 * @param ActiveFlag_u8val Pointer to store the active flag status.
 * @return uint8_t Error state: E_OK if successful, E_NOK if an error occurred, E_NULL_POINTER for null pointer.
 */
uint8_t NVIC_u8GetActiveFlag(uint8_t IRQ_u8Num , uint8_t * ActiveFlag_u8val)
{
	uint8_t Local_u8ErrorState=E_OK;
	/* check null pointer */
	if (ActiveFlag_u8val != NULL_POINTER)
	{
		if(IRQ_u8Num <= OTG_FS_IRQ)
		{
			uint8_t IRQ_u8RegNum = IRQ_u8Num / IRQ_REGSIZE;
			uint8_t IRQ_u8BitNum = (IRQ_u8Num % IRQ_REGSIZE);
			*ActiveFlag_u8val= (NVIC->IABR[IRQ_u8RegNum] >> IRQ_u8BitNum) & IRQ_BITMASK; /* Read bit*/
		}
		else
		{
			Local_u8ErrorState=E_NOK;
		}
		
	}
	else Local_u8ErrorState = E_NULL_POINTER;
	
	return Local_u8ErrorState;
		
}

//uint8_t SCB_u8SetPriorityGrouping(uint8_t copy_u8priority_group){
//	uint8_t Local_u8ErrorState=E_OK;
//	if (copy_u8priority_group >= BitsGroup4_Sub0 && copy_u8priority_group <= BitsGroup0_Sub4) {
//		uint32_t Local_u32AIRCRkey = AIRCR_VECTKEY << VECTKEY_SHIFTBITS; // VECTKEY value shifted to the correct position
//		uint32_t Local_u32AIRCRvalue = ( SCB->AIRCR & ~(AIRCR_MASK) ) | (copy_u8priority_group << PRIGROUP_SHIFTBITS); // Clear bits 8 to 10 and set to 3//( (AIRCR_VECTKEY << VECTKEY_SHIFTBITS) | (copy_u8priority_group << PRIGROUP_SHIFTBITS) );
//		SCB->AIRCR =  Local_u32AIRCRvalue | Local_u32AIRCRkey;
//	}
//	else Local_u8ErrorState= E_NOK; // Return an error code if the value is invalid
//
//	return Local_u8ErrorState;
//}

/**
 * @brief Sets the priority for a specific IRQ number in the NVIC.
 * 
 * @param IRQ_u8Num     The interrupt number to set the priority for.
 * @param IRQ_u8priority The priority to be set.
 * @return uint8_t Error state: E_OK if successful, E_NOK if an error occurred.
 */
uint8_t NVIC_u8SetIRQPriority(uint8_t IRQ_u8Num , uint8_t IRQ_u8priority){
	uint8_t Local_u8ErrorState=E_OK;
	if(IRQ_u8Num <= OTG_FS_IRQ && IRQ_u8priority <= IRQ_Priority15)
	{
		NVIC->IPR[IRQ_u8Num] = IRQ_u8priority<<IRQ_SHIFTBITS ;
	}
	else
	{
		Local_u8ErrorState=E_NOK;
	}
	return Local_u8ErrorState;
}
