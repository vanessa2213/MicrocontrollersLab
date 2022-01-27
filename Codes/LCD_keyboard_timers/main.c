#include "derivative.h" 

#define RS 1    // BIT0 mask 
#define RW 2    // BIT1 mask 
#define EN 4    // BIT2 mask 
 
void delayMs(int n);
void LCD_nibble_write(unsigned char data, unsigned char control);
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_init(void);

void delayUs(int n);

void keypad_init(void);
char keypad_getkey(void);

void led_init(void);
void led_set(int value);

void writeMenu(void);
int read_key(int x);

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
	data &= 0xF0;       // clear lower nibble for control 
	control &= 0x0F;    // clear upper nibble for data 
	GPIOD_PDOR = data | control;       // RS = 0, R/W = 0 
	GPIOD_PDOR = data | control | EN;  // pulse E 
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
void led_set(int value)
{
	//RED LED
	if (value == 4)
	{
		LCD_command(1);         // clear display 
		LCD_command(0x80);      // set cursor at the begging 
		LCD_data('R');          // write 
		LCD_data('E');
		LCD_data('D');
		LCD_data(' ');
		LCD_data('L');          
		LCD_data('E');
		LCD_data('D');
		LCD_data(' ');
		LCD_data('I');          
		LCD_data('S');
		LCD_data(' ');
		LCD_data('O');
		LCD_data('N');
		LCD_data('!');
		
		GPIOE_PSOR |= 0x08;		//Send 1 to E3
		
		delayMs(500);
		
		GPIOE_PCOR |= 0x08; 	//turn off the LED after several time
				 	 	 	 
	}
	
	//GREEN LED
	else if (value == 3)
	{
		LCD_command(1);         // clear display 
		LCD_command(0x80);      // set cursor at the begging
		LCD_data('G');          // write the word 
		LCD_data('R');
		LCD_data('E');
		LCD_data('E');
		LCD_data('N');
		LCD_data(' ');
		LCD_data('L');          
		LCD_data('E');
		LCD_data('D');
		LCD_data(' ');
		LCD_data('I');          
		LCD_data('S');
		LCD_data(' ');
		LCD_data('O');
		LCD_data('N');
		LCD_data('!');
		
		GPIOE_PSOR |= 0x10; 	//Send 1 to E4
		
		delayMs(500);
		GPIOE_PCOR |= 0x10; 	//turn off the LED after several time
		
	}
	
	
	//BLUE LED
	else if (value == 2)
	{
		
		LCD_command(1);         // clear display 
		LCD_command(0x80);      // set cursor at the begging 
		LCD_data('B');          // write the word 
		LCD_data('L');
		LCD_data('U');
		LCD_data('E');
		LCD_data(' ');
		LCD_data('L');          
		LCD_data('E');
		LCD_data('D');
		LCD_data(' ');
		LCD_data('I');          
		LCD_data('S');
		LCD_data(' ');
		LCD_data('O');
		LCD_data('N');
		LCD_data('!');
		
		GPIOE_PSOR |= 0x20;		//Send 1 to E5
		
		delayMs(500);
		
		GPIOE_PCOR |= 0x20; 	//turn off the LED after several time
		
	}
	
	else{
		GPIOE_PCOR |= 0x10; 	//turn off all the LEDS in case there is no key pressed
		GPIOE_PCOR |= 0x08;
		GPIOE_PCOR |= 0x20;
	}

}
void led_init(void){

	SIM_SCGC5 |= 0x2000;	//enable clk to port E
	
	PORTE_PCR3 = 0x100;		//make PTE3 as GPIO
	GPIOE_PDDR |= 0x08;		//make PTE3 as output pin
	GPIOE_PCOR |= 0x08;		//turn off red LED 
	
	PORTE_PCR4 = 0x100;		//make PTE4 as GPIO
	GPIOE_PDDR |= 0x10;		//make PTE4 as output pin
	GPIOE_PCOR |= 0x10;		//turn off green LED
	
	PORTE_PCR5 = 0x100;		//make PTE5 as GPIO
	GPIOE_PDDR |= 0x20;		//make PTE5 as output pin
	GPIOE_PCOR |= 0x20;		//turn off blue LED
}

void writeMenu(void){
	LCD_command(1);         // clear display 
	LCD_command(0x83);      // set cursor at the middle 
	LCD_data('P');          // write the word 
	LCD_data('R');
	LCD_data('E');
	LCD_data('S');
	LCD_data('S');
	LCD_data(' ');
	LCD_data('K');
	LCD_data('E');
	LCD_data('Y');
	LCD_command(0xC1);   	//set cursor at the second line
	LCD_data('R');          
	LCD_data(':');
	LCD_data('1');
	LCD_data(' ');
	LCD_data(' ');
	LCD_data('G');
	LCD_data(':');
	LCD_data('2');
	LCD_data(' ');
	LCD_data(' ');
	LCD_data('B');
	LCD_data(':');
	LCD_data('3');
}

int main(void)
{
	unsigned char key;
	LCD_init();				
	keypad_init();
	led_init();
	
	for(;;)
	{
		writeMenu();						//escribir el menu de inicio
		delayMs(100);						
		key = keypad_getkey();				//obtener la tecla que se encuentra presionada
		led_set(key);						//turn on the LED depending on the key
		
		
	}
}
