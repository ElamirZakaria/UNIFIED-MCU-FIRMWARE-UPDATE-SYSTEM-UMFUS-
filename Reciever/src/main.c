
#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "CAN_interface.h"
#include "NVIC_interface.h"
#include "STK_interface.h"
#include "MFPEC_interface.h"



void Parser_voidParseRecord(u8* Copy_u8BufData);
uint8 gEndDATA[6]={"done*"};
typedef void (*Function_t)(void);
Function_t addr_to_call = 0;

void Jump_APP(void)
{
#define SCB_VTOR   *((volatile u32*)0xE000ED08)

	SCB_VTOR = 0x0800A000;  /*new address for flash code App code */

	addr_to_call = *(Function_t*)(0x0800A004);
	addr_to_call();

}


uint8 gDATA[50]; /*buffer to receive Data */
uint8 gTXDATA[4]={"OK*"};
/* will be base address from flash lets say 0x08004000 */
uint8 g_pu8BaseAddress;


void CAN_TX(uint8* ARG_pu8BaseAddress, uint8 ARG_u8DLCValue )
{

	CAN_TxHeaderTypeDef tx_header;
	tx_header.StdId = 0x55D;
	tx_header.IDE = CAN_ID_STD;
	tx_header.RTR = CAN_RTR_DATA;
	/* assign the DLC value */
	tx_header.DLC = ARG_u8DLCValue;
	tx_header.TransmitGlobalTime=DISABLE;

	CAN_voidAddTxMsg(&tx_header, ARG_pu8BaseAddress);
}

void CAN_TXRecord(uint8* ARG_pu8BaseAddress)
{
	uint8 i , counter=0 ;
	/* this func will transmit 8 byte from record in frame if the terminator found then send the length */
	/* just count the number of DLC in last frame */
	/* note the terminator will be  '\0'  */
	for( i= 0; ARG_pu8BaseAddress[i] != '\0' ; i++)
	{
		counter ++;

		if( counter == 8 )
		{
			/* transmit data and counter=0 */
			CAN_TX( &ARG_pu8BaseAddress[i - 7 ] , counter );
			counter=0 ;
		}
	}
	/* to send the last */
	if(counter != 0 )
	{
		/* we will call func CAN func TX it will transmit the frame */
		CAN_TX( &ARG_pu8BaseAddress[ i - counter ] , counter );
	}
	else
	{
		/* do nothing */
	}
}




u8 CAN_RX(void)
{
	CAN_RxHeaderTypeDef Rx_header;
	u8 i;
	for(i=0 ;i == 0 ;  ){
		while(CAN_voidRXPending(CAN_RX_FIFO0) == 0);
		i=CAN_voidGetRxMsg(CAN_RX_FIFO0,&Rx_header,gDATA);
	}
	return i ; /* to indicate the end of code will check this value in main before parse code if(== 2  ) { jjump to APP by call Func}*/

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
	CAN_FilterInit.FilterIdHigh = Std_id_High(0x65D);
	CAN_FilterInit.FilterIdLow = 0x0000;
	CAN_FilterInit.FilterMaskIdHigh = 0x0000;
	CAN_FilterInit.FilterMaskIdLow = 0x0000;
	CAN_FilterInit.FilterMode = CAN_FILTERMODE_IDMASK;
	CAN_FilterInit.FilterScale = CAN_FILTERSCALE_32BIT;
	CAN_voidConfigFilter(&CAN_FilterInit);
}

u8 calculate_checksum(u8* hex_record)
	{
    u16 sum = 0;
    u16 length = strlen(hex_record)-2;
    u8 checksum=((AsciToHex(hex_record[length])<<4) | AsciToHex(hex_record[length+1]));
    for (u8 i = 1; i < length; i += 2) {
        if (i + 1 < length) {
            sum += ((AsciToHex(hex_record[i])<<4) | AsciToHex(hex_record[i+1])) ;
        }
    }
	   sum = (u8)(0x100 - (sum & 0xFF)) ;
	   return(sum==checksum)?1:0;
}

int main()
{
	/* Rcc init */
	RCC_voidInitSysClock(); /*default is 8M */
	RCC_voidEnableClock(RCC_AHB, 4); /*Enable  FPEC*/
	/* GPIO init */
    RCC_voidEnableClock(RCC_APB2, APB2_GPIOA_EN);
    RCC_voidEnableClock(RCC_APB2, APB2_GPIOC_EN);
    GPIO_voidSetPinDirection(GPIOA, PIN7, INPUT_PULL_UP_DOWN);
    GPIO_voidSetPinPull (GPIOA, PIN7, PULL_DOWN);
	GPIO_voidSetPinDirection(GPIOC, PIN13, OUTPUT_SPEED_2MHZ_PP);
	MFPEC_voidEraseAppArea();
	/* CAN init */
	RCC_voidEnableClock(RCC_APB1, APB1_CAN_EN);
	CAN_voidInit();
	/* CAN Filter set */
	CAN_FilterConfig();
	/* CAN Start */
	CAN_voidStart();
    volatile u8  L_u8Terminator = 0;
    while(GPIO_u8GetPinValue(GPIOA, PIN7)==0);
    CAN_TX(gTXDATA,3);
	while(1)
	{
		/* polling for new reord*/
		L_u8Terminator=CAN_RX();
		 if(L_u8Terminator==2 )
		{
			CAN_TX(gEndDATA,6);   /* send done before jump to app */
			STK_voidDelay_ms(1);
			Jump_APP();
		}
		  if(calculate_checksum(gDATA))  /* check if the checksum is correct to check for can bus integrity */
		  {
				Parser_voidParseRecord(gDATA); /* parse code after receive it */
				CAN_TX(gTXDATA,3); /* send OK to receive the next record */
		  }
		  else
		  {
              /* do nothing */
		  }
		}
	return 0;
}
