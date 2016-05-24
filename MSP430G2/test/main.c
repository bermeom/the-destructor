#include <msp430.h> 

#define POSITION_CHAR 'P'
#define RATE_CHAR 'R'
#define SERVOS 6

unsigned char buffer[6];
unsigned char buffer_index;
unsigned char rate[SERVOS];
unsigned short servo[SERVOS];
unsigned short input[SERVOS];

/*http://homepages.ius.edu/RWISMAN/C335/HTML/msp430Timer.HTM
 * main.c
 */
float periodo =1000-1;
float periodo1 =350;
float periodo2 =250;

void initPWM(){

    P1DIR |= BIT6|BIT2|BIT5 ; // P2.1 y P2.4 como salida
    P2DIR |= BIT0  | BIT3  | BIT1  | BIT2| BIT4| BIT5| BIT6  ; // P2.1 y P2.4 como salida
    P2SEL &= ~BIT7;                              //Reset Xout select1
    P2SEL |= BIT0  | BIT3  |BIT1  | BIT2| BIT4 | BIT5; // Asociado al Timer_A1
    P2SEL |=  BIT6|BIT2|BIT5;
    P1SEL |= BIT6|BIT2|BIT5 ; // Asociado al Timer_A1


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


int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    initPWM();
    _BIS_SR( LPM0_bits + GIE); // Bajo consumo LPM0
	return 0;
}



