#include "derivative.h" 

#define RS 1    // BIT0 mask 
#define RW 2    // BIT1 mask 
#define EN 4    // BIT2 mask 
 
void LCD_init(void);
void LCD_nibble_write(unsigned char data, unsigned char control);
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);

void TPM_init(void);
void delayUs(int n);
void delayMs(int n);
void TPM_timer(void);

void keypad_init(void);
char keypad_getkey(void);
int read_key(int x);
int getNum(int key);

void led_init(void);

void interupt_init(void);

void writeHello(void);
void askTime(void);
void writeNumbers(int value);

void writeTimer1(int num1, int num2);
void writeTimer2(int num1, int num2);
void writeTimeLeft(void);
void ledandbuzzerOn(void);
void ledandbuzzerOff(void);
void TimeOver(void);



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

void TPM_init(void){
	
	SIM_SCGC6|= SIM_SCGC6_TPM0_MASK;	//assign clk to TPM0
	//
	SIM_SOPT2 |= (SIM_SOPT2 & ~SIM_SOPT2_TPMSRC_MASK)|SIM_SOPT2_TPMSRC(3); //SET CLK SOURCE TO BE 32.768 kHZ 
	MCG_C1 |= MCG_C1_IRCLKEN_MASK;
	TPM0_SC = 0;
	
	TPM0_MOD = 32768-1;				//TPMx_MOD = value
	
	TPM0_SC |= TPM_SC_TOF_MASK;		//Clear TOF  flag
	
	TPM0_SC |= TPM_SC_CMOD(1); 		//Enable timer

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

void TPM_timer(void){
	while (!(TPM0_SC & TPM_SC_TOF_MASK));	//wait until tof is set
	TPM0_SC |= TPM_SC_TOF_MASK; 			//clear tof flag
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

int read_key(int x)
{
	int key_press;
	
	if(x!=0) 				//if a key was pressed return 1
	return key_press = 1;
	else					//if there is no key pressed return 0
	return key_press = 0;
	
}

int getNum(int value){
	if(value == 15)
	{
		return 0;
	}
	else if(value == 4)
	{
		return 1;
	}
	else if(value == 3)
	{
		return 2;
	}
	else if(value == 2)
	{
		return 3;
	}
	else if(value == 8)
	{
		return 4;
	}
	else if(value == 7)
	{
		return 5;
	}
	else if(value == 6)
	{
		return 6;
	}
	else if(value == 12)
	{
		return 7;
	}
	else if(value == 11)
	{
		return 8;
	}
	else if(value == 10)
	{
		return 9;
	}
}
void led_init(void){

	SIM_SCGC5 |= 0x2000;	//enable clk to port E
	
	//RED LED
	PORTE_PCR3 = 0x100;		//make PTE3 as GPIO
	GPIOE_PDDR |= 0x08;		//make PTE3 as output pin
	GPIOE_PCOR |= 0x08;		//turn off red LED 
	//BLUE LED
	PORTE_PCR5 = 0x100;		//make PTE3 as GPIO
	GPIOE_PDDR |= 0x20;		//make PTE3 as output pin
	GPIOE_PCOR |= 0x20;		//turn off blue LED 
	//BUZZER
	PORTE_PCR4 = 0x100;		//make PTE4 as GPIO
	GPIOE_PDDR |= 0x10;		//make PTE4 as output pin
	GPIOE_PCOR |= 0x10;		//turn off buzzer
	
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
	
	/*configure PTA2 for interrupt*/
	PORTA_PCR2 |=  0x00103; 	// make it GPIO and enable pull-up 
	GPIOA_PDDR &= ~0x0004; 		// make pin input  
	PORTA_PCR2 &= ~0xF0000;		// clear interrupt selection  
	PORTA_PCR2 |=  0xA0000; 	// enable falling edge interrupt  
	
	NVIC_ISER |= 0x40000000; 	// enable INT30 (bit 30 of ISER[0])  
}

void PORTA_IRQHandler(void) 
{ 
	char key;
	
	LCD_command(1);         // clear display 
	LCD_command(0x85);      // set cursor at the middle 
	LCD_data('P');
	LCD_data('A');
	LCD_data('U');
	LCD_data('S');
	LCD_data('E');
	LCD_data('D');
	LCD_command(0xC1);
	LCD_data('P');          
	LCD_data('R');
	LCD_data('E');
	LCD_data('S');
	LCD_data('S');
	LCD_data(' ');
	LCD_data('*');
	LCD_data(' ');
	LCD_data('T');
	LCD_data('O');
	LCD_data(' ');          
	LCD_data('P');
	LCD_data('L');
	LCD_data('A');
	LCD_data('Y');
	while(key != 16) 
			{ 
				GPIOE_PSOR |= 0x20;
				key = keypad_getkey();
			} 
	PORTA_ISFR = 0x00000002; // clear interrupt flag
	GPIOE_PCOR |= 0x20; 
	writeTimeLeft();
		
}

void writeHello(void){
	LCD_command(1);         // clear display 
	LCD_command(0x85);      // set cursor at the middle 
	LCD_data('H');          // write the word 
	LCD_data('E');
	LCD_data('L');
	LCD_data('L');
	LCD_data('O');
	delayMs(500);
	LCD_command(1);         // clear display 
}

void askTime(void){
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the middle 
	LCD_data('E');          // write the word 
	LCD_data('N');
	LCD_data('T');
	LCD_data('E');
	LCD_data('R');
	LCD_data(' ');
	LCD_data('#');
	LCD_data(' ');
	LCD_data('O');
	LCD_data('F');
	LCD_command(0xC0);
	LCD_data('S');
	LCD_data('E');
	LCD_data('C');
	LCD_data('O');
	LCD_data('N');
	LCD_data('D');
	LCD_data('S');
	delayMs(500);
	LCD_command(1);         // clear display 
	LCD_command(0x80);
	LCD_data('S');
	LCD_data('E');
	LCD_data('C');
	LCD_data('O');
	LCD_data('N');
	LCD_data('D');
	LCD_data('S');
	LCD_data(':');
	LCD_command(0xC0);
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
	LCD_data('T');
	LCD_data('A');
	LCD_data('R');
	LCD_data('T');
	LCD_command(0x88);
}

void writeNumbers(int value){
	if(value == 0) 	
	{
		LCD_data('0');
	}
	else if(value == 1)
	{
		LCD_data('1');
	}
	else if(value == 2)
	{
		LCD_data('2');
	}
	else if(value == 3)
	{
		LCD_data('3');
	}
	else if(value == 4)
	{
		LCD_data('4');
	}
	else if(value == 5)
	{
		LCD_data('5');
	}
	else if(value == 6)
	{
		LCD_data('6');
	}
	else if(value == 7)
	{
		LCD_data('7');
	}
	else if(value == 8)
	{
		LCD_data('8');
	}
	else if(value == 9)
	{
		LCD_data('9');
	}
	
}
void writeTimeLeft(void){
	LCD_command(1);         // clear display 
	LCD_command(0x82);      // set cursor at the middle 
	LCD_data('S');
	LCD_data('E');
	LCD_data('C');
	LCD_data('O');
	LCD_data('N');
	LCD_data('D');
	LCD_data('S');
	LCD_data(' ');
	LCD_data('L');
	LCD_data('E');
	LCD_data('F');
	LCD_data('T');
}

void writeTimer1(int num1, int num2){
	
	while(num1>0)				//cuando el numero sea igual a 1
	{	
		LCD_command(0xC7);		//Cursor en la segunda linea espacio 7
		writeNumbers(num2);		//Escribir el numero
		LCD_command(0xC8);		//Situar el curso en la segunda linea espacio 8
		writeNumbers(num1);		//Escribir el número actual 
		TPM_timer();			//Esperar 1 segundo
		num1--;					//restarle al num1
	}
	LCD_command(0xC8);			//Situarlo en el curso 8
	LCD_data('0');				//escribir el 0
	
}

void writeTimer2(int num1, int num2)
{
	while(num1>=0)  						// mientras el numero 1 sea mayor o igual a 0, crear un loop
		{
				if(num1==0 && num2>0)		//Numeros menores iguales a 9
				{ 			
					writeTimer1(num2,0);		//Cuenta regresiva 
					num2 = 0;				//termino por lo tanto num2 ya es 0
					
				}
				
				else if(num1>0 && num2>=0)	//Para número mayores a 9
				{
					
					writeTimer1(num2,num1);		//Cuenta regresiva
					TPM_timer();			//esperar 1 segundo
					num1--;					//restarle 1 a num1
					num2 = 9;				//por lo tanto num 2 es 9
				}
				else{						//caso cuando ambos digitos son 0 terminar el loop
					return;
				}
		}
	
}
void ledandbuzzerOn(void){
	GPIOE_PSOR |= 0x08; 	//turn RED LED on and off
	GPIOE_PSOR |= 0x10;		//turn buzzer on
}

void ledandbuzzerOff(void){
	GPIOE_PCOR |= 0x08; 	//turn RED LED on and off
	GPIOE_PCOR |= 0x10;		//turn buzzer on
}

void TimeOver(void){
	LCD_command(1);         // clear display 
	LCD_command(0x84);      // set cursor at the middle 
	LCD_data('T');
	LCD_data('I');
	LCD_data('M');
	LCD_data('E');
	LCD_data(' ');
	LCD_data('O');
	LCD_data('V');
	LCD_data('E');
	LCD_data('R');
	LCD_command(0xC7);			//Situarlo en el curso 8
	LCD_data('0');	
	LCD_command(0xC8);
	LCD_data('0');				//escribir el 0
	
	//turn on and of the red led and buzzer
	ledandbuzzerOn();
	delayMs(300);
	ledandbuzzerOff();
	delayMs(300);
	ledandbuzzerOn();
	delayMs(300);
	ledandbuzzerOff();
	delayMs(300);
	ledandbuzzerOn();
	delayMs(300);
	ledandbuzzerOff();
}

int main(void)
{
	unsigned char key;
	int rkey;
	int m;
	int n;
	int num;
	int num1;
	int num2;
	LCD_init();				
	keypad_init();
	led_init();
	TPM_init();
	interupt_init();
	
	for(;;)
	{
		m = 1;
		n = 0;									//números escritos
		writeHello();							//escribir Hello
		askTime();								//ask for time
		while(m == 1){
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
					}
				}
				else if(key == 14)						//if # is pressed
				{
					if (n==1)							//si solamente un numero fue ingresado
					{	
						writeTimeLeft();				//escribir TIME LEFT en la LCD
						TPM_timer();					//esperar 2 segundos antes de la cuenta regresiva
						TPM_timer();
						writeTimer1(num1,0);				//cuenta regresiva
						TimeOver();						//Despliega TIME OVER y hace paradear el LED y buzzer
						m=2;
						
					}
					else if(n==2){						//si dos numeros fueron ingresados 
						writeTimeLeft();				//escribir TIME LEFT en la LCD
						TPM_timer();					//esperar 2 segundos antes de la cuenta regresiva
						TPM_timer();
						writeTimer2(num1,num2);			//cuenta regresiva
						TimeOver();						//Despliega Time Over y hace parpadear el LED y buzzer
						m=2;
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
