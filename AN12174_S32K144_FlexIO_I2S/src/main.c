/* FLEXIO UART DEMO USE CASE
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


#include "S32K144.h" /* include peripheral declarations S32K144 */
void WDOG_Disable(void);
void Osc_Init (void);
void Busout_Cfg(void);
void PCC_Init(void);
void FlexIO_I2S_Init(void);
void FlexIO_I2S_Transmit(const unsigned char *TXDataBuffer, uint32_t TransferSize);
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

//	FlexIO_I2S_Init();
//	FlexIO_I2S_Transmit(const unsigned char *TXDataBuffer, uint32_t TransferSize);

	/* Enable FlexIO*/
//	FLEXIO->CTRL|=FLEXIO_CTRL_FLEXEN_MASK;


    /* Send hello string*/
	for (i=0;i<sizeof(string);i++)
	{
		/* Send each character*/
	//	FLEXIO->SHIFTBUF[0]=string[i];
	//	while((FLEXIO->SHIFTSTAT & 0b0001) ==0 );
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

void FlexIO_I2S_Init(void)
{



// FlexIO----DATA---


		FLEXIO ->SHIFTCTL[0]=  FLEXIO_SHIFTCTL_SMOD(2)    /*shifter transmit mode */
							  |FLEXIO_SHIFTCTL_TIMSEL(1) /*timer selection = timer 1*/
							  |FLEXIO_SHIFTCTL_TIMPOL(0) /*timer polarity on Shift on posedge of Shift clock*/
							  |FLEXIO_SHIFTCTL_PINSEL(0) /*pin selection = Pin0 (FXIO_D0)*/
							  |FLEXIO_SHIFTCTL_PINCFG(3) /*pin configuration as pin output*/
							  |FLEXIO_SHIFTCTL_PINPOL(0); /*pin polarity is active high*/

		FLEXIO->SHIFTBUF[0]=0;
		FLEXIO->SHIFTCFG[0]=0;

		  /* Configure FlexIO I2S BCLK Timer */
		FLEXIO->TIMCMP[0] =0x0000007D;
		FLEXIO->TIMCTL[0]=  FLEXIO_TIMCTL_TIMOD(3)  /* timer mode 16-bit counters mode */
				          |FLEXIO_TIMCTL_TRGSEL(4)  /* trigger selection pin 4 status flag */
						  |FLEXIO_TIMCTL_TRGPOL(0)  /* trigger polarity active high */
						  |FLEXIO_TIMCTL_TRGSRC_MASK  /* internal trigger */
						  |FLEXIO_TIMCTL_PINSEL(2)  /* Pin selection = pin2 FXIO_D2 */
						  |FLEXIO_TIMCTL_PINCFG(0)  /* output disable */
						  |FLEXIO_TIMCTL_PINPOL(1); /* pin polarity active low */


		FLEXIO->TIMCFG[0]= FLEXIO_TIMCFG_TIMOUT(0) /*timer initial output, output logic 1 when enable, no affect by reset*/
				          |FLEXIO_TIMCFG_TIMDEC(2) /*timer decrement counter on pin input */
						  |FLEXIO_TIMCFG_TIMENA(4) /*Timer enabled on Pin rising edge*/
						  |FLEXIO_TIMCFG_TIMDIS(3) /*Timer disabled on Timer compare and Trigger Low*/
						  |FLEXIO_TIMCFG_TIMRST(0) /*timer reset, timer never reset*/
						  |FLEXIO_TIMCFG_TSTOP(0) /*stop bit, Stop bit disabled*/
 				    	  |FLEXIO_TIMCFG_TSTART(0); /*disable*/


		/* Configure FlexIO I2S LRCL1K timer */ /* timer1_FlexIO  FlexIO_D2*/
		FLEXIO->TIMCMP[1] = 0x3F;
		FLEXIO->TIMCTL[1]= FLEXIO_TIMCTL_TIMOD(3)  /*timer mode single 16-bits counter mode*/
						  |FLEXIO_TIMCTL_TRGSEL(3) /*trigger pin 3  */
						  |FLEXIO_TIMCTL_TRGPOL(0) /*trigger polarity active low*/
						  |FLEXIO_TIMCTL_TRGSRC_MASK /*trigger internal source select*/
						  |FLEXIO_TIMCTL_PINSEL(1)/*pin selection = pin 1 FXIO_E15*/
						  |FLEXIO_TIMCTL_PINCFG(0) /*pin configuration output enable */
						  |FLEXIO_TIMCTL_PINPOL(0); /*pin polarity active high*/
		FLEXIO->SHIFTBUF[0]=0;
		FLEXIO->TIMCFG[1]= FLEXIO_TIMCFG_TIMOUT(0) /*Timer output is logic zero when enabled and on timer reset*/
						  |FLEXIO_TIMCFG_TIMDEC(2) /*timer decrement counter on FlexIO clock, shift clock equals timer output*/
						  |FLEXIO_TIMCFG_TIMENA(5) /*timer enable condition on timer0 enable,Timer enabled on Timer N-1 enable */
						  |FLEXIO_TIMCFG_TIMDIS(3) /*Timer never disabled*/
						  |FLEXIO_TIMCFG_TIMRST(0) /* timer never reset*/
						  |FLEXIO_TIMCFG_TSTART(0); /*disable*/

		FLEXIO->SHIFTSTAT=0;
		FLEXIO->SHIFTSDEN=0x00000001;
}

void FlexIO_I2S_Transmit(const unsigned char *TXDataBuffer, uint32_t TransferSize)
{
    uint32_t i, writedata=0;
    for (i=0; i<1/*TransferSize*/; i++/*=2*/)
    {
       /* Send each character - Left Channel */
        writedata = ((uint32_t )TXDataBuffer[i])<<24;
        writedata |= ((uint32_t )TXDataBuffer[i+1])<<16;
        writedata |= ((uint32_t )TXDataBuffer[i+1])<<8;
        writedata |= ((uint32_t )TXDataBuffer[i+1]);
        FLEXIO->SHIFTBUFBIS[0] = 0xFFFFFFF1; //0xFFFFFFF8;//writedata; SHIFTBUF  SHIFTBUFBIS
        while( (FLEXIO->SHIFTSTAT & 0x1) == 0 );
        /* Send each character - Right Channel */
        FLEXIO->SHIFTBUF[0] = 0xFFFFFFF8;//0xFFFFFFFE;//writedata;
        while( (FLEXIO->SHIFTSTAT & 0x1) == 0 );
    }
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
