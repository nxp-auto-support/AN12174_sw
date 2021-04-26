/*
 /*
 * main implementation: use this 'C' sample to create your own application
 * FLEXIO I2C bare metal example
 * JUN 30 2016 Osvaldo Romero - Initial version
 *
 * PTE4: FXIOD6: SDA
 * PTE5:FXIOD7: SCL
 * Shifter0: Tx
 * Shifter1: Rx
 * Timer0: SCL
 * Timer1: SDA
 * Baudrate: 100kbps
 *
 *  This examples uses the FLEXIO to emulate the I2C protocol.
 *  PTE4 and PTE5 pins are used.
 *  The S32K144 is the master and communicates with a FXOSFXOS8700CQ accelermeter
 *  with slave address 0x1E.
 *  The S32K144 request the accelerometer ID, activates accelerometer by wiriting
 *  0x01 value to accelerometer CTRL1 reg.
 *  The demo uses the PC terminal to print the data received from the accelerometer.
 *  Every time a key is pressed the S32K144 will read the value of the Z component
 *  of the accelerometer, and print this value in the serial terminal
 *
 */


#include "S32K144.h" /* include peripheral declarations S32K144 */
#include "clocks_and_modes.h"
#include "LPUART.h"

int i=0;



/* Function Prototypes */
void WDOG_Disable(void);
void PORT_init (void);
void FlexIO_I2C_Transmit(uint8_t address, uint8_t *TXDataBuffer,uint8_t num_bytes);
void Flexio_Init_IIC (void);
void FlexIO_IIC_Receive(uint8_t address,uint8_t *RXDataBuffer,uint8_t num_bytes);

/*Global Variables */
uint8_t data_tx[10];
uint8_t data_rx[10];
uint8_t data;


int main(void)
{

	WDOG_Disable();
	SOSC_init_8MHz();        	/* Initialize system oscilator for 8 MHz xtal */
	SPLL_init_80MHz();       	/* Initialize SPLL to 80 MHz with 8 MHz SOSC */
	NormalRUNmode_80MHz();   	/* Init clocks: 80 MHz SPLL & core, 40 MHz bus, 20 MHz flash */
	PORT_init();				/* Init LPUART and FLEXIO  pins */
	init_LPUART1();				/* Initialize LPUART @ 9600*/

	printf("\n\r FLEXIO I2C Demo");					/* Print Welcome msg */
	Flexio_Init_IIC();       						/* Init FLEXIO */
	data_tx[0]=0x0D;    					    	/* WHO AM I register */
	FlexIO_I2C_Transmit(0x1E,data_tx,1);    		/* Write to accelerometer, and request to read WHOAMI register */
	while((FLEXIO->SHIFTSTAT & (1<<0))==0); 		/* Is the buffer empty? */
	FLEXIO->SHIFTBUF[0]=0xFF;	  					/* Fill Tx Shifter with 0xFF to do a continuous start condition*/
	FlexIO_IIC_Receive(0x1E,data_rx,1);				/* Read accelerometer, store the result in data_rx */
	printf("\n\rAccelerometer ID:%d",data_rx[0]);	/* Print the contents in WHOAMI register, ID value should be 199 */

	/* Read the data in CTRL1 reg */
	data_tx[0]=0x2A;								/* 0x2A: CTRL1 REG */
	FlexIO_I2C_Transmit(0x1E,data_tx,1);			/* Write to accelerometer, and request to read CTRL1 register */
	while((FLEXIO->SHIFTSTAT & (1<<0))==0); 		/* Is the buffer empty? */
	FLEXIO->SHIFTBUF[0]=0xFF;	  					/* Fill Tx Shifter with 0xFF to prepare start condition*/
	FlexIO_IIC_Receive(0x1E,data_rx,1);				/* Read  the data in CTRL1 register */

	/* Write in CTRL1 reg */
	data_tx[0]=0x2A;								/* 0x2A: CTRL1 REG */
	data_tx[1]=0x01;								/* Data to write in CTRL 1 reg */
	FlexIO_I2C_Transmit(0x1E,data_tx,2);			/* Write 0x01 to CTRL1 register to activate accelerometer */
	while((FLEXIO->SHIFTSTAT & (1<<0))==0); 		/* Is the buffer empty? */
	FLEXIO->SHIFTBUFBBS[0]=0x00;	  				/* Fill Tx Shifter with 0x00 to do a stop condition*/


	/* Read the data in CTRL1 reg */
	data_tx[0]=0x2A;								/* 0x2A: CTRL1 REG */
	FlexIO_I2C_Transmit(0x1E,data_tx,1);			/* Write to accelerometer, and request to read CTRL1 register */
	while((FLEXIO->SHIFTSTAT & (1<<0))==0); 		/* Is the buffer empty? */
	FLEXIO->SHIFTBUF[0]=0xFF;	  					/* Fill Tx Shifter with 0xFF to prepare start condition*/
	FlexIO_IIC_Receive(0x1E,data_rx,1);				/* Read  the data in CTRL1 register */


	printf("\n\Press a key to trigger an I2C read%");	/* Print instructions */
	for(;;) {
		// receive_char();								/* Wait for a character to be input */
		 data_tx[0]=0x81;								/* 0x05: OUT_Z_MSB */
		 FlexIO_I2C_Transmit(0x1E,data_tx,1);			/* Write to accelerometer, and request to read OUT_Z_MSB register */
		 while((FLEXIO->SHIFTSTAT & (1<<0))==0); 		/* Is the buffer empty? */
		 FLEXIO->SHIFTBUF[0]=0xFF;	  					/* Fill Tx Shifter with 0xFF to prepare start condition*/
		 FlexIO_IIC_Receive(0x1E,data_rx,1);			/* Read  the data in OUT_Z_MSB register */
		 printf("\n\r Data: %d",data_rx[0]);			/* Print OUT_Z_MSB result in terminal */



		}
}



