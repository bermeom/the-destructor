//***************************************************************************************
//  MSP430 Blink the LED Demo - Software Toggle P1.0
//
//  Description; Toggle P1.0 by xor'ing P1.0 inside of a software loop.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//                MSP430x5xx
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
//
//  J. Stevenson
//  Texas Instruments, Inc
//  July 2011
//  Built with Code Composer Studio v5
//***************************************************************************************

#include <msp430.h>				
#define TIMER_OUT BIT6
#define LEDG BIT6

const unsigned RXD = BIT1;
const unsigned TXD = BIT2;
const unsigned SW2 = BIT3;

const unsigned long smclk_freq = 16000000;      // SMCLK frequency in hertz
const unsigned long bps = 9600;                 // Async serial bit rate


float id_Motor;
float periodoPWM_Moto1=320;//50k
float DC_PWM_Motor1=0.5;
float reloj=16e6;
int main(void) {
	/*WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
	P1DIR |= 0x01;					// Set P1.0 to output direction

	for(;;) {
		volatile unsigned int i;	// volatile to prevent optimization

		P1OUT ^= 0x01;				// Toggle P1.0 using exclusive-OR

		i = 10000;					// SW Delay
		do i--;
		while(i != 0);
	}

	WDTCTL = WDTPW + WDTHOLD; // Stop WDT
	  if (CALBC1_1MHZ==0xFF)        // If calibration constant erased
	  {
	    while(1);                               // do not load, trap CPU!!
	  }
	  DCOCTL = 0;                               // Select lowest DCOx and MODx settings
	  BCSCTL1 = CALBC1_16MHZ;                    // Set range
	  DCOCTL = CALDCO_16MHZ;                     // Set DCO step + modulation
	  P1DIR |= TIMER_OUT;
	  P1SEL |= TIMER_OUT;
	  float periodo=320;//50k
	  TACCR0 =periodo ; // PWM Period 50us (substract 1 because it's 0-based)
	  TACCTL1 = OUTMOD_3;
	  TACCR1 = periodo*(1-0.15); // CCR1 PWM duty cycle
	  TACTL = TASSEL_2 + MC_1; // SMCLK, up mode
	  _BIS_SR(CPUOFF); // Enter LPM0

	return 0;
}
*/



		WDTCTL =   WDTPW   +   WDTHOLD;    //  Stop    WDT
	 	P1DIR |= TIMER_OUT;
	 	P2DIR = BIT5; // P2.5 als Ausgang


	 	// Use Calibration values  for 1MHz    Clock   DCO
	 	DCOCTL =   0;
	 	BCSCTL1    =   CALBC1_1MHZ;
	 	DCOCTL =   CALDCO_1MHZ;

	 	// Configure   Pin Muxing  P1.1    RXD and P1.2    TXD
	 	P1SEL  =   BIT1    |   BIT2 | TIMER_OUT   ;
	 	P1SEL2 =   BIT1    |   BIT2;
	 	P2SEL  =   BIT5 ;
		P2SEL2 =   BIT5 ;


	 	///PWM 1 1.6
	 	TACCR0 =periodoPWM_Moto1 ; // PWM Period 50us (substract 1 because it's 0-based)
    	TACCTL1 = OUTMOD_3;

    	TACCR1 = periodoPWM_Moto1*(1-DC_PWM_Motor1); // CCR1 PWM duty cycle
    	TACTL = TASSEL_2 + MC_1; // SMCLK, up mode

    	//PWM 2
    	//P2OUT = ~BIT5;                   // Toggle P2.5
    	TACCR2 = periodoPWM_Moto1; // CCR1 PWM duty cycle
    	//TA1CCR1 = periodoPWM_Moto1*(1-DC_PWM_Motor1);// Berechnung Frequenz in ADC Wert 0 - 50Hz
		TA1CCTL2 = OUTMOD_0;
		TA1CTL = TASSEL_2 + MC_3 + TACLR;                  // SMCLK, up-down mode

		 const unsigned long brd = (smclk_freq + (bps >> 1)) / bps; // Bit rate divisor

	 	// Place   UCA0    in  Reset   to  be  configured*/
	 	UCA0CTL1   =   UCSWRST;

	 	// Configure
	 	UCA0CTL1   |=  UCSSEL_2;   //  SMCLK

		UCA0BR0    =  104;// 16*104;    //  1MHz    9600UBR00=0x82; UBR10=0x06; UMCTL0=0x5B; // uart0 16000000Hz 9599bps
		UCA0BR1    =   0;  //  1MHz    9600UBR01=0x82; UBR11=0x06; UMCTL1=0x5B; // uart1 16000000Hz 9599bps
		UCA0MCTL   =   UCBRS0; //  Modulation  UCBRSx  =   1*/

		/*UCA0BR0 = 130;        // 16MHz 9600
	 	UCA0BR1 = 6;          // 16MHz 9600
	 	UCA0MCTL = UCBRS1 + UCBRS0; // Modulation UCBRSx = 3*/
	 	//UCA0BR1 = (brd >> 12) & 0xFF;               // High byte of whole divisor
		//UCA0BR0 = (brd >> 4) & 0xFF;                // Low byte of whole divisor
		//UCA0MCTL = ((brd << 4) & 0xF0) | UCOS16;    // Fractional divisor, oversampling mode


	 	// Take    UCA0    out of  reset   /
	 	UCA0CTL1   &=  ~UCSWRST;

	 	                                           //
	 	 //   UCA0CTL1 = UCSWRST;                         // Hold USCI in reset to allow configuration
	 	 UCA0CTL0 = 0;                               // No parity, LSB first, 8 bits, one stop bit, UART (async)


	 	 	// Enable  USCI_A0 RX  interrupt
	 	IE2    |=  UCA0RXIE;

	 	__bis_SR_register(LPM0_bits    +   GIE);   //  Enter   LPM0,   interrupts  enabled



 }

	 //  Echo    back    RXed    character,  confirm TX  buffer  is  ready   first
	 #pragma vector=USCIAB0RX_VECTOR
	 __interrupt void    USCI0RX_ISR(void)
	 {
	 	while  (!(IFG2&UCA0TXIFG));    //  USCI_A0 TX  buffer  ready?
	 	UCA0TXBUF  =   UCA0RXBUF;  //  TX  -&amp;gt;   RXed    character

	 	id_Motor=UCA0RXBUF>>6;
	 	if (id_Motor==3||id_Motor==2){

	 		if ((UCA0RXBUF&(63))>47){
	 			UCA0TXBUF=0;
	 		}
			periodoPWM_Moto1=reloj/((UCA0RXBUF&(63))*3);
	 		TACCR0 =periodoPWM_Moto1 ; // PWM Period 50us (substract 1 because it's 0-based)
	 	 	TACCR1 = periodoPWM_Moto1*(1-DC_PWM_Motor1); // CCR1 PWM duty cycle

	 	}
	 	//P1OUT ^= LEDG;*/


	 }
//*/
