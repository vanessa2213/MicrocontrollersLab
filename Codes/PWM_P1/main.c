
#include "derivative.h" /* include peripheral declarations */
#include "MKL25Z4.h"

int main (void) 
{ 
	SIM_SCGC5 |= 0x1000; 		// enable clock to Port D  
	
	PORTD_PCR1 = 0x0400; 		// PTD1 used by TPM0  
	
	SIM_SCGC6 |= 0x01000000; 	// enable clock to TPM0 
	
	SIM_SOPT2 |= 0x01000000; 	// use MCGFLLCLK as timer counter clock  
	TPM0_SC = 0; 				// disable timer  
	TPM0_C1SC = 0x20 | 0x08; 	// edge-aligned, pulse high 
	TPM0_MOD = 43702; 			// Set up modulo register for 60 kHz  
	
	//TPM0_C1V = (%*MOD/100)
	
	//TPM0_C1V = (0*43702/100); 	// Set up channel value for   0% dutycycle turn on blue LED 
	//TPM0_C1V = (25*43702/100); 	// Set up channel value for  25% dutycycle
	//TPM0_C1V = (50*43702/100); 	// Set up channel value for  50% dutycycle
	TPM0_C1V = (75*43702/100); 	// Set up channel value for  75% dutycycle
	//TPM0_C1V = (100*43702/100); 	// Set up channel value for 100% dutycycle turn off blue LED
	
	
	TPM0_SC = 0x0F; 				// enable TPM0 with prescaler /64 in order to be see the LED  
	
	while (1) { } 
}
