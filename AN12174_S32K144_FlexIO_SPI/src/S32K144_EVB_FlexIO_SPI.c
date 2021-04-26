/*
 * main implementation: use this 'C' sample to create your own application
 *
 */


#include "derivative.h" /* include peripheral declarations S32K144 */
void WDOG_Disable(void);
void Osc_Init (void);
void Busout_Cfg(void);
void PCC_Init(void);
void FlexIO_config0(void);
void FlexIO_Config1(void);
void timer0_FlexIO_config(void);
void timer1_FlexIO_config(void);

int main(void)
{
	WDOG_Disable();
		/* Enable OSC on PTE4*/
		Busout_Cfg();
		/* Enable clocks in FLEXIO,PORTA,SCG*/
		PCC_Init();
		/*Init 8MHz board Extal*/
		Osc_Init();
		FLEXIO->CTRL= FLEXIO_CTRL_FLEXEN_MASK; /*enable FlexIO module*/
		FlexIO_config0();
		FlexIO_Config1();
		timer0_FlexIO_config();
		timer1_FlexIO_config();



	char b,a=0x55;

	for(;;) {	   
		/* Receive data from serial*/
			//while( !(FLEXIO->SHIFTSTAT&(1<<(1)<<FLEXIO_SHIFTSTAT_SSF_SHIFT))){};
			//b=FLEXIO->SHIFTBUFBBS[0];
			//a=b;

			/* Send data to serial*/
					FLEXIO->SHIFTBUFBBS[1]=a;
					while( !(FLEXIO->SHIFTSTAT&(1<<(0)<<FLEXIO_SHIFTSTAT_SSF_SHIFT))){};

	}
	
	return 0;
}




void FlexIO_config0(void)
		{

	PORTD->PCR[0]|=PORT_PCR_MUX(6);  /* Select PTD0 MUX as FlexIO_D0 MISO */
	FLEXIO ->SHIFTCTL[0]= FLEXIO_SHIFTCTL_SMOD(2)    /*shifter transmit mode */
						  |FLEXIO_SHIFTCTL_TIMSEL(0) /*timer selection = timer 0*/
						  |FLEXIO_SHIFTCTL_TIMPOL(1) /*timer polarity on negative of shift clock*/
						  |FLEXIO_SHIFTCTL_PINSEL(0) /*pin selection = Pin0 (FXIO_D0)*/
						  |FLEXIO_SHIFTCTL_PINCFG(3) /*pin configuration as pin output*/
						  |FLEXIO_SHIFTCTL_PINPOL(0); /*pin polarity is active high*/

	FLEXIO->SHIFTCFG[0]=  FLEXIO_SHIFTCFG_SSTART(0) /*start bit, disable transmit load data on enable */
			              |FLEXIO_SHIFTCFG_SSTOP(0) /*stop bit, disable */
						  |FLEXIO_SHIFTCFG_INSRC(0); /*Selects the input source for the shifter.*/

							/* bit byte swapped register get buffer*/
		}
