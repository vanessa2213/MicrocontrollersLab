
#include "MKL25z4.h"
#include "derivative.h" 

#define RS 1    // BIT0 mask 
#define RW 2    // BIT1 mask 
#define EN 4    // BIT2 mask 
 
void delayMs(int n);
void LCD_nibble_write(unsigned char data, unsigned char control);
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_init(void);
 

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

int main(void)
{
	LCD_init();
	for(;;)
	{
	 LCD_command(1);         // clear display 
	 delayMs(500);
	 LCD_command(0x85);      // set cursor at the middle 
	 LCD_data('H');          // write the word 
	 LCD_data('e');
	 LCD_data('l');
	 LCD_data('l');
	 LCD_data('o');
	 delayMs(500);
	}
}