void Flexio_Init_IIC (void)
{

	  PCC->PCCn[PCC_FlexIO_INDEX ]|=PCC_PCCn_PCS(1) 	/* FlexIO src clk: System oscillator */
			  	  	  	  	  	  | PCC_PCCn_CGC_MASK; 	/* Enable clock for PORTE */
	  /* Transmit shifter config */
	  FLEXIO->SHIFTCTL[0]=FLEXIO_SHIFTCTL_SMOD(2) 		/* Shifter mode: Transmit */
						|FLEXIO_SHIFTCTL_PINPOL(1) 		/* Pin active low */
						|FLEXIO_SHIFTCTL_PINSEL(6) 		/* FXDIO6 as SDA*/
						|FLEXIO_SHIFTCTL_PINCFG(1) 		/* Pin config: Open drain*/
						|FLEXIO_SHIFTCTL_TIMPOL(0) 		/* Shift on clk Posedge */
						|FLEXIO_SHIFTCTL_TIMSEL(1);  	/* Shifter Timer 1 is selected for SDA*/
	  FLEXIO->SHIFTCFG[0]=FLEXIO_SHIFTCFG_SSTART(2)  	/* Set shifter start bit to logic low level */
						 |FLEXIO_SHIFTCFG_SSTOP(3)   	/* Stop bit high */
						 |FLEXIO_SHIFTCFG_INSRC(0);  	/* Input source is the pin */

	  /* Receiver shifter config */
	  FLEXIO->SHIFTCTL[1]=FLEXIO_SHIFTCTL_SMOD(1) 	 /* Shifter mode: Receive */
						|FLEXIO_SHIFTCTL_PINPOL(0) 	 /* Pin active high */
						|FLEXIO_SHIFTCTL_PINSEL(6) 	 /* FXDIO6 as SDA*/
						|FLEXIO_SHIFTCTL_PINCFG(0) 	 /* Pin config: output disabled*/
						|FLEXIO_SHIFTCTL_TIMPOL(1) 	 /* Shift on clk negedge */
						|FLEXIO_SHIFTCTL_TIMSEL(1);  /* Shifter Timer 1 is selected for SDA*/
	  FLEXIO->SHIFTCFG[1]=FLEXIO_SHIFTCFG_SSTART(0)  /* Start disable load data on enable */
						 |FLEXIO_SHIFTCFG_SSTOP(2)   /* Stop bit low */
						 |FLEXIO_SHIFTCFG_INSRC(0);  /* Input source is the pin */
	/*SCL timer config */
	  FLEXIO->TIMCFG[0]=FLEXIO_TIMCFG_TSTART(1) 	/* Start bit enabled*/
					   |FLEXIO_TIMCFG_TSTOP(2) 		/* Stop bit enabled on timer disabled*/
					   |FLEXIO_TIMCFG_TIMENA(2) 	/* Timer enable on trigger high*/
					   |FLEXIO_TIMCFG_TIMDIS(2) 	/* Timer disable on timer compare*/
					   |FLEXIO_TIMCFG_TIMRST(2)		/* Timer reset on timer pin = timer output */
					   |FLEXIO_TIMCFG_TIMDEC(0) 	/* Timer decrement on FlexIO clk */
					   |FLEXIO_TIMCFG_TIMOUT(1); 	/* Timer output is zero and is not affected by reset */
	  FLEXIO->TIMCTL[0]=FLEXIO_TIMCTL_TIMOD(1)  	/* Dual 8 bit cntr baud/bit mode */
			  	  	   |FLEXIO_TIMCTL_PINPOL(0) 	/* Pin is active high */
					   |FLEXIO_TIMCTL_PINSEL(7) 	/* FXDIO7 for timer 0 */
					   |FLEXIO_TIMCTL_PINCFG(1) 	/* Pin cfg: open drain */
					   |FLEXIO_TIMCTL_TRGSRC(1) 	/* Internal trigger */
					   |FLEXIO_TIMCTL_TRGPOL(1) 	/* Trg pol: low */
					   |FLEXIO_TIMCTL_TRGSEL(1); 	/* Trg sel: Shifter 0*/



	/*SDA timer config */
	  FLEXIO->TIMCFG[1]=FLEXIO_TIMCFG_TSTART(1) /* Start bit enabled*/
					   |FLEXIO_TIMCFG_TSTOP(1) 	/* Stop bit enabled on timer compare*/
					   |FLEXIO_TIMCFG_TIMENA(1) /* Timer enable on timer 0*/
					   |FLEXIO_TIMCFG_TIMDIS(1) /* Timer disable on timer 0*/
					   |FLEXIO_TIMCFG_TIMRST(0)	/* Timer never reset */
					   |FLEXIO_TIMCFG_TIMDEC(2) /* Timer decrement on pin input*/
					   |FLEXIO_TIMCFG_TIMOUT(0); /* Timer output is one and is not affected by reset */

	  FLEXIO->TIMCTL[1]=FLEXIO_TIMCTL_TIMOD(3)  	/* Single 16 bit mode */
					   |FLEXIO_TIMCTL_PINPOL(1) 	/* Pin is active low */
					   |FLEXIO_TIMCTL_PINSEL(7) 	/* FXDIO7 for timer 	 */
					   |FLEXIO_TIMCTL_PINCFG(0) 	/* Pin cfg: outputdisabled */
					   |FLEXIO_TIMCTL_TRGSRC(1) 	/* Internal trigger */
					   |FLEXIO_TIMCTL_TRGPOL(1) 	/* Trg pol: low */
					   |FLEXIO_TIMCTL_TRGSEL(1); 	/* Trg sel: Shifter 0*/

	 FLEXIO->TIMCMP[1]=0xF;

	 FLEXIO->SHIFTSDEN=FLEXIO_SHIFTSDEN_SSDE(2);  /*Enbale DMA for shifter 1 */
	 FLEXIO->CTRL=FLEXIO_CTRL_FLEXEN_MASK; 		  /*Enable FlexIO */
}


