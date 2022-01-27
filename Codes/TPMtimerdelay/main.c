/*
 * main implementation: use this 'C' sample to create your own application
 *
 */
#include "derivative.h" /* include peripheral declarations */

#include "MKL25Z4.h"

int main (void) 
{ 
	int i; 
	SIM_SCGC5 |= 0x1000; 	// enable clock to Port D  
	PORTD_PCR1 = 0x100; 	// make PTD1 pin as GPIO  
	GPIOD_PDDR |= 0x02; 	// make PTD1 as output pin 
	GPIOD_PSOR = 0x02;

	SIM_SCGC6 |= 0x01000000; 	// enable clock to TPM0 
	SIM_SOPT2 |= 0x01000000; 	// use MCGFLLCLK as CNT clock  
	TPM0_SC = 0; 				// disable timer while configuring 
	TPM0_MOD = 0xFFFF; 			// max modulo value 
	TPM0_SC |= 0x80; 			// clear TOF 
	TPM0_SC |= 0x08; 			// enable timer free-running mode 

	while (1) {
		for(i = 0; i < 320; i++) { 			// repeat timeout for 320 times 
			while((TPM0_SC & 0x80) == 0) {}	// wait until the TOF is set 
			TPM0_SC |= 0x80; 				// clear TOF 
		}
		GPIOD_PTOR = 0x02; 					// toggle blue LED 
								
	}
}



