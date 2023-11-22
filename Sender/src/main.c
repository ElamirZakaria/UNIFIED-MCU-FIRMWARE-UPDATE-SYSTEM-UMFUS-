
#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "RCC_interface.h"
#include "MGPIO_interface.h"
#include "CAN_interface.h"
#include "STK_interface.h"
#include "MFPEC_interface.h"
#include "HEX.h"

uint8 gDATA[50]; /* buffer to receive*/
/* will be base address from flash lets say 0x0800A000 */
uint8 *g_pu8BaseAddress=(uint8*)0x0800A000;

void CAN_TX(uint8* ARG_pu8BaseAddress, uint8 ARG_u8DLCValue,uint16 ARG_u16ID )
{
	CAN_TxHeaderTypeDef tx_header;

	tx_header.StdId = ARG_u16ID;
	tx_header.IDE = CAN_ID_STD;
	tx_header.RTR = CAN_RTR_DATA;
	/* assign the DLC value */
	tx_header.DLC = ARG_u8DLCValue;
	tx_header.TransmitGlobalTime=DISABLE;
	CAN_voidAddTxMsg(&tx_header, ARG_pu8BaseAddress);
}
void CAN_RX(void)
{
	CAN_RxHeaderTypeDef Rx_header;
	for(u8 i=0 ;i == 0 ;  ){
		while(CAN_voidRXPending(CAN_RX_FIFO0) == 0);
		i=CAN_voidGetRxMsg(CAN_RX_FIFO0,&Rx_header,gDATA);
	}
}

void CAN_TXRecord(uint8* ARG_pu8BaseAddress,uint8 ARG_u8ECU)
{
	 uint8 i=0 , counter=0  ;
	 uint16  ID=0;
	/* this func will transmit 8 byte from record in frame if the terminator found then send the length */
	/* just count the number of DLC in last frame */
	/* note the terminator will be  '\0' */
	 switch (ARG_u8ECU){
	 	case 1 :
	 	ID = 0x75D;
	 	break;
	 	case 2 :
	 	ID = 0x65D;
	 	break;
	 	default:break;
	 	}
	while(1)
	{
		for( i= 0; ARG_pu8BaseAddress[i] != '\0'; i++)
		{
			counter ++;
			if( counter == 8 )
			{
				STK_voidDelay_ms(1);
				/* transmit data and counter=0 */
				CAN_TX( &ARG_pu8BaseAddress[i - 7 ] , counter ,ID);
				counter=0 ;
			}
		}
		/* to send the last part of record */
		if(counter != 0 )
		{
			/* we will call func CAN func TX it will transmit the frame */
			CAN_TX( &ARG_pu8BaseAddress[ i - counter ] , counter, ID );
		}
		else
		{
			/* do nothing */
		}
		CAN_RX();

		if (strcmp(gDATA, "done") == 0)
		{
			break;
		}
		counter=0;
		ARG_pu8BaseAddress += 48;
	}
}

uint16 Std_id_High(uint16 local_u16Std_Id)
{
	return (local_u16Std_Id << 5);
}
void CAN_FilterConfig(void)
{
	CAN_FilterTypeDef CAN_FilterInit;
	CAN_FilterInit.FilterActivation = ENABLE;
	CAN_FilterInit.FilterBank = 0;
	CAN_FilterInit.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	CAN_FilterInit.FilterIdHigh = Std_id_High(0x55D);
	CAN_FilterInit.FilterIdLow = 0x0000;
	CAN_FilterInit.FilterMaskIdHigh = 0x0000;
	CAN_FilterInit.FilterMaskIdLow = 0x0000;
	CAN_FilterInit.FilterMode = CAN_FILTERMODE_IDMASK;
	CAN_FilterInit.FilterScale = CAN_FILTERSCALE_32BIT;
	CAN_voidConfigFilter(&CAN_FilterInit);
}

void CAN_Interrupt(void)
{
//	CAN_voidInitInterput(CAN_TX_IRQn);
//	CAN_voidInitInterput(CAN_RX0_IRQn);
//	CAN_voidInitInterput(CAN_RX1_IRQn);
//	CAN_voidInitInterput(CAN_SCE_IRQn);

	CAN_voidEnableInterrupt(CAN_IER_TX_MAILBOX_EMPTY | CAN_IER_RX_FIFO0_MSG_PENDING | CAN_IER_ERROR | CAN_IER_LAST_ERROR_CODE | CAN_IER_BUSOFF);
}



void CAN_RxFifo0MsgPendingCallback()
{
	CAN_RX();
	// GPIO_voidSetPinValue(GPIOA, PIN1, HIGH);
	//NVIC_voidSetPendingFlag(CAN_RX0_IRQn);
}


int main()
{
	/* Rcc init PLL using HSE/2 HCLK = 72M CAN 36M*/
	RCC_voidInitSysClock();

	/* GPIO init */
    RCC_voidEnableClock(RCC_APB2, APB2_GPIOA_EN);
    RCC_voidEnableClock(RCC_APB2, APB2_GPIOC_EN);
    RCC_voidEnableClock(RCC_AHB, 4); /* FPEC*/
	MGPIO_stderrSetPinMode( MGPIO_PIN_A7, MGPIO_MODE_OUTPUT_PUSHPULL_2MHZ);
	MGPIO_stderrSetPinMode( MGPIO_PIN_A5, MGPIO_MODE_OUTPUT_PUSHPULL_2MHZ);
	MGPIO_stderrSetPinMode( MGPIO_PIN_C13, MGPIO_MODE_OUTPUT_PUSHPULL_2MHZ);
	/*use FPEC to write on Flash memory */
	for(u8 L_flashIterator=40;L_flashIterator<56;L_flashIterator++)
	{
		MFPEC_voidFlashPageErase(L_flashIterator);
	}
	for(u16 L_iterator=0;L_iterator<332;L_iterator++)
	{
		MFPEC_voidFlashWrite((uint32)g_pu8BaseAddress,(uint16*)&hexData[L_iterator][0],48);
		g_pu8BaseAddress+=48 ;
	}
	g_pu8BaseAddress=(uint8*)0x0800A000; /* return base address to read code */
	/* CAN init */
	RCC_voidEnableClock(RCC_APB1, APB1_CAN_EN);
	CAN_voidInit();
	/* CAN Filter set */
	CAN_FilterConfig();
	CAN_Interrupt();
	/* CAN Start */
	CAN_voidStart();
	MGPIO_stderrSetPinState(MGPIO_PIN_C13, MGPIO_HIGH);
	while(1)
	{
		MGPIO_stderrSetPinState(MGPIO_PIN_A7 ,MGPIO_LOW);
		MGPIO_stderrSetPinState(MGPIO_PIN_A5 ,MGPIO_HIGH); /* send an interrupt for slave to start to recieve */
		CAN_RX(); /* waiting for slave to send OK */
		CAN_TXRecord((uint8*)g_pu8BaseAddress,2);
		MGPIO_stderrSetPinState(MGPIO_PIN_C13, MGPIO_LOW);
		STK_voidDelay_ms(99000);
		MGPIO_stderrSetPinState(MGPIO_PIN_C13, MGPIO_HIGH);
		MGPIO_stderrSetPinState(MGPIO_PIN_A5 ,MGPIO_LOW);
		MGPIO_stderrSetPinState(MGPIO_PIN_A7 ,MGPIO_HIGH);
		CAN_RX(); /* waiting for slave to send OK */
		CAN_TXRecord((uint8*)g_pu8BaseAddress,1);
	}
	return 0;
}
