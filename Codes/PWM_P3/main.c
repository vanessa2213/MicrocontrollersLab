/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */
#include "MKL25Z4.h" 

#define RS 1    // BIT0 mask 
#define RW 2    // BIT1 mask 
#define EN 4    // BIT2 mask 

void LCD_nibble_write(unsigned char data, unsigned char control);
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_init(void);

void delayUs(int n);
void delayMs(int n);

void keypad_init(void);
char keypad_getkey(void);
int read_key(int x);
int getNum(int key);

void writeMenu(void);
void writeManual(void);
void writeAuto(void);

void TPM0init(void);
void setDutyCycle(int n);

void led_init(void);
void ADC0_init(void);

int main(void)
{
	int m;
	unsigned char key;
	int rkey;
	int num;
	short int result;
	
	LCD_init();
	keypad_init();
	led_init();
	TPM0init();
	ADC0_init();
	while(1)
	{
		setDutyCycle(0);
		GPIOE_PCOR |= 0x01;					//turn off blue led
		GPIOE_PCOR |= 0x02;					//turn off green LED
		
		writeMenu();
		m = 1 ;
		while(m == 1)
		{
			key = keypad_getkey(); 				//get which key was pressed
			rkey  = read_key(key); 				//read if the key was pressed
			num = getNum(key);
			
			if(rkey == 1)						//if it was pressed then
			{ 
				if(key != 13 && key!=14 && key!=16 && key!= 0 && key!= 1 && key!= 9 && key!= 5) 
					
				//si key no es igual a null,*,#,D,C,B,A
				{
					if(num == 1) //Manual mode
					{
						GPIOE_PSOR |= 0x01;						//turn on blue led
						writeManual();							//write mode manual
						while (rkey == 1)						//if still pressed stay here till is not as debouncer
						{				
							key = keypad_getkey();				//get the key pressed
							rkey  = read_key(key);				//read if the key is still pressed
						}
						while(key != 16){
								
							key = keypad_getkey(); 				//get which key was pressed
							num = getNum(key);					//get num pressed
							
							if (num == 1)
							{
								setDutyCycle(25);				//25% duty cycle
								LCD_command(0x8E);      		//cursor 1st line position 15
								LCD_data('1');					//MODE 1
								LCD_command(0x90);
								while (rkey == 1)				//debouncer
								{				
									key = keypad_getkey();		//get the key pressed
									rkey  = read_key(key);		//read if the key is still pressed
								}
							}
							
							else if (num == 2)
							{
								setDutyCycle(50);				//50% duty cycle
								LCD_command(0x8E);      		//cursor 1st line position 15
								LCD_data('2');					//MODE 2
								LCD_command(0x90);
								while (rkey == 1)				//if still pressed stay here till is not as debouncer
								{				
									key = keypad_getkey();		//get the key pressed
									rkey  = read_key(key);		//read if the key is still pressed
								}
							}
							else if (num == 3)
							{
								setDutyCycle(75);				//75% duty cycle
								LCD_command(0x8E);      		//cursor 1st line position 15
								LCD_data('3');					//MODE 3
								LCD_command(0x90);
								while (rkey == 1)				//if still pressed stay here till is not as debouncer
								{				
									key = keypad_getkey();		//get the key pressed
									rkey  = read_key(key);		//read if the key is still pressed
								}
							}
							else if (num == 4)
							{
								setDutyCycle(100);				//75% duty cycle
								LCD_command(0x8E);      		//cursor 1st line position 15
								LCD_data('4');					//MODE 4
								LCD_command(0x90);
								while (rkey == 1)				//if still pressed stay here till is not as debouncer
								{				
									key = keypad_getkey();		//get the key pressed
									rkey  = read_key(key);		//read if the key is still pressed
								}
							}
						}
					}
					else if(num == 2) 			//Automatic mode
					{
						
						writeAuto();			//write mode auto
						GPIOE_PSOR |= 0x02;		//turn on green LED 
						while (rkey == 1)		//debouncer
						{				
							key = keypad_getkey();		//get the key pressed
							rkey  = read_key(key);		//read if the key is still pressed
						}
						while(key!=16)	//while is not *
						{
							key = keypad_getkey(); 				//get which key was pressed
							ADC0_SC1A = 0; 						//start conversion on channel 0 where LM45 is connected
							while(!(ADC0_SC1A & 0x80)) { } 		// wait for conversion complete  
							result = ADC0_RA;
							if(result >= 1 && result < 1025)    //Between 0 and 0.75 V
							{
								setDutyCycle(25);		//25% duty cycle
								LCD_command(0xC5);      //cursor 1st line position 6
								LCD_data('1');			//MODE 1
								LCD_command(0x90);		//bye bye cursor
							}
							else if(result >= 1025 && result <= 2048)	//Between 0.75 and 1.5 V
							{
								setDutyCycle(50);		//50% duty cycle
								LCD_command(0xC5);      //cursor 1st line position 6
								LCD_data('2');			//MODE 2
								LCD_command(0x90);		//bye bye cursor
							}
							else if(result > 2048 && result < 3071)		//Between 1.5 and 2.25 V
							{
								setDutyCycle(75);		//75% duty cycle
								LCD_command(0xC5);      //cursor 1st line position 6
								LCD_data('3');			//MODE 3
								LCD_command(0x90);		//bye bye cursor						
							}
							else if(result >=3071 && result <= 4095)	//Between 2.25 and 3 V
							{
								setDutyCycle(100);		//100% duty cycle
								LCD_command(0xC5);      //cursor 1st line position 6
								LCD_data('4');			//MODE 4
								LCD_command(0x90);		//bye bye cursor				
							}
						}
						
					}
				}
				else if(key == 16) 						//if * was pressed reset
				{
					m = 2;
				}
			}
		}	
	}
}

