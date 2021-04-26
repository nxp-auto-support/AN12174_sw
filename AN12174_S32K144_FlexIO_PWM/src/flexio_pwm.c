/*
 * flexio_pwm.c
 *
 *  Created on: Apr 30, 2018
 *      Author: nxa19513
 */
#include "flexio_pwm.h"
#include "flexio.h"

 void flexio_pwm_init(uint32_t freq, uint8_t duty){


	    uint32_t low, high, i,timcmp;

	    // check param
	    if (freq > MAX_FREQ || freq < MIN_FREQ)
	    {
	        return;
	    }
	    if (duty > 99 || duty == 0)
	    {
	        duty = 50;
	    }


    FLEXIO->TIMCTL[0] = FLEXIO_TIMCTL_TIMOD(FLEXIO_TIMER_MODE_DISABLED)/*FLEXIO_TIMER_MODE_8BIT_BAUD */
						| FLEXIO_TIMCTL_PINSEL(FlexioPin0)
						| FLEXIO_TIMCTL_PINPOL(FLEXIO_PIN_POLARITY_HIGH)
						| FLEXIO_TIMCTL_PINCFG(FLEXIO_PIN_CONFIG_OUTPUT)
						| FLEXIO_TIMCTL_TRGSRC(FLEXIO_TRIGGER_SOURCE_INTERNAL)
						| FLEXIO_TIMCTL_TRGPOL(FLEXIO_TRIGGER_POLARITY_LOW)
						| FLEXIO_TIMCTL_TRGSEL(0);

	FLEXIO->TIMCFG[0] = FLEXIO_TIMCFG_TSTART(FLEXIO_TIMER_START_BIT_DISABLED)
						| FLEXIO_TIMCFG_TSTOP(FLEXIO_TIMER_STOP_BIT_DISABLED)
						| FLEXIO_TIMCFG_TIMENA(FLEXIO_TIMER_ENABLE_ALWAYS)
						| FLEXIO_TIMCFG_TIMDIS(FLEXIO_TIMER_DISABLE_NEVER)
						| FLEXIO_TIMCFG_TIMRST(FLEXIO_TIMER_RESET_NEVER)
						| FLEXIO_TIMCFG_TIMDEC(FLEXIO_TIMER_DECREMENT_CLK_SHIFT_TMR)
						| FLEXIO_TIMCFG_TIMOUT(FLEXIO_TIMER_INITOUT_ONE);
		/*
	     * Set TIMCMP[15:8] = low output period - 1
	     * Set TIMCMP[7:0] = high output period - 1
	     */
	    high = (FLEXIO_CLK / freq) * duty / 100;
	    low  = (FLEXIO_CLK / freq) * (100 - duty) / 100;
	    timcmp = ((low - 1) << 8) | (high - 1);

	    FLEXIO->TIMCMP[0] = (uint32_t)timcmp;
 }


 void flexio_init(void)
{

    // Reset the FlexIO.
    FLEXIO->CTRL|= FLEXIO_CTRL_FLEXEN(false);// Disable the Flexio first.
    FLEXIO->CTRL|= FLEXIO_CTRL_SWRST(true);
    FLEXIO->CTRL|= FLEXIO_CTRL_SWRST(false);
    // Enable FlexIO work in debug mode, Fase access.
    FLEXIO_SetDebugMode(FLEXIO, true);
    FLEXIO->CTRL |= FLEXIO_CTRL_FASTACC(true);
    FLEXIO->CTRL|= FLEXIO_CTRL_FLEXEN(true);
}

 void flexio_pwm_start(void)
{
	FLEXIO->TIMCTL[0] |= FLEXIO_TIMCTL_TIMOD(FLEXIO_TIMER_MODE_8BIT_PWM);
}

 void flexio_pwm_stop(void)
{
	FLEXIO->TIMCTL[0] |= FLEXIO_TIMCTL_TIMOD(FLEXIO_TIMER_MODE_DISABLED);
}
