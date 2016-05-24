/*
 */
#include <msp430.h>


int id_Motor;
int sentidoMotor2=0;
int sentidoMotor1=0;


float periodoPWM_Moto1=320;//50k
float DC_PWM_Motor1=0.5;
float reloj=16e6;


long Impresion;
float PWM1;
float dato1;
float PWM2;
float dato2;
float PWM3;
float dato3;
float clk=16e6;
float periodo=320;//clk*0.00002;
int TABLA[]={'V','O','L','T','A','G','E','=','*','*','*','*','m','V',0x0D};
int i;
unsigned int ADC[7] = { 7, 8, 9, 10, 11, 12,13 };

void uart_init( void ) {
    /**
     * 1. Set UCSWRST
     * 2. Initialize all USCI registers
     * 3. Configure ports
     * 4. Clear UCSWRST
     * 5. Enable interrupts via UCxRXIE and/or UCxTXIE
     */

    // (1) Set state machine to the reset state
    UCA0CTL1 = UCSWRST;

    // (2) Initialize USCI registers
    UCA0CTL1 |= UCSSEL_2;                                 // CLK = SMCLK
    UCA0BR0 = 104;                                        // 1MHz/9600 =104
    UCA0BR1 = 0x00;                                       // from User Guide table 15-4
    UCA0MCTL = UCBRS0;                                    // Modulation UCBRSx = 1


    // (3) Configure ports
    P1SEL = BIT1 + BIT2;                                  // P1.1 = RXD, P1.2=TXD
    P1SEL2= BIT1 + BIT2;                                  // P1.1 = RXD, P1.2=TXD

    // (4) Clear UCSWRST flag
    UCA0CTL1 &= ~UCSWRST;                                 // Initialize USCI state machine
}

void main(void)
{

	WDTCTL = WDTPW | WDTHOLD;// Stop watchdog timer
	// Initialize the ADC10
	// UART settings depend on an accurate 1 MHz clock.
	DCOCTL = 0;                               // Select lowest DCOx and MODx settings
	BCSCTL1 = CALBC1_16MHZ;
	DCOCTL = CALDCO_16MHZ;



	// Initialize the ADC10 using
	//  2.5V reference, 16 clocks, on, multiple samples
	ADC10CTL0 = SREF_1 | REF2_5V | REFON | ADC10SHT_2 | ADC10ON | MSC;

	// input channel 5, trigger on ADC10SC bit, no clock division,
	//  internal ADC clock, sequence of conversions counting down from A5
	ADC10CTL1 = INCH_5 | SHS_0 | ADC10DIV_0 | ADC10SSEL_0 | CONSEQ_1;

	// We want samples of A5, A4, A3, and A0 but you have to take all 6.
	ADC10DTC1 = 4;
	ADC10AE0 = BIT5 + BIT4 + BIT0;      // (all but A3 which we need for switch S2)
	ADC10CTL0 |= ENC + ADC10SC; // Sampling and conversion ready


	//---------------- UAR -----------------*/
	//DCOCTL = 0;
	P1SEL = BIT1 | BIT2 ;                     // P1.1 = RXD, P1.2=TXD
    P1SEL2 = BIT1 | BIT2 ;                    // P1.1 = RXD, P1.2=TXD

    // Initialize the UART uart_init();
	// Place   UCA0    in  Reset   to  be  configured
	UCA0CTL1   =   UCSWRST;
	// Configure
	UCA0CTL1 |=  UCSSEL_2;   //  SMCLK
	/*UCA0BR0    =  104;// 16*104;    //  1MHz    9600UBR00=0x82; UBR10=0x06; UMCTL0=0x5B; // uart0 16000000Hz 9599bps
	UCA0BR1    =   0;  //  1MHz    9600UBR01=0x82; UBR11=0x06; UMCTL1=0x5B; // uart1 16000000Hz 9599bps
	UCA0MCTL   =   UCBRS0; //  Modulation  UCBRSx  =   1*/


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

	//CONFIGURACION DEL TIMER (PWM)*/
    BCSCTL1 |= XT2OFF | DIVA_0;
    BCSCTL3 = XT2S_0 | LFXT1S_2 | XCAP_1;
    P1DIR |= BIT6;					// P1.6 set as output (Green LED)
    P1SEL |= BIT6;					// P1.6 selected Timer0_A Out1 output
    P2DIR |= BIT5 | BIT2;					// P1.6 set as output (Green LED)
    P2SEL |= BIT5 | BIT2;					// P1.6 selected Timer0_A Out1 output
    TA0CCTL1 = CM_0 | CCIS_0 | OUTMOD_7;
    TA1CCTL1 = CM_0 | CCIS_0 | OUTMOD_7;
    TA1CCTL2 = CM_0 | CCIS_0 | OUTMOD_7;
    //      TA0CCR0, Timer_A Capture/Compare Register 0
    TA0CCR0 = periodo; // Cargamos el periodo PWM
    TA1CCR0 = periodo; // Cargamos el periodo PWM
    ADC[1]=ADC[2]=ADC[3]=ADC[4]=periodo*0.5;
    // TA0CCR1, Timer_A Capture/Compare Register 1
    TA0CCR1 = periodo*0.8;
    TA1CCR1 = periodo*0.8;
    TA1CCR2 = periodo*0.8;
    TA0CTL = TASSEL_2 | ID_0 | MC_1;
    TA1CTL = TASSEL_2 | ID_0 | MC_1;


    while(1){
    	ADC10SA = (unsigned int)&ADC[0];           // destination address for DMA
        ADC10CTL0 |= ENC + ADC10SC;     // trigger the conversion
        while( ADC10CTL1 & ADC10BUSY ) ;
        //Motor 1
        dato1=(ADC[0]*0.0034799) * 1000;			//CONVERSIÓN A MILIVOLTS
		PWM1=periodo*(dato1/3200);//(ADC10MEM);		//TOMANDO EN CUENTA SI 1000 FUERA LA MAYOR INTENSIDAD DEL LED
		PWM1=(periodo*0.5-PWM1*0.5);
		PWM1=(PWM1<=0)?0:PWM1;
		if(sentidoMotor1==1){
			PWM1=periodo-PWM1;
		}
		//Motor 2
		dato2=(ADC[1]*0.0034799) * 1000;			//CONVERSIÓN A MILIVOLTS
		PWM2=periodo*(dato2/3200);//(ADC10MEM);		//TOMANDO EN CUENTA SI 1000 FUERA LA MAYOR INTENSIDAD DEL LED
		PWM2=(periodo*0.5-PWM2*0.5);
		PWM2=(PWM2<=0)?0:PWM2;
		if(sentidoMotor2==1){
			PWM2=periodo-PWM2;
		}
		TA0CCR1=PWM1;// PWM duty cycle,
		TA1CCR2=PWM2;
		//-----------TEST
		dato3=(ADC[2]*0.0034799) * 1000;			//CONVERSIÓN A MILIVOLTS
		PWM3=periodo*(dato3/3200);//(ADC10MEM);		//TOMANDO EN CUENTA SI 1000 FUERA LA MAYOR INTENSIDAD DEL LED
		PWM3=(periodo*0.5-PWM3*0.5);
		PWM3=(PWM3<=0)?0:PWM3;
		TA1CCR1 = PWM3;


		 __bis_SR_register(GIE);// LOW POWER MODE (PROCESADOR DURMIENDO)

    }
    //_BIS_SR(LPM0_bits+GIE);*/

 	__bis_SR_register(LPM0_bits    +   GIE);   //  Enter   LPM0,   interrupts  enabled
}

