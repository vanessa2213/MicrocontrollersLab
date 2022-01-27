/* A to D conversion of channel 0 * This program converts the analog input from channel 0 (PTE20) 
 * using software trigger continuously. * Connect LM35 10mV/°C to PTE20. 
 */
#include "MKL25Z4.h" 

#define RS 1    // BIT0 mask 
#define RW 2    // BIT1 mask 
#define EN 4    // BIT2 mask 

void ADC0_init(void); 
void LED_set(int s); 
void led_init(void);

void LCD_nibble_write(unsigned char data, unsigned char control);
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_init(void);

void delayUs(int n);
void delayMs(int n);
void writeTemperature(int value);

void TPM_init(void);
void TPM_timer(void);

void keypad_init(void);
char keypad_getkey(void);
int read_key(int x);
int getNum(int key);

void interupt_init(void);

void askTemperature(void);
void writeNumbers(int value);

void ledandbuzzerOn(void);
void ledandbuzzerOff(void);

void writeTempNorm(void);
void PORTA_IRQHandler(void) ;
int main (void) 
{
	short int result;
	short int temperature;
	unsigned char key;
	int rkey;
	int m;
	int c=0;
	int n;
	int num;
	int num1 = 0;
	int num2 = 0;
	short int destemp;
	led_init(); 			// Configure LEDs 
	ADC0_init(); 			// Configure ADC0 
	LCD_init();				// LCD Configure		
	keypad_init();			// Keyboard configure
	TPM_init();				// Timer Configure
	interupt_init();		// Interuption Configure
	
	while (1)
	{
		//Pruebas de sensor LM35
		
		ADC0_SC1A = 0; 				//start conversion on channel 0 
		while(!(ADC0_SC1A & 0x80)) { } 	// wait for conversion complete  
		result = ADC0_RA; 				// read conversion result and clear COCO flag  
		temperature = result* 500 / 65536;		//Converter to Celsius 
		
		
		/*LCD_command(1);         // clear display 
		LCD_command(0x80);      // set cursor at the middle 
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
		LCD_command(0xC2);
		LCD_data(0xDF);
		LCD_data('C');
		writeTemperature(temperature);
		delayMs(500);
		
		m = 1;
		n = 0;								//números escritos
		
		GPIOE_PCOR |= 0x20;					//turn off fan
		GPIOE_PCOR |= 0x08;					//turn off red led
		GPIOE_PCOR |= 0x02;					//turn off green led
		GPIOE_PCOR |= 0x01;					//turn off blue led
		GPIOE_PCOR |= 0x10;					//turn off buzzer
		
		askTemperature();					
		
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
					if (n==0 && num != 0) 				//para escribir el primer numero
					{
						n++;	
						writeNumbers(num);				//escribir el numero presionado
						num1 = num;
						while (rkey == 1)				//if still pressed stay here till is not as debouncer
						{				
							key = keypad_getkey();		//get the key pressed
							rkey  = read_key(key);		//read if the key is still pressed
						}
					}
					else if(n==1)						//para escribir el segundo numero
					{
						n++;
						writeNumbers(num);				//escribir el numero presionado
						num2 = num;
						while (rkey == 1)				//if still pressed stay here till is not as debouncer
						{				
							key = keypad_getkey();		//get the key pressed
							rkey  = read_key(key);		//read if the key is still pressed
						}
						LCD_command(0x90);				// ocultar el cursor
					}
				}
				else if(key == 14)						//if # is pressed
				{
					destemp= num1*10 + num2;			//destemp is the desire temperature
					
					ADC0_SC1A = 0; 							//start conversion on channel 0 where LM45 is connected
					while(!(ADC0_SC1A & 0x80)) { } 			// wait for conversion complete  
					result = ADC0_RA; 						// read conversion result and clear COCO flag  
					temperature = result* 50 / 1024;		//Converter to Celsius 
					
					writeTempNorm();						//Write Temperature:XX°C  Status normal.
					
					while(key != 16)						//mientras * not pressed
					{
						
						while(temperature < destemp && key != 16 )	//mientras temperature do not reach desire temp
						{
							key = keypad_getkey();					//get if the key was pressed
							writeTemperature(temperature);			//write the actual temp
							LCD_command(0x90);						//ocultar cursor
							
							GPIOE_PCOR |= 0x20;			//turn off fan
							GPIOE_PCOR |= 0x08;			//turn off red led
							GPIOE_PSOR |= 0x02;			//turn green led
							GPIOE_PCOR |= 0x10;			//buzzer
							GPIOE_PCOR |= 0x01;			//turn off blue led
							
							TPM_timer();				//wait 1 second
							c++;			
							
							if (c==30)
							{							//every 30 sec
								
								GPIOE_PCOR |= 0x02;				//turn off green led
								GPIOE_PSOR |= 0x01;				//turn on blue led
								GPIOE_PSOR |= 0x20;				//set on fan
								int i;
								for(i=0; i<10; i++)
								{				//10 seconds
									
									ADC0_SC1A = 0; 						//start conversion on channel 0 
									while(!(ADC0_SC1A & 0x80)) { } 		// wait for conversion complete  
									result = ADC0_RA; 					// read conversion result and clear COCO flag  
									temperature = result* 50 / 1024;	//Converter to Celsius 
									writeTemperature(temperature);		//Write actual temperature
									LCD_command(0x90);					//ocultar cursor
									TPM_timer();						// wait 1 second
									key = keypad_getkey();				//get the key pressed
								}
								c=0;									//reset counter
							}
							
							ADC0_SC1A = 0; 						//start conversion on channel 0 
							while(!(ADC0_SC1A & 0x80)) { } 		// wait for conversion complete  
							result = ADC0_RA; 					// read conversion result and clear COCO flag  
							temperature = result* 50 / 1024;	//Converter to Celsius 
							
						}
						while(temperature >= destemp && key != 16)
						{
							key = keypad_getkey();				//get if the  * key pressed
							writeTemperature(temperature);		//write the actual temperature
							
							GPIOE_PSOR |= 0x08;					//turn on red led
							GPIOE_PSOR |= 0x10;					//turn on buzzer
							GPIOE_PCOR |= 0x02;					//turn off green led	
							GPIOE_PSOR |= 0x20;					//set on fan
							
							ADC0_SC1A = 0; 						//start conversion on channel 0 
							while(!(ADC0_SC1A & 0x80)) { } 		// wait for conversion complete  
							result = ADC0_RA; 					// read conversion result and clear COCO flag  
							temperature = result* 50 / 1024;	//Converter to Celsius 
							writeTemperature(temperature);		//write actual temperture
							LCD_command(0x90);					//hide the cursor
							TPM_timer();						//wait 1 second
							
						}
					}
				}
				else if(key == 16) 						//if * was pressed reset
				{
					m = 2;
				}
			}
		}*/
	 
	}
}

