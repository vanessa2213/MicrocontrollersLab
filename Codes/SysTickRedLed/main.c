/*          
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include <MKL25Z4.H> 
int main (void) 
{ 
	SIM_SCGC5 |= 0x0400; 	// enable clock to Port B  
	PORTB_PCR18 = 0x100; 	// make PTB18 pin as GPIO 
	GPIOB_PDDR |= 0x040000; // make PTB18 as output pin 
	
	// Configuring SysTick 
	SYST_RVR = 8388000 - 1; // SysTick Reload Value Register: reload with number of clocks per 200 ms 
	SYST_CSR = 5;			// SysTick Control and Status Register: enable it, no interrupt, use system clock 
	while (1) 
	{ 
		if (SYST_CSR & 0x10000) // if COUNT flag is set  
		GPIOB_PTOR = 0x040000; 	// toggle red LED 
	} 

}
 
