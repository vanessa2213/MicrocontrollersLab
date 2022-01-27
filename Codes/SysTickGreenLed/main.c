/*
 * main implementation: use this 'C' sample to create your own application
 *
 */


#include "MKL25Z4.h"

int main (void) {
	
	void delayMs(int n);
	SIM_SCGC5 |= 0x400; 	// enable clock to Port B 
	PORTB_PCR19 = 0x100; 	// make PTB19 pin as GPIO  
	GPIOB_PDDR |= 0x080000; // make PTB19 as output pin 
	while (1) 
	{ 
		delayMs(1000); 			// delay 1000 ms 
		GPIOB_PTOR = 0x080000; 	// toggle green LED 
	} 
}

void delayMs(int n) 
{ 
	int i; 
	SYST_RVR = 41940 - 1; 
	SYST_CSR = 0x5; 					// Enable the timer and choose sysclk as the clock source 
	for(i = 0; i < n; i++) 
	{ 
		while((SYST_CSR & 0x10000) == 0) // wait until the COUNT flag is set 
		{	
		} 
	} 
	SYST_CSR = 0; 	// Stop the timer (Enable = 0)  
}

