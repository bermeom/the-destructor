/*
 * ======== Standard MSP430 includes ========
 */
#include <msp430.h>

/*
 * ======== Grace related includes ========
 */
#include <ti/mcu/msp430/Grace.h>

/*
 *  ======== main ========
 */
int main(void)
{
	int i,j;
    Grace_init();// Activate Grace-generated configuration
    
    // >>>>> Fill-in user code here <<<<<
    
    for(;;){

    	for(i=0;i<=20000;i++){
    		for(j=0;j<=100;j++){


    		}
    	}

    P1OUT^=0x40;

    }

    return (0);
}
