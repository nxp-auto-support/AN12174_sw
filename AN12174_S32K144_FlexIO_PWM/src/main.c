/* LIN protocol emulation details:
 * ==================================================================
 * FlexIO input frequency is the SYSTEM OSC divide by 8 = 1MHz.
 * Interrupts used, DMA not used.
 *
 * A simple State-Machine was implemented to handle all inconsistent
 * LIN frame errors while transmitting.
 *
 * FlexIO settings are based in UART emulation. Available FlexIO
 * resources after this implementation are 2 timers, 2 shifters
 * and 6 pins.
 *
 * If using S32K144 EVB, please connect:
 * - PTD6(LIN_RX) to PTE15(FXIO_D2)
 * - PTD7(LIN_TX) to PTD3(FXIO_D5)
 * And connect EVB to 12V.
 *
 * Please connect LIN, VBAT and GND between both boards,
 * pressing button 0 sends selected LIN frame to slave.
 * */

#include "S32K144.h" 	/* include peripheral declarations S32K144 */
#include "flexio.h"		/* Flexible I/O module */
#include "flexio_pwm.h"


void SystemFIRC_setup(void)
{
	SCG->FIRCDIV |= SCG_FIRCDIV_FIRCDIV2(1); /* Divide by 1 = 48MHz in FIRCDIV2 for FlexIO module */
}

void PORT_setup(void)
{
	PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;	/* CGC=1: Clock enabled for PORTD */
	PORTD->PCR[0]  = PORT_PCR_MUX(6);	/* FXIO_D0 pin output */
}



int main(void)
{
	/*
	 * Variables
	 *
	 */
	uint32_t duty = 100, i;

	/* Initialize SOSC, SPLL and dividers */
	SystemFIRC_setup();

	/* Configure PORTs and GPIOs */
	PORT_setup();

	/* Configure FlexIO regs. for PWM emulation. */
	FLEXIO_enable();

	for(;;)
	{
	flexio_pwm_init(200000, --duty);
	flexio_pwm_start();
			// delay for a while to check PWM
			for (i = 0; i < 100000; i++)
			{
			__asm("nop");
			}
			if (duty == 0)
			{
			duty = 100;
			}

	}

return 0;
}



