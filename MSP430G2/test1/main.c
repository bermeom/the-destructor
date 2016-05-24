#include <msp430.h> 

float dato_;
float id_Motor;
float periodoPWM_Moto1=320;//50k
float periodoPWM_Moto;//50k
float DC_PWM_Motor1=0.5;
float reloj=1e6;
float periodo=320;//clk*0.00002;

void main(void) {
	WDTCTL = WDTPW + WDTHOLD; 			// Stop watchdog timer
	  //---------------------UAT-------------------
	  // Use Calibration values  for 1MHz    Clock   DCO
	  DCOCTL  =   0;
	  BCSCTL1 = CALBC1_1MHZ;
	  DCOCTL  =  CALDCO_1MHZ;
	  P1SEL  =   BIT1    |   BIT2   ;
	  P1SEL2 =   BIT1    |   BIT2;
	  // Place   UCA0    in  Reset   to  be  configured*/
	  UCA0CTL1   =   UCSWRST;
 	  // Configure
	  UCA0CTL1   |=  UCSSEL_2;   //  SMCLK

	  // 1MZ Clk
	  UCA0BR0    =  104;// 16*104;    //  1MHz    9600UBR00=0x82; UBR10=0x06; UMCTL0=0x5B; // uart0 16000000Hz 9599bps
	  UCA0BR1    =   0;  //  1MHz    9600UBR01=0x82; UBR11=0x06; UMCTL1=0x5B; // uart1 16000000Hz 9599bps
	  UCA0MCTL   =   UCBRS0; //  Modulation  UCBRSx  =   1*/
	  /*
	  // 16Mz CLK
	  UCA0BR0 = 130;        // 16MHz 9600
	  UCA0BR1 = 6;          // 16MHz 9600
	  UCA0MCTL = UCBRS1 + UCBRS0; // Modulation UCBRSx = 3*/
	  // Take    UCA0    out of  reset   /
	  UCA0CTL1   &=  ~UCSWRST;
	  //   UCA0CTL1 = UCSWRST;                         // Hold USCI in reset to allow configuration
	  UCA0CTL0 = 0;                               // No parity, LSB first, 8 bits, one stop bit, UART (async)
	  // Enable  USCI_A0 RX  interrupt
	  IE2    |=  UCA0RXIE;



	  BCSCTL1 |= XT2OFF | DIVA_0;
	  BCSCTL3 = XT2S_0 | LFXT1S_2 | XCAP_1;
	  P1DIR |= BIT6;					// P1.6 set as output (Green LED)
	  P1SEL |= BIT6;					// P1.6 selected Timer0_A Out1 output
	  P2DIR |= BIT2;					// P1.6 set as output (Green LED)
	  P2SEL |= BIT2;					// P1.6 selected Timer0_A Out1 output
	  TA0CCTL1 = CM_0 | CCIS_0 | OUTMOD_3;
	  TA1CCTL1 = CM_0 | CCIS_0 | OUTMOD_3;
	  TA1CCTL2 = CM_0 | CCIS_0 | OUTMOD_3;
	  periodo=0;//reloj/(50);
	  TA0CCR0 = periodo; // Cargamos el periodo PWM
	  TA1CCR0 = periodo; // Cargamos el periodo PWM

	  TA0CCR1 = periodo*0.5;
	  TA1CCR1 = periodo*0.5;

	  TA0CTL = TASSEL_2 | ID_0 | MC_1;
	  TA1CTL = TASSEL_2 | ID_0 | MC_1;



	  __bis_SR_register(LPM0_bits + GIE);        // LPM0 (low power mode) interrupts enabled

	}


//  Echo    back    RXed    character,  confirm TX  buffer  is  ready   first
	 #pragma vector=USCIAB0RX_VECTOR
	 __interrupt void    USCI0RX_ISR(void)
	 {
	 	while  (!(IFG2&UCA0TXIFG));    //  USCI_A0 TX  buffer  ready?
	 	UCA0TXBUF  =   UCA0RXBUF&63;  //  TX  -&amp;gt;   RXed    character


	 	id_Motor=(UCA0RXBUF>>6);
		periodoPWM_Moto=(UCA0RXBUF&(63));
		if (periodoPWM_Moto>47){

			periodoPWM_Moto=(reloj/((0)*3));
			TA0CCR0=periodoPWM_Moto;
			TA1CCR0=periodoPWM_Moto;
			TA0CCR1=periodoPWM_Moto*(DC_PWM_Motor1);
			TA1CCR1=periodoPWM_Moto*(DC_PWM_Motor1);
			}else if (id_Motor==0||id_Motor==1){

			 			periodoPWM_Moto=(reloj/((periodoPWM_Moto)*3));
			 			TA0CCR0 =periodoPWM_Moto ; // PWM Period 50us (substract 1 because it's 0-based)
			 			TA0CCR1 = /*periodoPWM_Moto<10?0:*/periodoPWM_Moto*(DC_PWM_Motor1); // CCR1 PWM duty cycle
					}else if (id_Motor==2||id_Motor==3){
							periodoPWM_Moto=(reloj/((periodoPWM_Moto)*3));
							TA1CCR0 =periodoPWM_Moto ; // PWM Period 50us (substract 1 because it's 0-based)
							TA1CCR1 = /*periodoPWM_Moto<10?0:*/periodoPWM_Moto*(DC_PWM_Motor1); // CCR1 PWM duty cycle
					}
	 	//P1OUT ^= LEDG;*/


	 }

