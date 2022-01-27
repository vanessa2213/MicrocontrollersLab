/* p6_1: PORTA interrupt from a switch 
 
 	   Upon pressing a switch(button) connecting either PTA1 or PTA2  ground,
   	   the green LED will toggle for three times. 
   	   Main program toggles red LED while waiting for interrupt from switches. 
   	   
*/


#include "derivative.h" 

void PORTA_IRQHandler(void);
void delayMs(int n); 

int main(void) {
	 
	NVIC_ICPR |= 0x40000000;	// disable INT30 (bit 30 of ISER[0]) while configuring 
	
	SIM_SCGC5  |=  0x400; 		// enable clock to Port B
	PORTB_PCR18 = 0x100; 		// make PTB18 pin as GPIO red
	PORTB_PCR19 = 0x100; 		// make PTB19 pin as GPIO green
	
	
	GPIOB_PDDR |= 0xC0000; 		// make PTB18, 19 as output pin 
	GPIOB_PDOR |= 0xC0000; 		// turn off LEDs 
	
	SIM_SCGC5 |= 0x200; 		// enable clock to Port A 
	
	/* configure PTA1 for interrupt */
	PORTA_PCR1 |=  0x00103; 	// make it GPIO and enable pull-up
	GPIOA_PDDR &= ~0x0002; 		// make pin input 
	PORTA_PCR1 &= ~0xF0000; 	// clear interrupt selection 
	PORTA_PCR1 |=  0x90000;		// enable falling edge interrupt 
	
	/*configure PTA2 for interrupt*/
	PORTA_PCR2 |=  0x00103; 	// make it GPIO and enable pull-up 
	GPIOA_PDDR &= ~0x0004; 		// make pin input  
	PORTA_PCR2 &= ~0xF0000;		// clear interrupt selection  
	PORTA_PCR2 |=  0x90000; 	// enable falling edge interrupt  
	

	NVIC_ISER |= 0x40000000;  
	
	/* toggle the red LED continuously */ 
	while(1) 
	{ 
		GPIOB_PTOR |= 0x40000; 	// toggle red LED 
		delayMs(500); 
	} 
}

void PORTA_IRQHandler(void) 
{ 
	int i; 
	/* toggle green LED (PTB19) three times */ 
	for (i = 0; i < 3; i++) 
	{ 
		GPIOB_PDOR &= ~0x80000; 	//turn on green LED  
		delayMs(500); 
		GPIOB_PDOR |= 0x80000; 		// turn off green LED  
		delayMs(500); 
	} 
	PORTA_ISFR = 0x00000006; 		// clear interrupt flag 
}

void delayMs(int n) {
	int i;
	int j;
	for(i = 0 ; i < n; i++){
		for(j = 0 ; j < 7000; j++) { }
	}
}
