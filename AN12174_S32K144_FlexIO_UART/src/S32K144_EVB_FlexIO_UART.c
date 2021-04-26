/*
 * FLEXIO UART DEMO USE CASE
 * Rev1.0
 * Demo will show the UART communication using the FlexIO module with following parameters:
 * 8-bit communication
 * One stop bit
 * No parity
 * No hardware flow control
 * Baud rate: 115200
 * Hardware used: S32K144 EVB
 * PTA0: FlexIO_D2 as TX
 * PTA1: FlexIO_D3 as RX
 */


#include "derivative.h" /* include peripheral declarations S32K144 */
void WDOG_Disable(void);
void Osc_Init (void);
void Busout_Cfg(void);
void PCC_Init(void);
void Flexio_Uart_Tx_Init(void);
void Flexio_Uart_Rx_Init(void);
char string[] = "\rHello World!!!\n\r";

char i=0;
char data=0x55;
int main(void)
{
	int counter = 0;

	WDOG_Disable();
	/* Enable OSC on PTE4*/
	Busout_Cfg();
	/* Enable clocks in FLEXIO,PORTA,SCG*/
	PCC_Init();
	/*Init 8MHz board Extal*/
	Osc_Init();

	Flexio_Uart_Tx_Init();
	Flexio_Uart_Rx_Init();

	/* Enable FlexIO*/
	FLEXIO->CTRL|=FLEXIO_CTRL_FLEXEN_MASK;


    /* Send hello string*/
	for (i=0;i<sizeof(string);i++)
	{
		/* Send each character*/
		FLEXIO->SHIFTBUF[0]=string[i];
		while((FLEXIO->SHIFTSTAT & 0b0001) ==0 );
	}


	for(;;) {
	   	counter++;
	   	/* Receive data from serial*/
	//	while( !(FLEXIO->SHIFTSTAT&(1<<(1)<<FLEXIO_SHIFTSTAT_SSF_SHIFT))){};
	//	data=FLEXIO->SHIFTBUFBYS[1];

		/* Send data to serial*/
		FLEXIO->SHIFTBUF[0]=data;
		while( !(FLEXIO->SHIFTSTAT&(1<<(0)<<FLEXIO_SHIFTSTAT_SSF_SHIFT))){};


	}

	return 0;
}

void Flexio_Uart_Tx_Init(void)
{

	PORTA->PCR[0]|=PORT_PCR_MUX(0b100);  /* Select PTA0 MUX as FXIOD2 */

	/* Shifter Configuration*/
	FLEXIO->SHIFTCFG[0]|= FLEXIO_SHIFTCFG_SSTOP(0x03)   /* Stop bit enable on logic 1*/
					   |  FLEXIO_SHIFTCFG_SSTART(0x02); /* Start bit 0 before loading the data on first shift*/

	FLEXIO->SHIFTCTL[0]|= FLEXIO_SHIFTCTL_TIMSEL(0X00)  /* Select Timer 0*/
					   |  FLEXIO_SHIFTCTL_TIMPOL(0x00)  /* Shift on pos edge*/
					   |  FLEXIO_SHIFTCTL_PINCFG(0x03)  /* Shift as output*/
					   |  FLEXIO_SHIFTCTL_PINSEL(0x02)  /* FXIO D2 as Tx*/
					   |  FLEXIO_SHIFTCTL_PINPOL(0x00)  /* Pin active high*/
					   |  FLEXIO_SHIFTCTL_SMOD(0x02);    /* Shifter in Tx mode*/


	/* Timer Configuration*/
	FLEXIO->TIMCFG[0]|= FLEXIO_TIMCFG_TIMDEC(0x0) /* Counter Decrement of FlexIO clock*/
					 | FLEXIO_TIMCFG_TIMDIS(0x02) /* Timer disable on Timer compare*/
					 | FLEXIO_TIMCFG_TIMENA(0x02) /* Timer enable on trigger high*/
					 | FLEXIO_TIMCFG_TSTOP(0x02) /* Shifter to generate stop bit on timer compare*/
					 | FLEXIO_TIMCFG_TSTART(0x01); /* Enable start bit*/


	FLEXIO->TIMCTL[0]|= FLEXIO_TIMCTL_TRGSEL(0x01)     /* Trigger on shifter flag*/
					 |  FLEXIO_TIMCTL_TRGPOL(0x01)    /* Trigger active LOW*/
					 |  FLEXIO_TIMCTL_TRGSRC(0x01)    /* Internal Trigger*/
					 |  FLEXIO_TIMCTL_PINSEL(0x02)    /* Select FXIO D2*/
					 |  FLEXIO_TIMCTL_PINCFG(0x00)    /* FXDIO disabled*/
					 |  FLEXIO_TIMCTL_TIMOD(0x01);     /* Baud rate mode*/


	/* Time Compare*/
	FLEXIO->TIMCMP[0]=0x0F21;  /* Baud rate at 115200*/

}


