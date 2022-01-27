/* 
 	 Main program toggles red LED while waiting for interrupt from switches 
  	 which toogles green LED A1 and blue LED A2
 */

#include "MKL25Z4.h" 
void delayMs(int n); 

int main(void) 
{ 
	NVIC_ICPR |= 0x40000000;	// disable INT30 (bit 30 of ISER[0]) while configuring 
	
	SIM_SCGC5  |=  0x400; 		// enable clock to Port B
	SIM_SCGC5  |= 0x1000; 		//enable clock to Port D 
	
	PORTB_PCR18 = 0x100; 		// make PTB18 pin as GPIO red
	PORTB_PCR19 = 0x100; 		// make PTB19 pin as GPIO green 
	PORTD_PCR1  = 0x100; 		// make PTD1 pin as GPIO blue
	GPIOB_PDDR |= 0xC0000; 		// make PTB18, 19 as output pin  
	GPIOB_PDOR |= 0xC0000; 		// turn off LEDs 
	GPIOD_PDDR |= 0x02; 		// make PTD1 as output pin  
	GPIOD_PDOR |= 0x02; 		// turn off blue LED 
	 
	SIM_SCGC5 |= 0x200; 		// enable clock to Port A 
		
	/* configure PTA1 for interrupt */
	PORTA_PCR1 |=  0x00103; 	// make it GPIO and enable pull-up
	GPIOA_PDDR &= ~0x0002; 		// make pin input 
	PORTA_PCR1 &= ~0xF0000; 	// clear interrupt selection 
	PORTA_PCR1 |=  0xA0000;		// enable falling edge interrupt 
	
	/*configure PTA2 for interrupt*/
	PORTA_PCR2 |=  0x00103; 	// make it GPIO and enable pull-up 
	GPIOA_PDDR &= ~0x0004; 		// make pin input  
	PORTA_PCR2 &= ~0xF0000;		// clear interrupt selection  
	PORTA_PCR2 |=  0xA0000; 	// enable falling edge interrupt  
	
	NVIC_ISER |= 0x40000000; 	// enable INT30 (bit 30 of ISER[0])  
		
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
	while (PORTA_ISFR & 0x00000006) 
	{ 
		if (PORTA_ISFR & 0x00000002) //Port A1
		{ 
			/* toggle green LED (PTB19) three times */ 
			for (i = 0; i < 3; i++) 
			{ 
				GPIOB_PDOR &= ~0x80000; //turn on green LED  
				delayMs(500); 
				GPIOB_PDOR |= 0x80000; // turn off green LED 
				delayMs(500); 
			} PORTA_ISFR = 0x00000002; // clear interrupt flag  
		}
		if (PORTA_ISFR & 0x00000004) //Port A2
		{ 
			/* toggle blue LED (PTD1) three times */ 
			for (i = 0; i < 3; i++) 
			{ 
				GPIOD_PDOR &= ~0x02; // turn on blue LED  
				delayMs(500); 
				GPIOD_PDOR |= 0x02; // turn off blue LED  
				delayMs(500); 
			} PORTA_ISFR = 0x00000004; // clear interrupt flag 
		} 
	} 
}
	/* Delay n milliseconds */ 
void delayMs(int n) 
{ 
	int i; 
	int j; 
	for(i = 0 ; i < n; i++) 
	{
		for (j = 0; j < 7000; j++) {}
	}
}
	
	