void led_init(void){

	SIM_SCGC5 |= 0x2000;	//enable clk to port E
	SIM_SCGC5 |= 0x0200;	//enable clk to port A
		
	//DC fan
	PORTA_PCR4 = 0x0300;		// PTA4 used by TPM0 
		
	//BLUE LED
	PORTE_PCR0 = 0x100;		//make PTE0 as GPIO
	GPIOE_PDDR |= 0x01;		//make PTE0 as output pin
	GPIOE_PCOR |= 0x01;		//turn off blue LED 
	//GREEN LED
	PORTE_PCR1 = 0x100;		//make PTE1 as GPIO
	GPIOE_PDDR |= 0x02;		//make PTE1 as output pin
	GPIOE_PCOR |= 0x02;		//turn off green LED 
}


void TPM0init(void){
		
	SIM_SCGC6 |= 0x01000000; 	// enable clock to TPM0 
	
	SIM_SOPT2 |= 0x01000000; 	// use MCGFLLCLK as timer counter clock  
	TPM0_SC = 0; 				// disable timer  
	TPM0_C1SC = 0x20 | 0x08; 	// edge-aligned, pulse high 
	TPM0_MOD = 43687; 			// Set up modulo register for 50 Hz  
	TPM0_SC = 0x0C;				// Set up PS /16
}

void setDutyCycle(int n){
	TPM0_C1V = (43687+1)*n/100;		//Set up V
	 
}

void writeManual(void) 		// write mode :IDLE
{
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the begging first line
	LCD_data('S');
	LCD_data('E');
	LCD_data('T');
	LCD_data(' ');
	LCD_data('S');
	LCD_data('P');
	LCD_data('E');
	LCD_data('E');
	LCD_data('D');
	LCD_command(0xC0);	  	//set cursor at the begging second line
	LCD_data('1');
	LCD_data(':');
	LCD_data('L');
	LCD_data(' ');
	LCD_data('2');
	LCD_data(':');
	LCD_data('M');
	LCD_data(' ');
	LCD_data('3');
	LCD_data(':');
	LCD_data('M');
	LCD_data('H');
	LCD_data(' ');
	LCD_data('4');
	LCD_data(':');
	LCD_data('H');
	LCD_command(0x90);		//bye bye cursor
}

void writeAuto(void) 		// write mode :IDLE
{
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the begging first line
	LCD_data('A');
	LCD_data('U');
	LCD_data('T');
	LCD_data('O');
	LCD_data('M');
	LCD_data('A');
	LCD_data('T');
	LCD_data('I');
	LCD_data('C');
	LCD_command(0xC0);	  	//set cursor at the begging secon line
	LCD_data('M');
	LCD_data('O');
	LCD_data('D');
	LCD_data('E');
	LCD_command(0x90);		//bye bye cursor
	
}

void writeMenu(void) 		// write mode menu
{
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the begging first line
	LCD_data('S');
	LCD_data('E');
	LCD_data('T');
	LCD_data(' ');
	LCD_data('M');
	LCD_data('O');
	LCD_data('D');
	LCD_data('E');
	LCD_command(0xC0);	  	//set cursor at the begging first line
	LCD_data('1');
	LCD_data(':');
	LCD_data('M');
	LCD_data('a');
	LCD_data('n');
	LCD_data(' ');
	LCD_data('2');
	LCD_data(':');
	LCD_data('A');
	LCD_data('u');
	LCD_data('t');
	LCD_data('o');
	LCD_command(0x90);
}