void writeTempIDLE(void) //write mode :IDLE
{
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the begging first line
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
	LCD_command(0xC2);	  	//set cursor at the 3rd position in the second line
	LCD_data(0xDF);			//write °
	LCD_data('C');			//write C
	LCD_command(0xCC);		//Set cursor almost at the end
	LCD_data('I');			//Mode: IDLE
	LCD_data('D');
	LCD_data('L');
	LCD_data('E');
}
void PORTA_IRQHandler(void) 
{ 
	char key;
	short int result;
	short int temperature;
	GPIOE_PCOR |= 0x08;			//turn off red led
	GPIOE_PCOR |= 0x10;			//turn off buzzer
	
	writeTempIDLE();
	while(key != 16) 
			{ 
				GPIOE_PSOR |= 0x20;				//turn on fan
				GPIOE_PSOR |= 0x01;				//turn blue led
				GPIOE_PSOR |= 0x02;				//turn green led
				
				key = keypad_getkey();			//just in case * was pressed
				
				ADC0_SC1A = 0; 						//start conversion on channel 0 
				while(!(ADC0_SC1A & 0x80)) { } 		// wait for conversion complete  
				result = ADC0_RA; 					// read conversion result and clear COCO flag  
				temperature = result* 50 / 1024;	//Converter to Celsius 
				writeTemperature(temperature);		//Write the actual temperture
				LCD_command(0x90);					//mandar a chsm el cursor :D
				TPM_timer();						//wait 1 second
			} 
	PORTA_ISFR = 0x00000002; 	// clear interrupt flag
	GPIOE_PCOR |= 0x20; 		// turn off fan
	GPIOE_PCOR |= 0x01;			// turn off blue led
	GPIOE_PCOR |= 0x02;			// turn off green led
	writeTempNorm();			// write mode: Normal
		
}

void writeTempNorm(void){
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the middle 
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
	LCD_command(0xC2);		//set cursor 2nd line 3rd position
	LCD_data(0xDF);			//write °
	LCD_data('C');			
	LCD_command(0xCC);		//set cursor almost at the end
	LCD_data('N');
	LCD_data('O');
	LCD_data('R');
	LCD_data('M');
}
	
