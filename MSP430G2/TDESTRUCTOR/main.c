
#include <msp430g2553.h>
/*
unsigned int werte[2]; //for holding the conversion results
unsigned int i;
float periodo =1000-1;
float periodo1 =350;
float periodo2 =250;


void initPWM(){

    P1DIR |= BIT6|BIT5 ; // P2.1 y P2.4 como salida
    P2DIR |= BIT0  | BIT3  | BIT1  | BIT2| BIT4| BIT5| BIT6  ; // P2.1 y P2.4 como salida
    P2SEL &= ~BIT7;                              //Reset Xout select1
    P2SEL |= BIT0  | BIT3  |BIT1  | BIT2| BIT4 | BIT5; // Asociado al Timer_A1
    //P2SEL |=  BIT6|BIT2|BIT5;
    P1SEL |= BIT6|BIT5 ; // Asociado al Timer_A1


    TACCR0 =periodo ; // PWM Period 50us (substract 1 because it's 0-based)
    TACCR1 = periodo*0.8; // CCR1 PWM duty cy
    TACCR2 = periodo*0.4; // CCR1 PWM duty cy


    TA0CCR0 = periodo2;                 // Count limit (16 bit)
	TA0CCR1 =periodo2*0.7;                 // Enable Timer A0 interrupts, bit 4=1
	TA0CCR2 =periodo2*0.3;                 // Enable Timer A0 interrupts, bit 4=1


    TA1CCR0 = periodo1; // Cargamos el periodo PWM
    TA1CCR1 = periodo1*0.3; // PWM duty cycle, 10% CCR1/(CCR0+1) * 100
    TA1CCR2 = periodo1*0.4; // PWM duty cycle, 50% CCR2/(CCR0+1) * 100


//	// Asociado al Timer_A1


//	TACCR0 = 60;                                       //Duty cycle

    TACCTL0 = OUTMOD_7;
	TACCTL1 = OUTMOD_7; //Modo7 reset/set
	TACCTL2 = OUTMOD_7;                //Output mode reset/set
	TA0CCTL0 = OUTMOD_4; //Modo7 reset/set
	TA0CCTL1 = OUTMOD_7; //Modo7 reset/set
	TA0CCTL2 = OUTMOD_7; //Modo7 reset/set
    TA1CCTL0 = OUTMOD_4; //Modo7 reset/set
    TA1CCTL1 = OUTMOD_7; //Modo7 reset/set
    TA1CCTL2 = OUTMOD_7; //Modo7 reset/set

	TACTL   = TASSEL_2 + MC_1;        // SMCLK, upmode
    TA0CTL	= TASSEL_2 + MC_1; // Timer SMCLK Modo UP
    TA1CTL	= TASSEL_2 + MC_1; // Timer SMCLK Modo UP




}


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer

    BCSCTL1 = CALBC1_1MHZ; // Set range
    DCOCTL = CALDCO_1MHZ;
    BCSCTL2 &= ~(DIVS_3); // SMCLK = DCO = 1MHz

    P1DIR |= BIT0; // Rote LED P1.0 als Ausgang
    P1DIR |= BIT6; // Grüne LED P1.6 als Ausgang
    P2DIR |= BIT3; // P2.3 als Ausgang
    P2DIR |= BIT1; // P2.4 als Ausgang
    P2DIR |= BIT2; // P2.5 als Ausgang
    P2DIR |= BIT5; // P2.5 als Ausgang
    //initPWM();

    ADC10CTL1 =INCH_3 + ADC10DIV_7 + ADC10SSEL_3 + CONSEQ_3;// A3/A2/A1/A0, once multi channel
    ADC10CTL0 = ADC10SHT_3 + MSC + ADC10ON + ADC10IE;//2.5 reference voltage
    ADC10AE0 |= BIT0  + BIT3; // ADC10 Analog enable
    ADC10DTC1 = 0x02;                          // 3 conversions
    ADC10CTL0 |= ENC + ADC10SC; // Sampling and conversion ready
	ADC10CTL0 &= ~ENC; // Disable sampling
	while (ADC10CTL1 & BUSY); // Wait if ADC10 core is active
	ADC10SA = (unsigned int)&werte[0];
	ADC10CTL0 |= ENC + ADC10SC; // Sampling and conversion ready;

    TA1CCTL0 = OUTMOD_7 + CCIE;// + MC_1; // CCR0 toggle, interrupt enabled
    TA1CCTL1 = OUTMOD_7 + CCIE;// + MC_1; // CCR1 toggle, interrupt enabled
    TA1CCTL2 = OUTMOD_7 + CCIE;// + MC_1; // CCR2 toggle, interrupt enabled
    TA1CTL = TASSEL_2 + MC_2 + ID_3 + TACLR + TAIE; // SMCLK, Contmode, clear TAR, interrupt enabled

    __bis_SR_register(LPM0_bits + GIE); // LPM0 aktiv, interrupts enabled
}



// Timer1_A0 interrupt service routine
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0 (void)
{
//P2OUT ^= BIT6;                // Toggle P2.3
//TA1CCR0 += ((62500)/((werte[2]/4)+400)); // Berechnung Frequenz in ADC Wert 400 - 650Hz
}



// Timer1_A1 Interrupt Vector (TAIV) handler*//*
 *
 *
#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer1_A1_ISR(void)
{
switch(__even_in_range(TA1IV,4))
{
case 0:  break;
case 2:
		//P1OUT ^= BIT6;                // Toggle Grüne LED
		//P2OUT ^= BIT1;                // Toggle P2.1
		//TA1CCR1 += ((62500)/(werte[1]/19));// Berechnung Frequenz in ADC Wert 0 - 50Hz
		//TA1CCR1 =(werte[1]);// Berechnung Frequenz in ADC Wert 0 - 50Hz
		TA0CCR1 =periodo2*0.2;                 // Enable Timer A0 interrupts, bit 4=1

	break;
case 4:
    	//TA1CCR2 = periodo1*0.6; // PWM duty cycle, 50% CCR2/(CCR0+1) * 100
		//TA0CCR2 =periodo2*0.7;                 // Enable Timer A0 interrupts, bit 4=1

		P2OUT ^= BIT5; // Toggle Rote LED
		P2OUT ^= BIT2;                   // Toggle P2.5
		TA1CCR2 += ((62500/3)/(werte[0]/19));// Berechnung Frequenz in ADC Wert 0 - 50Hz
		break;
default: break;
}
}



#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
ADC10CTL0 &= ~ENC; // Disable sampling
while (ADC10CTL1 & BUSY); // Wait if ADC10 core is active
ADC10SA = (unsigned int)&werte[0];
ADC10CTL0 |= ENC + ADC10SC; // Sampling and conversion ready;
//__bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}*/

