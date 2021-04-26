
#include "flexio.h"
uint16_t gu16CMP_Low = 0;	/* Global value for TIMCMP[7:0] */

void FLEXIO_enable(void)
{
	 PCC->PCCn[PCC_FlexIO_INDEX] &= ~PCC_PCCn_CGC_MASK;		/* Disable FLEXIO clock 	*/
	 PCC->PCCn[PCC_FlexIO_INDEX] |= PCC_PCCn_PCS(0b11)			/* FIRCDIV2_CLK selected. 	*/
			 	 	 	 	 	 |  PCC_PCCn_CGC_MASK;		/* Enable FLEXIO clock 		*/
}

void FLEXIO_start(void)
{
	FLEXIO->CTRL = FLEXIO_CTRL_FLEXEN_MASK;		/* Enable FlexIO module */
}