void Flexio_Uart_Rx_Init(void)
{

	PORTA->PCR[1]|=PORT_PCR_MUX(0b100);  /* Select PTA1 MUX as FXIOD3 */

	/* Shifter Configuratio*/
	FLEXIO->SHIFTCFG[1]|= FLEXIO_SHIFTCFG_INSRC(0x00)	/* Pin Source*/
						|	FLEXIO_SHIFTCFG_SSTOP(0x03) /* Stop bit enable on logic 1*/
						|	FLEXIO_SHIFTCFG_SSTART(0x02); /* Start bit 0*/

	FLEXIO->SHIFTCTL[1]|= FLEXIO_SHIFTCTL_TIMSEL(0x01)  /* Select Timer 1*/
						|  FLEXIO_SHIFTCTL_TIMPOL(0x01)  /* Shift on neg edge*/
						|  FLEXIO_SHIFTCTL_PINCFG(0x00)  /* Pin disabled*/
						|  FLEXIO_SHIFTCTL_PINSEL(0x03)  /* FXIO D3*/
						|  FLEXIO_SHIFTCTL_PINPOL(0x00)  /* Pin active high*/
						|  FLEXIO_SHIFTCTL_SMOD(0x01);    /* Shifter in Rx mode*/

	/*Timer Configuration*/
	FLEXIO->TIMCFG[1]|= FLEXIO_TIMCFG_TIMDEC(0x0) /* Counter Decrement of FlexIO clock*/
						 | FLEXIO_TIMCFG_TIMDIS(0x02) /* Timer disable on Timer compare*/
						 | FLEXIO_TIMCFG_TIMENA(0x04) /* Timer enable on pin rising edge*/
						 | FLEXIO_TIMCFG_TSTOP(0x02) /* Shifter to generate stop bit on timer compare*/
						 | FLEXIO_TIMCFG_TSTART(0x01); /* Enable start bit*/

	FLEXIO->TIMCTL[1]|= FLEXIO_TIMCTL_TRGSEL(0b0010)     /* Trigger on input pin 3 */
							|FLEXIO_TIMCTL_TRGPOL(0x00)    /* Trigger active LOW*/
							|FLEXIO_TIMCTL_TRGSRC(0x01)    /* Internal Trigger*/
							|FLEXIO_TIMCTL_PINSEL(0x03)    /* Select FXIO D3*/
							|FLEXIO_TIMCTL_PINPOL(0x01)    /* PIN pol low*/
							|FLEXIO_TIMCTL_PINCFG(0x00)    /* FXDIO disabled*/
							|FLEXIO_TIMCTL_TIMOD(0x01);     /* Baud rate mode*/
    /*Timer Compare*/
	FLEXIO->TIMCMP[1]=0x0F21;  /* Baud rate at 115200*/



}

void WDOG_Disable(void)
{
	WDOG->CNT = 0xD928C520;                // unlock watchdog
	WDOG->TOVAL = 0x0000FFFF;            // maximum timeout value
	WDOG->CS = 0x00002100;                // disable watchdog
	
}
void PCC_Init(void)
{
	PCC->PCCn[PCC_FlexIO_INDEX ]|=PCC_PCCn_PCS(0b01); /*OSC clock for FlexIO */
	PCC->PCCn[PCC_FlexIO_INDEX ]|=PCC_PCCn_CGC_MASK; /* Enable clock for FlexIO */
	PCC->PCCn[PCC_PORTA_INDEX ]|=PCC_PCCn_CGC_MASK; /* Enable clock for PORTA */
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
	PCC->PCCn[PCC_PORTE_INDEX ]|=PCC_PCCn_CGC_MASK; /* Enable clock for PORTC */
    PORTE->PCR[4] = 0x01000240;                        // configure Pin PTE4 as CLKOUT
}