//  Echo    back    RXed    character,  confirm TX  buffer  is  ready   first
#pragma vector=USCIAB0RX_VECTOR
__interrupt void    USCI0RX_ISR(void)
{
	while  (!(IFG2&UCA0TXIFG));    //  USCI_A0 TX  buffer  ready?
	UCA0TXBUF  =   UCA0RXBUF;  //  TX  -&amp;gt;   RXed    character

	 	id_Motor=UCA0RXBUF;
	 	id_Motor=(id_Motor&192)>>6;
	 	/*if (id_Motor>=0&&id_Motor<=101){
	 		sentidoMotor2=!sentidoMotor2;
	 		//sentidoMotor2=1;
	 	}else*/ if(id_Motor==1){

	 		sentidoMotor1=1;
	 	}else if(id_Motor==0){

	 		sentidoMotor1=0;
	 	}else if(id_Motor==3){

	 		sentidoMotor2=1;
	 	}else if(id_Motor==2){

	 		sentidoMotor2=0;
	 	}
//*/



	 }













/*TABLA[11]= (Impresion%10)+0X30;
Impresion=Impresion/10;
TABLA[10]= (Impresion%10)+0X30;
Impresion=Impresion/10;
TABLA[9]= (Impresion%10)+0X30;
Impresion=Impresion/10;
TABLA[8]= (Impresion%10)+0X30;
//IMPRESION DE DATOS
for(i=0;i<15;i++)
{
while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
UCA0TXBUF = TABLA[i];                    // IMPRESIÓN DEL ARREGLO CON LOS DATOS CONVERTIDOS DEL ADC10MEM
}
if(x>711.2)//SI X(MILIVOLTS RECIBIDOS) ES MAYOR A 711.2 MILIVOLTS
{
	P1OUT = BIT0;
	if(x>1422.4)
	{
		P1OUT = BIT0+BIT3;
		if(x>2133.6)
		{
			P1OUT = BIT0+BIT3+BIT5;
			if(x>2844.8)
			{
				P1OUT = BIT0+BIT3+BIT5;
				P2OUT = BIT1;
				if(x>=3550)
				{
					P1OUT = BIT0+BIT3+BIT5+BIT7;
					P2OUT = BIT0+BIT1;
				}
				else
				{
					P2OUT &= ~BIT0;
				}
			}
			else
			{
				P2OUT &= ~BIT1;
			}
		}
		else
		{
			P1OUT &= ~BIT5;
		}
	}
	else
	{
		P1OUT &= ~BIT3;
	}
}
else
{
	P1OUT &= ~BIT0;
	P1OUT &= ~BIT3;
	P1OUT &= ~BIT5;
	P2OUT &= ~BIT1;
	P2OUT &= ~BIT0;
}
*/