void FlexIO_I2C_Transmit(uint8_t address, uint8_t *TXDataBuffer,uint8_t num_bytes)
{

	uint8_t  i=0;
    FLEXIO->TIMCMP[0]=((((num_bytes+1)*9+1)*2-1)<<8);
    FLEXIO->TIMCMP[0]|=0x25; /* Buadrate 100kbps. ((8MHz/100kbps)/2)-1=39 - 2(due to sync delays)=0x25*/

    FLEXIO->SHIFTCFG[0]|=FLEXIO_SHIFTCFG_SSTOP(3); 	/* Set stop bit high */
    FLEXIO->SHIFTCFG[1]=FLEXIO_SHIFTCFG_SSTOP(2); 	/* Set stop bit low*/

    while((FLEXIO->SHIFTSTAT & (1<<0))==0); 	/* Is the buffer empty? */
    FLEXIO->SHIFTBUFBBS[0]=(address<<1)|0;	    /* Transmit data, device address and write command*/
	while((FLEXIO->SHIFTSTAT & (1<<1))==0); 	/* Is the rx shifter filled */
	(void)FLEXIO->SHIFTBUFBBS[1]; 				/* Read RX shifter */

	for(i=0;i<num_bytes;i++)
	{

	    while((FLEXIO->SHIFTSTAT & (1<<0))==0); 	/* Is the buffer empty? */
	    FLEXIO->SHIFTBUFBBS[0]=*TXDataBuffer++;	  	/* Transmit data, register address to read*/
		while((FLEXIO->SHIFTSTAT & (1<<1))==0); 	/* Is the rx shifter filled */
		(void)FLEXIO->SHIFTBUFBBS[1]; 				/* Read RX shifter */
	}


}