void FlexIO_Config1(void)
{
	PORTD->PCR[1]|=PORT_PCR_MUX(6);                  /* Select PTD1 MUX as FlexIO_D1 MOSI */
	FLEXIO ->SHIFTCTL[1]= FLEXIO_SHIFTCTL_SMOD(1)    /* shifter receive mode */
						 |FLEXIO_SHIFTCTL_TIMSEL(0)  /* timer select = timer 0 */
						 |FLEXIO_SHIFTCTL_TIMPOL(0)	 /* timer polarity on positive shift clock */
						 |FLEXIO_SHIFTCTL_PINSEL(1)  /* pin selection = pin1 (FXIO_D1) */
						 |FLEXIO_SHIFTCTL_PINCFG(0)  /* pin configuration as output disable */
						 |FLEXIO_SHIFTCTL_PINPOL(0); /* pin polarity active high */

	FLEXIO->SHIFTCFG[1]=FLEXIO_SHIFTCFG_SSTART(0)    /* start bit, disable transmitter loads data on enable */
					    |FLEXIO_SHIFTCFG_SSTOP(0)    /* stop bit, disable */
						|FLEXIO_SHIFTCFG_INSRC(0);   /* Selects the input source for the shifter. */

						/* bit byte swapped register */

}
void timer0_FlexIO_config (void)                      /* SPI_CLK FlexIO_D2 */
{
	PORTE->PCR[15]|=PORT_PCR_MUX(6);				  /* Select PTE15 MUX as FlexIO_D2 CLK */
	FLEXIO->TIMCTL[0]= FLEXIO_TIMCTL_TIMOD(1)         /* timer mode dual 8-bit counters baud/bit mode */
			          |FLEXIO_TIMCTL_TRGSEL(1) 		  /* trigger selection shifter 0 status flag */
					  |FLEXIO_TIMCTL_TRGPOL(1) 		  /* trigger polarity active low */
					  |FLEXIO_TIMCTL_TRGSRC_MASK      /* internal trigger */
					  |FLEXIO_TIMCTL_PINSEL(2)        /* Pin selection = pin2 FXIO_D2 */
					  |FLEXIO_TIMCTL_PINCFG(3)        /* output enable */
					  |FLEXIO_TIMCTL_PINPOL(0);       /* pin polarity active high */

	FLEXIO->TIMCFG[0]= FLEXIO_TIMCFG_TIMOUT(1) /*timer initial output, output logic 0 when enable, no affect by reset*/
			          |FLEXIO_TIMCFG_TIMDEC(0) /*timer decrement source, decrement on FlexIO clock, shift on timer output */
					  |FLEXIO_TIMCFG_TIMENA(2) /*timer enable condition, on trigger high*/
					  |FLEXIO_TIMCFG_TIMDIS(2) /*timer disable condition, on timer compare*/
					  |FLEXIO_TIMCFG_TIMRST(0) /*timer reset, timer never reset*/
					  |FLEXIO_TIMCFG_TSTART_MASK /*start bit, enable*/
					  |FLEXIO_TIMCFG_TSTOP(2); /*stop bit, enable on timer disable*/

	FLEXIO->TIMCMP[0] = 0xF50;//29 /*timer compare value,(((flexioClk / baudRate) / 2) - 1);---((n*2-1)<<8) | (baudrate_divider/2-1)) n= bit to be sending   */
}														/*(((42M/500k)/2)-1)=0x17*/
void timer1_FlexIO_config (void)/*SPI_CS FlexIO_D3*/
{
	PORTE->PCR[16]|=PORT_PCR_MUX(6);
	FLEXIO->TIMCTL[1]= FLEXIO_TIMCTL_TIMOD(3)  /*timer mode single 16-bits counter mode*/
					  |FLEXIO_TIMCTL_TRGSEL(1) /*trigger selection trigger from timer0 */
					  |FLEXIO_TIMCTL_TRGPOL(0) /*trigger polarity active high*/
					  |FLEXIO_TIMCTL_TRGSRC_MASK /*trigger internal source select*/
					  |FLEXIO_TIMCTL_PINSEL(3)/*pin selection = pin 3 FXIO_D3*/
					  |FLEXIO_TIMCTL_PINCFG(3) /*pin configuration output enable */
					  |FLEXIO_TIMCTL_PINPOL(1); /*pin polarity active low*/

	FLEXIO->TIMCFG[1]= FLEXIO_TIMCFG_TIMOUT(0) /*output logic 1 when enabled, not affect by reset*/
					  |FLEXIO_TIMCFG_TIMDEC(0) /*timer decrement counter on FlexIO clock, shift clock equals timer output*/
					  |FLEXIO_TIMCFG_TIMENA(1) /*timer enable condition on timer0 enable */
					  |FLEXIO_TIMCFG_TIMDIS(1) /*timer disable condition on timer 0 disable*/
					  |FLEXIO_TIMCFG_TIMRST(0) /* timer never reset*/
					  |FLEXIO_TIMCFG_TSTART(0); /*disable*/


	FLEXIO->TIMCMP[1] = 0xFFFF;//F17
}





void PCC_Init(void)
{
	PCC->PCCn[PCC_FlexIO_INDEX ]|=PCC_PCCn_PCS(0b01); /*OSC clock for FlexIO */
	PCC->PCCn[PCC_FlexIO_INDEX ]|=PCC_PCCn_CGC_MASK; /* Enable clock for FlexIO */
	PCC->PCCn[PCC_PORTD_INDEX ]|=PCC_PCCn_CGC_MASK; /* Enable clock for PORTD */
	PCC->PCCn[PCC_PORTE_INDEX ]|=PCC_PCCn_CGC_MASK; /* Enable clock for PORTE */
}

void WDOG_Disable(void)
{
	WDOG->CNT = 0xD928C520;                // unlock watchdog
	WDOG->TOVAL = 0x0000FFFF;            // maximum timeout value
	WDOG->CS = 0x00002100;                // disable watchdog

}

void Osc_Init (void)
{
    /* SOSC Configuration */
    SCG->SOSCDIV = 0x00010101u;  /* SCG_SOSCDIV: SOSCDIV3=1, SOSCDIV2=1, SOSCDIV1=1 */
    SCG->SOSCCFG = 0x00000024u;  /* SCG_SOSCCFG: RANGE=2, EREFS=1 */
    while(SCG->SOSCCSR & 0x00800000u); /* Control and status register can be written?*/
    SCG->SOSCCSR= 0x5u;           /* SCG_SOSCCSR: SOSCLPEN=1, SOSCEN=1 */
    while(!(SCG->SOSCCSR & 0x01000000u));
}


void Busout_Cfg(void)
{
	PCC->PCCn[PCC_PORTE_INDEX ]|=PCC_PCCn_CGC_MASK; /* Enable clock for PORTE */
    PORTE->PCR[4] |= 0x01000240;                        // configure Pin PTE4 as CLKOUT
}
