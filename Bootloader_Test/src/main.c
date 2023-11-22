#include <LBIT_math.h>
#include <LSTD_types.h>
#include <MRCC_interface.h>
#include <MSTK_interface.h>
#include <MGPIO_interface.h>



int main(void)
{
	/*System Clock Initialization*/
	MRCC_stderrInitSystemClock(MRCC_SYSCLOCK_HSE_CRYSTAL);
	MRCC_stderrInitBusClock(MRCC_BUS_APB2, MRCC_APB2_NO_PRESCALAR);
	MRCC_stderrInitBusClock(MRCC_BUS_APB1, MRCC_APB1_NO_PRESCALAR);

	/*Enable Clock Security System*/
	MRCC_voidEnableCSS();

	/*Enable Clock For Peripherals*/
	MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_GPIOA);


	/*Pin A0 Output for debugging*/
	MGPIO_stderrSetPinMode(MGPIO_PIN_A0, MGPIO_MODE_OUTPUT_PUSHPULL_2MHZ);
	MGPIO_stderrSetPinMode(MGPIO_PIN_A10, MGPIO_MODE_INPUT_FLOATING);
	/*Initialize SysTick*/
	MSTK_stderrInit(MSTK_CLOCKSRC_AHB_8);
	for(;;)
	{
		MGPIO_stderrSetPinState(MGPIO_PIN_A0, MGPIO_LOW);
		MSTK_voidBusyWait(1000000);
		MGPIO_stderrSetPinState(MGPIO_PIN_A0, MGPIO_HIGH);
		MSTK_voidBusyWait(1000000);
	}
}




