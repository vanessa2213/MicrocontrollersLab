
#include "derivative.h" /* include peripheral declarations */
#include "MKL25Z4.h"
void delayMs(int n) ;

int main (void) 
{ 
	int pulseWidth = 0; 
	
	SIM_SCGC5 |= 0x1000; 		// enable clock to Port D  
	PORTD_PCR1 = 0x0400; 		// PTD1 used by TPM0  
	SIM_SCGC6 |= 0x01000000; 	// enable clock to TPM0 
	SIM_SOPT2 |= 0x01000000; 	// use MCGFLLCLK as timer counter clock 
	TPM0_SC = 0; 				// disable timer 
	TPM0_C1SC = 0x20 | 0x08; 	// edge-aligned, pulse high  
	TPM0_MOD = 43702; 			// Set up modulo register for 60 kHz  
	TPM0_C1V = 14568; 			// Set up channel value for 33% dutycycle 
	TPM0_SC = 0x08; 			// enable TPM0 with no prescaler to be able of see the intensity of the blue LED
	while (1) 
	{
		pulseWidth += 437; 			//1% of MOD
		if (pulseWidth > 43702) 	// if it reaches to the maximum intensity then reset
			pulseWidth = 0; 
		TPM0_C1V = pulseWidth; 		// set duty cycle
		delayMs(20); 				//delay to see the changes
	} 
} 

/* Delay n milliseconds * The CPU core clock is set to MCGFLLCLK at 41.94 MHz in SystemInit(). */ 

void delayMs(int n) 
{ 
	int i; 
	int j; 
	for(i = 0 ; i < n; i++) 
		for (j = 0; j < 7000; j++) {}
}
