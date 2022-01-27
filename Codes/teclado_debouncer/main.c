/*
 * main implementation: use this 'C' sample to create your own application
 *
 */




#include "MKL25z4.h"
#include "derivative.h"

void delayMs(int n);
void delayUs(int n);

void keypad_init(void);
char keypad_getkey(void);

void led_init(void);
void led_set(int value);

int read_key(int x);



int main(void)
{
	unsigned char key;
	int rkey;
	keypad_init();
	led_init();

	
	while(1)
	{
		key = keypad_getkey(); 		//get which key was pressed
		rkey  = read_key(key); 		//read if the key was pressed
		
		if(rkey == 1){ 				//if it was pressed then
			led_set(key);			//turn on the LED depending on the key
			
			while (rkey == 1){				//if still pressed stay here till is not
				key = keypad_getkey();		//get the key pressed
				rkey  = read_key(key);		//read if the key was actually pressed
			}
		}
		led_set(key);						//just to turn off the LEDs after there is no pressing
	}
}

void keypad_init(void){
	
	SIM_SCGC5 |= 0x0800;	//enable clk to Port C
	
	PORTC_PCR0 = 0x103;		//PTC0 as GPIO and enable pull-up resistor
	PORTC_PCR1 = 0x103;		//PTC1 as GPIO and enable pull-up resistor
	PORTC_PCR2 = 0x103;		//PTC2 as GPIO and enable pull-up resistor
	PORTC_PCR3 = 0x103;		//PTC3 as GPIO and enable pull-up resistor
	PORTC_PCR4 = 0x103;		//PTC4 as GPIO and enable pull-up resistor
	PORTC_PCR5 = 0x103;		//PTC5 as GPIO and enable pull-up resistor
	PORTC_PCR6 = 0x103;		//PTC6 as GPIO and enable pull-up resistor
	PORTC_PCR7 = 0x103;		//PTC7 as GPIO and enable pull-up resistor
	
	GPIOD_PDDR = 0x0F; 		//make PTC7-0 as input pins
}

char keypad_getkey(void){
	
	int col, row;
	const char row_select[] = {0x01, 0x02, 0x04, 0x08};
	
	GPIOC_PDDR = 0x0F;			//enable all rows
	GPIOC_PCOR = 0x0F;
	delayUs(2);					//wait for a signal
	
	col = 0xF00 & GPIOC_PDIR;	//read all columns
	
	GPIOC_PDDR = 0; 			//disable ll rows
	
	if (col == 0xF0)			//no key was pressed
	return 0;
	
	for (row = 0; row<4; row++)				//find out which key was pressed
	{
		GPIOC_PDDR = 0; 					//disable all  rows
		GPIOC_PDDR |= row_select[row];		//enable one row
		GPIOC_PCOR = row_select[row];		//drive the active row low
		delayUs(2);							//read all columns
		col = GPIOC_PDIR & 0xF0;			//if one of the inputs is low some key is pressed
		if (col != 0xF0) break;
	}
	
	GPIOC_PDDR = 0;	//disable all rows
	
	if (row == 4)	//no key was pressed
	return 0;
	
	//return which key was pressed
	if (col == 0xE0) return row * 4 + 1;	//return the position in column 1
	if (col == 0xD0) return row * 4 + 2;	//return the position in column 2
	if (col == 0xB0) return row * 4 + 3;	//return the position in column 3
	if (col == 0x70) return row * 4 + 4;	//return the position in column 4
	
	return 0;	//just to be sure of any miss information
}

void led_init(void){

	SIM_SCGC5 |= 0x400;		//enable clk to Port B
	SIM_SCGC5 |= 0x1000;	//enable clk to Port D
	
	//RED LED
	PORTB_PCR18 = 0x103;	//make PTB18 as GPIO
	GPIOB_PDDR |= 0x40000;	//make PTB18 as output pin
	GPIOB_PSOR |= 0x40000;	//turn of red LED
	
	//GREEN LED
	PORTB_PCR19 = 0x103;	//make PTB19 as GPIO
	GPIOB_PDDR |= 0x80000;	//make PTB19 as output pin
	GPIOB_PSOR |= 0x80000;	//turn off green LED
	
	//BLUE LED
	PORTD_PCR1 = 0x103;		//make PD1 as GPIO
	GPIOD_PDDR |= 0x02;		//make PD1 as output pin
	GPIOD_PSOR |= 0x02;		//turn off blue LED
}

void led_set(int value)
{
	//RED LED 1st column
	if (value == 1 || value == 5|| value == 9|| value == 13|| value == 3|| value == 7|| value == 11|| value == 15)
	GPIOB_PCOR |= 0x40000;
	else
	GPIOB_PSOR |= 0x40000;
	
	//GREEN LED 2nd column
	if (value == 2|| value == 6|| value == 10|| value == 14)
	GPIOB_PCOR |= 0x80000;
	else
	GPIOB_PSOR |= 0x80000;
	
	//JUST FOR FUN ADDING COLOR PURPLE ON VALUES 3 7 11 15, THATS WHY RED AND BLUE TURN ON AT THAT POSITION
	
	//BLUE LED 4th column
	if (value == 4|| value == 8|| value == 12|| value == 16 || value == 3|| value == 7|| value == 11|| value == 15)
	GPIOD_PCOR |= 0x02;
	else
	GPIOD_PSOR |= 0x02;
}

int read_key(int x){
	int key_press;
	
	if(x!=0) 				//if a key was pressed return 1
	return key_press = 1;
	else					//if there is no key pressed return 0
	return key_press = 0;
	
	
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

