/*
 * main implementation: use this 'C' sample to create your own application
 *
 */




#include "MKL25z4.h"
#include "derivative.h" 

#define RS 0x04     // PTA2 mask 
#define RW 0x10     // PTA4 mask 
#define EN 0x20     // PTA5 mask 

void delayMs(int n);
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_init(void);

/*void delayMs(int n) {
	int i;
	int j;
	for(i = 0 ; i < n; i++){
		for(j = 0 ; j < 7000; j++) { }
	}
}*/

void delayMs(int n)
{
	LPTMR0_CSR = 0; //se limpia todo el registro para poder utilizarlo
	LPTMR0_CMR = n; //Es con el tiempo que se va comparar 
	LPTMR0_PSR = 0x05; //Prescaler/glitch filter clock 1 selected and Prescaler/glitch filter is bypassed.
	LPTMR0_CSR = 0x01; // This one enbable LPTMR to para que no haga un reset interno 
	while(!(LPTMR_CSR_TCF_MASK & LPTMR0_CSR)){ 
		
	}
}

void LCD_data(unsigned char data){
	GPIOA_PSOR = RS;         // RS = 1
	GPIOA_PCOR = RW;         // RW = 0
	GPIOD_PDOR = data;
	GPIOA_PSOR = EN;         //pulse E 
	delayMs(0);				
	GPIOA_PCOR = EN;
	delayMs(1);
}
void LCD_command(unsigned char command){
	GPIOA_PCOR = RS|RW; 		/*RS=0,R/W=0*/
	GPIOD_PDOR = command;
	GPIOA_PSOR = EN;        //pulse E 
	delayMs(0);
	GPIOA_PCOR = EN;
	
	if (command < 4)
	delayMs(4);         /* command 1 and 2 needs up to 1.64ms */
	else
	delayMs(1);         /* all others 40 us */
}
void LCD_init(void){
	SIM_SCGC5 |= 0x1000;     // enable clock to Port D 
	PORTD_PCR0 = 0x100;      // make PTD0 pin as GPIO 
	PORTD_PCR1 = 0x100;      // make PTD1 pin as GPIO 
	PORTD_PCR2 = 0x100;      // make PTD2 pin as GPIO 
	PORTD_PCR3 = 0x100;      // make PTD3 pin as GPIO 
	PORTD_PCR4 = 0x100;      // make PTD4 pin as GPIO 
	PORTD_PCR5 = 0x100;      // make PTD5 pin as GPIO 
	PORTD_PCR6 = 0x100;      // make PTD6 pin as GPIO 
	PORTD_PCR7 = 0x100;      // make PTD7 pin as GPIO 
	GPIOD_PDDR = 0xFF;       // make PTD7-0 as output pins 
	
	SIM_SCGC5 |= 0x0200;     // enable clock to Port A 
	PORTA_PCR2 = 0x100;      // make PTA2 pin as GPIO 
	PORTA_PCR4 = 0x100;      // make PTA4 pin as GPIO 
	PORTA_PCR5 = 0x100;      // make PTA5 pin as GPIO 
	GPIOA_PDDR |= 0x34;      // make PTA5, 4, 2 as output pins 
	
	delayMs(30);             // initialization sequence 
	LCD_command(0x30);
	delayMs(10);
	LCD_command(0x30);
	delayMs(1);
	LCD_command(0x30);
	delayMs(1);

	LCD_command(0x38); // set 8-bit data, 2-line, 5x7 font 
	LCD_command(0x06); // move cursor right 
	LCD_command(0x01); // clear screen, move cursor to home 
	LCD_command(0x0F); // turn on display, cursor blinking 
}


int main(void)
{
	SIM_SCGC5 |= 0x00000001;
	delayMs(500);
	LCD_init();
	for(;;) {	   
		LCD_command(1);         //clear the display 
		delayMs(500);
		LCD_command(0x80);      // set cursor at first line 
		delayMs(1);
		LCD_data('H');          
	LCD_data('e');
		LCD_data('l'); 
		LCD_data('l'); 
		LCD_data('o');
		delayMs(500);
	}
	
	return 0;
}
