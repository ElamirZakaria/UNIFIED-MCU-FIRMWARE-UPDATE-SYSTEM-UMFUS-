/*Libraries*/
#include <LSTD_types.h>
#include <LBIT_math.h>
#include <stdio.h>

/*MCAL*/
#include <MRCC_interface.h>
#include <MGPIO_interface.h>
#include <MSTK_interface.h>
#include <MUART_interface.h>
#include <MI2C_interface.h>

/*HAL*/
#include <HNVM_interface.h>



int main(void)
{
	/*System Clock Initialization*/
	MRCC_stderrInitSystemClock(MRCC_SYSCLOCK_HSE_CRYSTAL);

	/*System Buses Clock Initialization*/
	MRCC_stderrInitBusClock(MRCC_BUS_APB2, MRCC_APB2_NO_PRESCALAR);
	MRCC_stderrInitBusClock(MRCC_BUS_APB1, MRCC_APB1_NO_PRESCALAR);

	/*Enable Clock Security System*/
	MRCC_voidEnableCSS();

	/*Enable Clock For Peripherals*/
	MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_GPIOA);
	MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_GPIOB);
	MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_USART2);


	/*UART2 AF GPIO Mode Change*/
	MGPIO_stderrSetPinMode(MGPIO_PIN_A2, MGPIO_MODE_AF_PUSHPULL_2MHZ);
	MGPIO_stderrSetPinMode(MGPIO_PIN_A3, MGPIO_MODE_INPUT_FLOATING);

	/*UART2 Initialization*/
	MUART_stderrInit(MUART_UART2, 0, MUART_DATABITS_8, MUART_PARITY_NONE, MUART_STOPBITS_1);
	MUART_voidEnableTx(MUART_UART2);
	MUART_voidEnableRx(MUART_UART2);

	/*Initialize SysTick*/
	MSTK_stderrInit(MSTK_CLOCKSRC_AHB_8);
	MSTK_voidBusyWait(1000000);

	/*Backup Variables*/
	u8 lastPageFlag = 0;
	u8 backupPageCount = 0;
	u8 burstDataWrite[1024] = {0};
	u8 burstDataRead[1024] = {0};


	/*Debugging Only*/
	/*
	 * char debugData[100]={0};
	 * u8 burstDataWrite[1024] = {16, 103, 161, 26, 8, 253, 231, 123, 217, 141, 96, 223, 25, 71, 82, 168, 246, 104, 198, 54, 252, 0, 129, 114, 143, 65, 187, 28, 237, 139, 31, 103, 64, 113, 74, 63, 36, 174, 48, 234, 19, 78, 196, 232, 65, 233, 213, 167, 136, 92, 176, 93, 16, 32, 246, 115, 178, 182, 214, 134, 178, 254, 164, 230, 161, 32, 116, 249, 65, 156, 72, 158, 158, 198, 33, 230, 170, 45, 43, 71, 35, 14, 20, 129, 34, 180, 118, 218, 148, 106, 177, 174, 175, 244, 49, 118, 181, 4, 89, 36, 82, 124, 38, 155, 64, 230, 18, 21, 215, 121, 203, 29, 72, 232, 4, 79, 68, 94, 120, 9, 252, 168, 31, 161, 4, 55, 121, 209, 65, 117, 88, 254, 248, 61, 112, 187, 206, 115, 230, 137, 228, 118, 55, 157, 128, 4, 79, 98, 231, 107, 255, 241, 192, 115, 175, 59, 139, 245, 74, 101, 116, 190, 189, 223, 89, 244, 142, 142, 167, 59, 160, 198, 95, 91, 170, 82, 96, 8, 39, 211, 156, 138, 14, 146, 69, 51, 228, 30, 83, 87, 252, 50, 3, 55, 253, 212, 83, 49, 18, 88, 19, 185, 45, 129, 85, 68, 95, 182, 113, 234, 120, 182, 225, 236, 101, 144, 173, 115, 54, 230, 232, 43, 81, 150, 66, 253, 120, 71, 47, 76, 46, 110, 166, 70, 250, 66, 127, 179, 20, 120, 133, 26, 113, 83, 128, 222, 92, 51, 227, 121, 194, 113, 52, 101, 60, 114, 80, 19, 243, 33, 72, 217, 236, 169, 109, 187, 125, 232, 3, 78, 173, 143, 90, 40, 224, 46, 100, 127, 26, 18, 29, 71, 103, 164, 179, 102, 137, 137, 17, 211, 91, 22, 206, 165, 198, 75, 207, 121, 68, 232, 160, 11, 133, 146, 253, 235, 74, 95, 217, 124, 140, 160, 227, 86, 196, 249, 227, 48, 117, 118, 185, 12, 155, 209, 52, 30, 157, 176, 56, 187, 230, 111, 105, 200, 102, 111, 246, 23, 48, 243, 225, 8, 154, 21, 179, 71, 128, 107, 65, 227, 236, 48, 183, 80, 113, 43, 131, 184, 212, 196, 124, 119, 81, 166, 132, 158, 17, 183, 192, 17, 48, 242, 72, 27, 119, 166, 173, 212, 106, 152, 135, 107, 22, 126, 83, 108, 202, 30, 24, 137, 204, 70, 36, 23, 199, 141, 112, 160, 54, 207, 137, 41, 33, 82, 113, 122, 45, 162, 221, 251, 89, 160, 121, 200, 218, 187, 114, 9, 10, 216, 20, 122, 117, 84, 157, 19, 18, 222, 70, 143, 88, 105, 3, 152, 82, 168, 34, 28, 157, 163, 8, 52, 107, 28, 239, 229, 157, 193, 43, 211, 33, 109, 248, 100, 129, 140, 166, 9, 49, 150, 40, 206, 243, 171, 54, 184, 61, 54, 211, 177, 252, 16, 251, 0, 122, 42, 62, 80, 140, 244, 241, 60, 213, 144, 121, 156, 18, 153, 234, 94, 169, 243, 40, 119, 103, 217, 65, 213, 49, 99, 141, 172, 122, 159, 2, 72, 208, 46, 9, 183, 45, 158, 156, 188, 213, 51, 174, 221, 136, 104, 248, 14, 13, 193, 124, 170, 101, 130, 68, 41, 230, 217, 80, 116, 143, 114, 3, 108, 92, 182, 72, 89, 121, 102, 191, 23, 82, 2, 38, 139, 195, 228, 210, 248, 29, 17, 64, 138, 114, 51, 169, 50, 148, 96, 131, 190, 22, 188, 97, 120, 1, 84, 66, 100, 94, 73, 115, 224, 48, 112, 189, 179, 75, 5, 254, 169, 48, 140, 164, 207, 49, 185, 177, 236, 234, 77, 137, 215, 32, 169, 178, 41, 196, 130, 75, 187, 89, 83, 126, 217, 181, 25, 150, 194, 77, 220, 106, 49, 249, 224, 163, 255, 214, 215, 145, 34, 139, 215, 229, 199, 220, 197, 30, 102, 138, 228, 90, 115, 140, 54, 30, 190, 163, 215, 146, 167, 64, 88, 74, 47, 34, 136, 138, 31, 246, 13, 35, 157, 4, 141, 135, 162, 200, 48, 66, 6, 247, 205, 230, 101, 250, 219, 195, 169, 117, 11, 239, 73, 37, 69, 125, 137, 129, 119, 128, 118, 93, 221, 18, 247, 47, 28, 211, 165, 182, 171, 50, 130, 193, 174, 213, 255, 168, 84, 128, 209, 165, 158, 123, 251, 38, 166, 147, 123, 108, 167, 100, 83, 142, 240, 20, 121, 14, 153, 235, 98, 82, 50, 11, 84, 245, 204, 95, 96, 188, 197, 201, 50, 94, 83, 28, 254, 64, 131, 99, 185, 144, 178, 65, 128, 148, 115, 60, 54, 187, 55, 108, 170, 204, 167, 222, 255, 17, 168, 222, 199, 118, 79, 84, 106, 252, 59, 3, 143, 41, 8, 32, 169, 65, 53, 22, 7, 192, 124, 181, 165, 110, 233, 21, 172, 228, 122, 219, 174, 122, 162, 111, 109, 201, 107, 39, 154, 192, 89, 235, 185, 120, 28, 11, 148, 194, 34, 185, 130, 160, 57, 91, 99, 65, 247, 82, 211, 186, 49, 216, 147, 227, 24, 17, 206, 254, 64, 57, 111, 151, 246, 80, 198, 18, 75, 241, 75, 237, 197, 75, 86, 174, 187, 134, 153, 162, 9, 127, 108, 1, 202, 4, 35, 140, 170, 62, 197, 45, 110, 122, 3, 210, 106, 204, 219, 38, 177, 218, 44, 32, 82, 33, 227, 160, 138, 133, 209, 152, 51, 197, 163, 3, 20, 0, 122, 85, 44, 111, 66, 129, 195, 223, 176, 234, 87, 242, 236, 132, 91, 96, 5, 44, 73, 183, 190, 26, 57, 136, 2, 111, 168, 251, 1, 116, 175, 67, 5, 47, 147, 251, 101, 10, 84, 42, 232, 36, 67, 2, 15, 251, 108, 52, 167, 134, 88, 100, 218, 18, 165, 116, 16, 34, 101, 138, 71, 49, 24, 122, 218, 173, 117, 127, 203, 8, 179, 119, 66, 150, 101, 148, 154, 109, 3, 34, 249, 75, 233, 206, 156, 255, 106, 246, 213, 249, 31, 187, 37, 26, 160, 253, 212, 131, 178, 215, 177, 119, 62, 102, 249, 94, 173, 163, 208, 98, 8, 217, 17, 191, 229, 138, 130, 43, 72, 194, 113, 249, 202, 231, 195, 131, 150, 134, 14, 243, 129, 84, 70, 239, 199};
	 */


	/*Initialize EEPROM*/
	HNVM_stderrInit();

	/*Backup process*/
	while(!lastPageFlag)
	{
		/*Read From Flash Page by Page until a known delimiter is found*/
		for(int i =0; i<1024; i++)
		{
			burstDataWrite[i] = i /*Replace (i) with byte read from flash*/;

			if(burstDataWrite[i] == '#')
			{
				lastPageFlag = 1;
				MUART_voidSendString(MUART_UART2, "Found Hash\n");
				break;
			}
			else
			{

			}
		}

		/*Write the read page from the flash to EEPROM*/
		for(int i =0; i<1024; i+=64)
		{
			HNVM_stderrBurstWrite(i, &burstDataWrite[i&960], 64);
			MSTK_voidBusyWait(5000);
		}

		backupPageCount++;
	}

	/*Save the number of pages in a location to be known in this case 0x200FF*/
	HNVM_voidWriteByte(0x200FF, backupPageCount);
	MSTK_voidBusyWait(5000);






/*-------------------------------------------------------------------------------------------------------------------------------------------------------------*/


	/*Restore From Backup Process*/


	/*Read the number of pages saved during last backup*/
	backupPageCount = HNVM_u8ReadByte(0x200FF);

	/*Restore From Backup*/
	for(int i =0; i<backupPageCount; i++)
	{
		/*Read a page from EEPROM*/
		HNVM_stderrBurstRead(i*1024 , burstDataRead, 1024);

		/*Write the page into flash using FPEC inside a loop*/
		/*TODO Here*/

		/*Debugging Only*/
		/*
		 * sprintf(debugData, "Start Address: 0x%X\n",i);
		 *MUART_voidSendString(MUART_UART2, debugData);
		 *
		 *for(int j = 0; j<1024; j++)
		 *{
		 *	sprintf(debugData, "Address: 0x%X\tData: 0x%X\n",i+j , burstDataRead[j]);
		 *	MUART_voidSendString(MUART_UART2, debugData);
		 *}
		 */

	}


	for(;;)
	{

	}
}

