void keypad_init(void)
{
	SIM_SCGC5 |= 0x0800;   //enable clk to port c
	
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

void ADC0_init(void) 
{
	SIM_SCGC5 |= 0x2000; 				// clock to PORTE 
	PORTE_PCR20 = 0; 					// PTE20 analog input 
	SIM_SCGC6 |= 0x8000000; 			// clock to ADC0 
	ADC0_SC2 &= ~0x40; 					// software trigger 
	/* clock div by 4, long sample time, single ended 12 bit, bus clock */
	ADC0_CFG1 = 0x40 | 0x10 | 0x04 | 0x00; 
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
	
	for (row = 0; row<4; row++) 		//finds out which key was pressed
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

int read_key(int x)
{
	int key_press;
	
	if(x!=0) 				//if a key was pressed return 1
	return key_press = 1;
	else					//if there is no key pressed return 0
	return key_press = 0;
	
}

int getNum(int value) //identify which number was pressed from the keyboard
{
	if(value == 15)
		return 0;
	else if(value == 4)
		return 1;
	else if(value == 3)
		return 2;
	else if(value == 2)
		return 3;
	else if(value == 8)
		return 4;
	else if(value == 7)
		return 5;
	else if(value == 6)
		return 6;
	else if(value == 12)
		return 7;
	else if(value == 11)
		return 8;
	else if(value == 10)
		return 9;
}
void LCD_init(void)
{
	SIM_SCGC5 |= 0x1000;     // enable clock to Port D 
	PORTD_PCR0 = 0x100;      // make PTD0 pin as GPIO 
	PORTD_PCR1 = 0x100;      // make PTD1 pin as GPIO 
	PORTD_PCR2 = 0x100;      // make PTD2 pin as GPIO 
	PORTD_PCR3 = 0x100;      // make PTD3 pin as GPIO
	PORTD_PCR4 = 0x100;      // make PTD4 pin as GPIO 
	PORTD_PCR5 = 0x100;      // make PTD5 pin as GPIO 
	PORTD_PCR6 = 0x100;      // make PTD6 pin as GPIO 
	PORTD_PCR7 = 0x100;      // make PTD7 pin as GPIO 
	
	GPIOD_PDDR |= 0xF7;         // make PTD7-4, 2, 1, 0 as output pins 
	delayMs(30);                // initialization sequence 
	LCD_nibble_write(0x30, 0);
	delayMs(10);
	LCD_nibble_write(0x30, 0);
	delayMs(1);
	LCD_nibble_write(0x30, 0);
	delayMs(1);
	LCD_nibble_write(0x20, 0);  // use 4-bit data mode 
	delayMs(1);
	LCD_command(0x28);          // set 4-bit data, 2-line, 5x7 font 
	LCD_command(0x06);          // move cursor right 
	LCD_command(0x01);          // clear screen, move cursor to home 
	LCD_command(0x0F);          // turn on display, cursor blinking 
}

void LCD_nibble_write(unsigned char data, unsigned char control)
{
	data &= 0xF0;       				// clear lower nibble for control 
	control &= 0x0F;   				 	// clear upper nibble for data 
	GPIOD_PDOR = data | control;       	// RS = 0, R/W = 0 
	GPIOD_PDOR = data | control | EN;  	// pulse E 
	delayMs(0);
	GPIOD_PDOR = data;
	GPIOD_PDOR = 0;
}


void LCD_command(unsigned char command)
{
	LCD_nibble_write(command & 0xF0, 0);   // upper nibble first 
	LCD_nibble_write(command << 4, 0);     // then lower nibble 
	if (command < 4)
		delayMs(4);         // commands 1 and 2 need up to 1.64ms 
	else
		delayMs(1);         // all others 40 us 
}
  
void LCD_data(unsigned char data)
{
	LCD_nibble_write(data & 0xF0, RS);    // upper nibble first 
	LCD_nibble_write(data << 4, RS);      // then lower nibble  
	delayMs(1);
}


void delayMs(int n) {
	int i;
	int j;
	for(i = 0 ; i < n; i++){
		for(j = 0 ; j < 7000; j++) { }
	}
}

void delayUs(int n){
	int i,j;
	for(i = 0 ; i < n; i++) {
		for(j = 0; j < 5; j++) ;
	}
}