void writeNumbers(int value)	//write numbers on the LCD
{
	if(value == 0) 	
		LCD_data('0');
	else if(value == 1)
		LCD_data('1');
	else if(value == 2)
		LCD_data('2');
	else if(value == 3)
		LCD_data('3');
	else if(value == 4)
		LCD_data('4');
	else if(value == 5)
		LCD_data('5');
	else if(value == 6)
		LCD_data('6');
	else if(value == 7)
		LCD_data('7');
	else if(value == 8)
		LCD_data('8');
	else if(value == 9)
		LCD_data('9');
}
void askTemperature(void)	//ask for the desire temperature
{
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the middle 
	LCD_data('S');
	LCD_data('E');
	LCD_data('T');
	LCD_data(' ');
	LCD_data('T');
	LCD_data('H');
	LCD_data('E');
	LCD_data(' ');
	LCD_data('D');
	LCD_data('E');
	LCD_data('S');
	LCD_data('I');
	LCD_data('R');
	LCD_data('E');
	LCD_command(0xC0);		//set cursor at the second line
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
	delayMs(500);			// delay 500 ms
	LCD_command(1);         // clear display 
	LCD_command(0x80);		//set cursor 1st line 1st position
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
	LCD_command(0xC0);		//set cursor at the second line first position
	LCD_data('P');          
	LCD_data('R');
	LCD_data('E');
	LCD_data('S');
	LCD_data('S');
	LCD_data(' ');
	LCD_data('#');
	LCD_data(' ');
	LCD_data('T');
	LCD_data('O');
	LCD_data(' ');          
	LCD_data('S');
	LCD_data('E');
	LCD_data('T');
	LCD_command(0x8E);		//set cursor afet the numbers of the desire temperature that will be introduce
	LCD_data(0xDF);			//write °
	LCD_data('C');
	LCD_command(0x8C);		//set cursor to write the desire temp
	
}
void TPM_init(void)
{
	SIM_SCGC6|= SIM_SCGC6_TPM0_MASK;										//assign clk to TPM0
	
	SIM_SOPT2 |= (SIM_SOPT2 & ~SIM_SOPT2_TPMSRC_MASK)|SIM_SOPT2_TPMSRC(3); //SET CLK SOURCE TO BE 32.768 kHZ 
	MCG_C1 |= MCG_C1_IRCLKEN_MASK;
	TPM0_SC = 0;
	
	TPM0_MOD = 32768-1;				//TPMx_MOD = value
	
	TPM0_SC |= TPM_SC_TOF_MASK;		//Clear TOF  flag
	
	TPM0_SC |= TPM_SC_CMOD(1); 		//Enable timer
}

void TPM_timer(void){
	while (!(TPM0_SC & TPM_SC_TOF_MASK));	//wait until tof is set
	TPM0_SC |= TPM_SC_TOF_MASK; 			//clear tof flag
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

void writeTemperature(int value)	//write the actual temperature from the LM35
{
	char buffer[3];					//array to save the temp
	int i;
	sprintf(buffer, "%i", value);	//convert into an array the value/temperature
	LCD_command(0xC0);				//set cursor at the second line 1st position
	for(i=0; i<2; i++){
		LCD_data(buffer[i]);		//write the temperature
	}
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

void ADC0_init(void) 
{
	SIM_SCGC5 |= 0x2000; 				// clock to PORTE 
	PORTE_PCR29 = 0; 					// PTE20 analog input 
	SIM_SCGC6 |= 0x8000000; 			// clock to ADC0 
	ADC0_SC2 &= ~0x40; 					// software trigger 
	/* clock div by 4, long sample time, single ended 12 bit, bus clock */
	ADC0_CFG1 = 0x40 | 0x10 | 0x04 | 0x00; 
}

void led_init(void){

	SIM_SCGC5 |= 0x2000;	//enable clk to port E
	
	//RED LED
	PORTE_PCR3 = 0x100;		//make PTE3 as GPIO
	GPIOE_PDDR |= 0x08;		//make PTE3 as output pin
	GPIOE_PCOR |= 0x08;		//turn off red LED 
	//Fan
	PORTE_PCR5 = 0x100;		//make PTE5 as GPIO
	GPIOE_PDDR |= 0x20;		//make PTE5 as output pin
	GPIOE_PCOR |= 0x20;		//turn off fan
	//BUZZER
	PORTE_PCR4 = 0x100;		//make PTE4 as GPIO
	GPIOE_PDDR |= 0x10;		//make PTE4 as output pin
	GPIOE_PCOR |= 0x10;		//turn off buzzer
	//BLUE LED
	PORTE_PCR0 = 0x100;		//make PTE1 as GPIO
	GPIOE_PDDR |= 0x01;		//make PTE1 as output pin
	GPIOE_PCOR |= 0x01;		//turn off blue LED 
	//GREEN LED
	PORTE_PCR1 = 0x100;		//make PTE0 as GPIO
	GPIOE_PDDR |= 0x02;		//make PTE0 as output pin
	GPIOE_PCOR |= 0x02;		//turn off green LED 
}

void interupt_init(void)
{
	NVIC_ICPR |= 0x40000000;	// disable INT30 (bit 30 of ISER[0]) while configuring 
	SIM_SCGC5 |= 0x200; 		// enable clock to Port A 
		
	/* configure PTA1 for interrupt */
	PORTA_PCR1 |=  0x00103; 	// make it GPIO and enable pull-up
	GPIOA_PDDR &= ~0x0002; 		// make pin input 
	PORTA_PCR1 &= ~0xF0000; 	// clear interrupt selection 
	PORTA_PCR1 |=  0xA0000;		// enable falling edge interrupt  
	
	NVIC_ISER |= 0x40000000; 	// enable INT30 (bit 30 of ISER[0])  
}
