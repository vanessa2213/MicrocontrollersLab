/*
 * main implementation: use this 'C' sample to create your own application
 *
 */


#include "MKL25z4.h"
#include "derivative.h" /* include peripheral declarations */

void delayMs(int n);
void delayUs(int n);

void keypad_init(void);
char keypad_getkey(void);

void led_init(void);
void led_set(int value);




int main(void)
{
	unsigned char key;
	keypad_init();
	led_init();

	
	while(1)
	{
		key = keypad_getkey(); 	//identify the key pressed
		led_set(key); 			//turning on the LEDs according to the key code
	}
}

void keypad_init(void){
	SIM_SCGC5 |= 0x0800;    //enable clk to port c
	
	PORTC_PCR0 = 0x103;    //PTC0 as GPIO and enable pullup
	PORTC_PCR1 = 0x103;    //PTC1 as GPIO and enable pullup
	PORTC_PCR2 = 0x103;    //PTC2 as GPIO and enable pullup
	PORTC_PCR3 = 0x103;    //PTC3 as GPIO and enable pullup
	PORTC_PCR4 = 0x103;    //PTC4 as GPIO and enable pullup
	PORTC_PCR5 = 0x103;    //PTC5 as GPIO and enable pullup
	PORTC_PCR6 = 0x103;    //PTC6 as GPIO and enable pullup
	PORTC_PCR7 = 0x103;    //PTC7 as GPIO and enable pullup
	GPIOC_PDDR = 0x0F;     //make PTC7-0 as input pins
}

char keypad_getkey(void){
	
	int col, row;
	const char row_select[] = {0x01, 0x02, 0x04, 0x08};
	
	GPIOC_PDDR = 0x0F; 	//enable al rows
	GPIOC_PCOR = 0x0F;
	delayUs(2);  	   	//wait for signal
	
	col = 0xF00 & GPIOC_PDIR;	//read all columns
	
	GPIOC_PDDR = 0; 			//disable all rows
	
	if (col == 0xF0)			//no key pressed;
	return 0;
	
	for (row = 0; row<4; row++) //finds out which key was pressed
	{
		GPIOC_PDDR = 0; 				//disable all rows
		GPIOC_PDDR |= row_select[row];	//enable one row
		GPIOC_PCOR = row_select[row];	//drive the active row low
		delayUs(2);						//wait for signal to settle
		col = GPIOC_PDIR & 0xF0;		//read all columns
		if (col != 0xF0) break;			//if one of the inputs is low some key is pressed
	}
	
	GPIOC_PDDR = 0;	//disable all rows
	
	if (row == 4)	//no key was pressed
	return 0;
	
	if (col == 0xE0) return row * 4 + 1; //key in column 1
	if (col == 0xD0) return row * 4 + 2; //key in column 2
	if (col == 0xB0) return row * 4 + 3; //key in column 3
	if (col == 0x70) return row * 4 + 4; //key in column 4
	return 0;							 //other information received
}

void led_init(void){

	SIM_SCGC5 |= 0x2000;	//enable clk to port E
	
	//RED LED
	
	PORTE_PCR3 = 0x100;		//make PTE3 as GPIO
	GPIOE_PDDR |= 0x08;		//make PTE3 as output pin
	GPIOE_PCOR |= 0x08;		//turn off red LED
	
	//GREEN LED
	
	PORTE_PCR4 = 0x100;		//make PTE4 as GPIO
	GPIOE_PDDR |= 0x10;		//make PTE4 as output pin
	GPIOE_PCOR |= 0x10;	
	
	//BLUE LED
	
	PORTE_PCR5 = 0x100;		//make PTE5 as GPIO
	GPIOE_PDDR |= 0x20;		//make PTE5 as output pin
	GPIOE_PCOR |= 0x20;		//turn off red LED
}

void led_set(int value)
{
	//RED LED
	if (value == 4)
	{
		GPIOE_PSOR |= 0x08;
		
	}
	else{
		GPIOE_PCOR |= 0x10;	
		GPIOE_PCOR |= 0x20;	
	}
	
	
	//GREEN LED
	if (value == 3)
	{
		GPIOE_PSOR |= 0x10;
	}
	else{
		GPIOE_PCOR |= 0x20;	
		GPIOE_PCOR |= 0x08;
	}
	
	//JUST FOR FUN ADDING COLOR PURPLE ON VALUES 3 7 11 15, THATS WHY RED AND BLUE TURN ON AT THAT POSITION
	
	//BLUE LED
	if (value == 2)
	{
		GPIOE_PSOR |= 0x20;
	}
	
	else{
		GPIOE_PCOR |= 0x10;	
		GPIOE_PCOR |= 0x08;
	}

}


void delayUs(int n){
	int i,j;
	for(i = 0 ; i < n; i++) {
		for(j = 0; j < 5; j++) ;
	}
}

void delayMs(int n) {
	int i;
	int j;
	for(i = 0 ; i < n; i++){
		for(j = 0 ; j < 7000; j++) { }
	}
}
