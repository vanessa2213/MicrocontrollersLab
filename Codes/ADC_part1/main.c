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
void writeNumbers(char value);
void writeTemperature(int value);

void ADC0_init(void); 


int main (void) 
{ 
	short int result; 
	short int temperature;
	ADC0_init(); 					// Configure ADC0 
	LCD_init();						// LCD configure
	while (1) 
	{ 
		ADC0_SC1A = 26; 						// start conversion on channel 26 temperature 
		while(!(ADC0_SC1A & 0x80)) { } 			// wait for COCO 
		result = ADC0_RA; 						// read conversion result and clear COCO flag  
		temperature = ((result/10)-32)*5/9;		//Converter to Celsius 
		writeTemperature(temperature);			//write the actual temperature on the LCD
		delayMs(250);							//wait 250 ms
	}
}

void writeTemperature(int value)
{
	char buffer[3];								//an array to save the temperature
	int i;
	sprintf(buffer, "%i", value);				//converts the temperature into an array 
	LCD_command(1);       					 	// clear display 
	LCD_command(0x80);      					// set cursor at the begging
	LCD_data('T');
	LCD_data('E');
	LCD_data('M');
	LCD_data('P');
	LCD_data('E');
	LCD_data('R');
	LCD_data('A');
	LCD_data('T');
	LCD_data('U');
	LCD_data('R');
	LCD_data('E');
	LCD_data(':');
	LCD_command(0xC0);							//set cursos at the second line
	
	for(i=0; i<2; i++){
		LCD_data(buffer[i]);					//write the two numbers
	}
	LCD_data(0xDF);								//write ° on the LCD
	LCD_data('C');								//write C
	LCD_command(0x90);							//mandar muy lejos lejitos al cursor
		
}

void ADC0_init(void) 
{ 
	SIM_SCGC6 |= 0x8000000; 					// clock to ADC0 
	ADC0_SC2 &= ~0x40; 							// software trigger 
	
	/* clock div by 4, long sample time, single ended 12 bit, bus clock */ 
	ADC0_CFG1 = 0x40 | 0x10 | 0x04 | 0x00; 
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
	data &= 0xF0;      						// clear lower nibble for control 
	control &= 0x0F;    					// clear upper nibble for data 
	GPIOD_PDOR = data | control;      	 	// RS = 0, R/W = 0 
	GPIOD_PDOR = data | control | EN;  		// pulse E 
	delayMs(0);
	GPIOD_PDOR = data;
	GPIOD_PDOR = 0;
}


void LCD_command(unsigned char command)
{
	LCD_nibble_write(command & 0xF0, 0);   	// upper nibble first 
	LCD_nibble_write(command << 4, 0);     	// then lower nibble 
	if (command < 4)
		delayMs(4);         				// commands 1 and 2 need up to 1.64ms 
	else
		delayMs(1);         				// all others 40 us 
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