void FlexIO_IIC_Receive(uint8_t address,uint8_t *RXDataBuffer,uint8_t num_bytes)
{
	uint8_t rxdata;
	uint8_t i=0;


    FLEXIO->TIMCMP[0]=((((num_bytes+1)*9+1)*2-1)<<8);
    FLEXIO->TIMCMP[0]|=0x25; /* Buadrate 100kbps. ((8MHz/100kbps)/2)-1=39 - 2(due to sync delays)=0x25*/
	FLEXIO->SHIFTCFG[0]|=FLEXIO_SHIFTCFG_SSTOP(3); 	/* Set stop bit high */
	FLEXIO->SHIFTCFG[1]=FLEXIO_SHIFTCFG_SSTOP(2); 	/* Set stop bit low*/

	while((FLEXIO->SHIFTSTAT & (1<<0))==0); 	/* Is the buffer empty? */
	FLEXIO->SHIFTBUFBBS[0]=(address<<1)|1;	  	/* Transmit data, device address and read command*/
	while((FLEXIO->SHIFTSTAT & (1<<0))==0); 	/* Is the buffer empty? */
    FLEXIO->SHIFTBUFBBS[0]=0xFF;				/* 0xFF in the transmit shifter to tristate the output */
    while((FLEXIO->SHIFTSTAT & (1<<1))==0); 	/* Is the rx shifter filled */
	(void)FLEXIO->SHIFTBUFBBS[1]; 				/* Drain RX shifter */

	for(i=0;i<num_bytes;i++)
	{
	  	while((FLEXIO->SHIFTSTAT & (1<<1))==0); 	/*Is the rx shifter filled */
	  	rxdata=FLEXIO->SHIFTBUFBIS[1]; 				/* Read data */
	    *RXDataBuffer++ = rxdata;
	}



}

void PORT_init (void)
{
	PCC->PCCn[PCC_PORTE_INDEX ]|=PCC_PCCn_CGC_MASK; /* Enable clock for PORTE */
	PORTE->PCR[4]|=PORT_PCR_MUX(6) 	/* Port E4: MUX = ALT6, FXIO */
				|PORT_PCR_PE_MASK	/* Pull resistor: enable */
				|PORT_PCR_PS(1); 	/*Select:Pull up */
	PORTE->PCR[5]|=PORT_PCR_MUX(6) 	/* Port E5: MUX = ALT7, FXIO */
				|PORT_PCR_PE_MASK	/* Pull resistor: enable */
				|PORT_PCR_PS(1);  	/*Select:Pull up */

	PCC->PCCn[PCC_PORTC_INDEX ]|=PCC_PCCn_CGC_MASK; /* Enable clock for PORTC */
	PORTC->PCR[6]|=PORT_PCR_MUX(2);  /* Port C6:  MUX = ALT2,UART2 TX */
	PORTC->PCR[7]|=PORT_PCR_MUX(2);  /* Port C7:  MUX = ALT2,UART2 RX */
}
void WDOG_Disable(void)
{

  	WDOG->CNT = 0xD928C520; 	/*Unlock watchdog*/
	WDOG->TOVAL = 0x0000FFFF;	/*Maximum timeout value*/
	WDOG->CS = 0x00002100;      /*Disable watchdog*/

}

