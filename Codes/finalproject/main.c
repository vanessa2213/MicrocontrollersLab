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
void writeIntro(void);

void keypad_init(void);
char keypad_getkey(void);
int read_key(int x);
int getNum(int key);

void writeNumbers(int value);
int writePrice(int n);
void writeUser(void);
void writeUser(void);

void writeContra(void);
void writeNoValid(void);
void newSaldo(int n);
void IncorrectPass(void);
void writeSaldo(int n );
void noMoney(void);
void noMoney(void);
void productSoldOut(void);

void led_init(void);

void TPM_init(void);
void TPM_timer(void);
void TPM1C0init(void);
void setDutyCycleTPM1C0(int n);
void setInitialDuty(int n1, int n2, int n3, int n4);
void PORTA_IRQHandler(void) ;
void interupt_init(void);
void configure(void);
void askCon(void);
void writeConfig(void);
void configuring(void);
void ADC0_init(void);
void writeHighTemp(void);

int dutycycleof10;
int dutycycleof11;
int dutycycleof12;
int dutycycleof13;
unsigned char key;
int m;
int vent;

int main(void)
{
	
	
	int rkey;
	int n;
	int num;
	int num1;
	int num2;
	int us1;
	int us2;
	int user;
	int pas1;
	int pas2;
	int pas3;
	int pas4;
	int password;
	int saldo;
	int price;
	int desnum;
	
	dutycycleof10 = 25;
	dutycycleof11 = 75;
	dutycycleof12 = 50;
	dutycycleof13 = 75;
	vent = 0;
	saldo = 58;
	LCD_init();
	keypad_init();			// Keyboard configure
	led_init();
	TPM_init();
	TPM1C0init();
	interupt_init();
	while(1)
	{
		m = 1;
		n = 0;
		num1 = 0;
		num2 = 0;
		us1 = 0;
		us2 = 0;
		pas1 = 0;
		pas2 = 0;
		pas3 = 0;
		pas4 = 0;
		price  = 0;
		setInitialDuty(dutycycleof10, dutycycleof11, dutycycleof12, dutycycleof13);
		writeIntro();
		while(m == 1)
		{
			
			
			
				key = keypad_getkey(); 				//get which key was pressed
				rkey  = read_key(key); 				//read if the key was pressed
				num = getNum(key);
				
				GPIOE_PCOR |= 0x01;		//turn off red LED 
				GPIOE_PSOR |= 0x02;		//turn on green LED
				
				
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
						while (rkey == 1)				//if still pressed stay here till is not as debouncer
						{				
							key = keypad_getkey();		//get the key pressed
							rkey  = read_key(key);		//read if the key is still pressed
						}
						desnum= num1*10 + num2;			//destemp is the desire temperature
						if (desnum != 10 && desnum != 11 && desnum != 12 && desnum != 13)
						{
							writeNoValid();
							m = 2;
							key  = 16;
						}
						else{
							GPIOE_PSOR |= 0x01;		//turn on red LED 
							GPIOE_PCOR |= 0x02;		//turn off green LED
							price = writePrice(desnum);
						}
						
						while(key != 16)
						{
							key = keypad_getkey(); 				//get which key was pressed
							num = getNum(key);
							
							if(num == 1)
							{
								writeUser();
								n=0;
								while(key != 16)
								{
									key = keypad_getkey(); 				//get which key was pressed
									num = getNum(key);
									rkey  = read_key(key);
									if(rkey == 1)
									{
										
										if(key != 13 && key!=14 && key!=16 && key!= 0 && key!= 1 && key!= 9 && key!= 5) 
															
														//si key no es igual a null,*,#,D,C,B,A
										{
											if (n==0 && num != 0) 				//para escribir el primer numero
											{
												n++;	
												writeNumbers(num);				//escribir el numero presionado
												us1 = num;
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
												us2 = num;
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
											user= us1*10 + us2;			//destemp is the desire temperature
											n = 0;
											writeContra();
											
											while( key != 16)
											{
												key = keypad_getkey(); 				//get which key was pressed
												num = getNum(key);
												rkey  = read_key(key);
												if(rkey == 1)
												{
													if(key != 13 && key!=14 && key!=16 && key!= 0 && key!= 1 && key!= 9 && key!= 5) 
																		
																	//si key no es igual a null,*,#,D,C,B,A
													{
														if (n==0 && num != 0) 				//para escribir el primer numero
														{
															n++;	
															LCD_data('*');				//escribir el numero presionado
															pas1 = num;
															while (rkey == 1)				//if still pressed stay here till is not as debouncer
															{				
																key = keypad_getkey();		//get the key pressed
																rkey  = read_key(key);		//read if the key is still pressed
															}
															
														}
														else if(n==1)						//para escribir el segundo numero
														{
															n++;
															LCD_data('*');				//escribir el numero presionado
															pas2 = num;
															while (rkey == 1)				//if still pressed stay here till is not as debouncer
															{				
																key = keypad_getkey();		//get the key pressed
																rkey  = read_key(key);		//read if the key is still pressed
															}
															
														}
														else if(n==2)						//para escribir el segundo numero
														{
															n++;
															LCD_data('*');				//escribir el numero presionado
															pas3 = num;
															while (rkey == 1)				//if still pressed stay here till is not as debouncer
															{				
																key = keypad_getkey();		//get the key pressed
																rkey  = read_key(key);		//read if the key is still pressed
															}
															
														}
														else if(n==3)						//para escribir el segundo numero
														{
															n++;
															LCD_data('*');				//escribir el numero presionado
															pas4 = num;
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
														password = 1000*pas1 + 100*pas2 + 10*pas3 + pas4;
														if(password == 1111 && user == 22)
														{
															writeSaldo(saldo); //escribir saldo actual
															while( key != 16)
															{	//buy y=1 n=2
																
																key = keypad_getkey(); 				//get which key was pressed
																num = getNum(key);
																if (num == 1)//if 1 check 
																{
																	if (saldo >= price)
																	{
																		
																		if (desnum == 10)//check if dutycycleofx >0
																		{
																			if(dutycycleof10 > 0)
																			{
																				saldo = saldo - price;
																				newSaldo(saldo);
																				
																				TPM_timer();
																				TPM_timer();
																				TPM_timer();
																				TPM_timer();
																				TPM_timer();
																				dutycycleof10 = dutycycleof10 - 25;
																				
																				m=2;
																				key = 16;
																			}
																			else		//this product is soldout
																			{
																				productSoldOut();
																				m=2;
																				key = 16;
																			}
																		}
																		else if(desnum == 11)
																		{
																			if(dutycycleof11 > 0)
																			{
																				saldo = saldo - price;
																				newSaldo(saldo);
																				dutycycleof11 = dutycycleof11 - 25;
																				TPM_timer();
																				TPM_timer();
																				TPM_timer();
																				TPM_timer();
																				TPM_timer();
																				m=2;
																				key = 16;
																				
																				
																			}
																			else		//this product is soldout
																			{
																				productSoldOut();
																				m=2;
																				key = 16;
																			}
																			
																		}
																		else if(desnum == 12)
																		{
																			if(dutycycleof12 > 0)
																			{
																				saldo = saldo - price;
																				newSaldo(saldo);
																				dutycycleof12 = dutycycleof12 - 25;
																				TPM_timer();
																				TPM_timer();
																				TPM_timer();
																				TPM_timer();
																				TPM_timer();
																				m=2;
																				key = 16;
																				
																			}
																			else		//this product is soldout
																			{
																				productSoldOut();
																				m=2;
																				key = 16;
																			}
																			
																			
																		}
																		else if(desnum == 13)
																		{
																			if(dutycycleof13 > 0)
																			{
																				saldo = saldo - price;
																				newSaldo(saldo);
																				dutycycleof13 = dutycycleof13 - 25;
																				TPM_timer();
																				TPM_timer();
																				TPM_timer();
																				TPM_timer();
																				TPM_timer();
																				m=2;
																				key = 16;
																				
																				
																			}
																			else		//this product is soldout
																			{
																				productSoldOut();
																				m=2;
																				key = 16;
																			}
																			
																		}
																	}	
																	
																
																else		//you dont have enough money
																	{
																		noMoney();
																		m=2;
																		key = 16;
																	}
																}
																else if( num == 2)
																{
																	m = 2;
																	key = 16;
																}
															}
														}
														else
														{
															IncorrectPass(); 	//your username or password is incorrect please try again
															m = 2;
															key  = 16;
														}
															
													}
								
											
												}
											}
										}
								
									}
								}
							}
							else if(num == 2)
							{
								key = 16;
								m = 2;
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

void ADC0_init(void) 
{
	SIM_SCGC5 |= 0x2000; 				// clock to PORTE 
	PORTE_PCR29 = 0; 					// PTE20 analog input 
	SIM_SCGC6 |= 0x8000000; 			// clock to ADC0 
	ADC0_SC2 &= ~0x40; 					// software trigger 
	/* clock div by 4, long sample time, single ended 12 bit, bus clock */
	ADC0_CFG1 = 0x40 | 0x10 | 0x04 | 0x00; 
	ADC0_SC3 |= 0x07;
}

void interupt_init(void)
{
	NVIC_ICPR |= 0x40000000;	// disable INT30 (bit 30 of ISER[0]) while configuring 
	SIM_SCGC5 |= 0x200; 		// enable clock to Port A 
		
	// configure PTA1 for interrupt 
	PORTA_PCR1 |=  0x00103; 	// make it GPIO and enable pull-up
	GPIOA_PDDR &= ~0x0002; 		// make pin input 
	PORTA_PCR1 &= ~0xF0000; 	// clear interrupt selection 
	PORTA_PCR1 |=  0xA0000;		// enable falling edge interrupt  
	
	NVIC_ISER |= 0x40000000; 	// enable INT30 (bit 30 of ISER[0])  
}
void PORTA_IRQHandler(void) 
{ 
	char key=0;
	int n = 0;
	int num = 0;
	int dc = 0;
	int pos = 0;
	int pas = 0;
	int num1 = 0;
	int num2 = 0;
	int dc0 = 0;
	int dc1 = 0;
	int dc2 = 0;
	int rkey = 0;
	int pas1 = 0;
	int pas2 = 0;
	int pas3 = 0;
	int pas4 = 0;
	
	GPIOE_PSOR |= 0x01;		//turn on red LED
	GPIOE_PCOR |= 0x02;		//turn off green LED 
	writeContra();
	while(key != 16) 
	{
		
		key = keypad_getkey();			//just in case * was pressed
		rkey  = read_key(key); 				//read if the key was pressed
		num = getNum(key);
		if(key != 13 && key!=14 && key!=16 && key!= 0 && key!= 1 && key!= 9 && key!= 5) 
				
			//si key no es igual a null,*,#,D,C,B,A
			{
				if (n==0 && num != 0) 				//para escribir el primer numero
				{
					n++;	
					LCD_data('*');				//escribir el numero presionado
					pas1 = num;
					while (rkey == 1)				//if still pressed stay here till is not as debouncer
					{				
						key = keypad_getkey();		//get the key pressed
						rkey  = read_key(key);		//read if the key is still pressed
					}
				}
				else if(n==1)						//para escribir el segundo numero
				{
					n++;
					LCD_data('*');			//escribir el numero presionado
					pas2 = num;
					while (rkey == 1)				//if still pressed stay here till is not as debouncer
					{				
						key = keypad_getkey();		//get the key pressed
						rkey  = read_key(key);		//read if the key is still pressed
					}
					
				}
				else if(n==2)						//para escribir el segundo numero
				{
					n++;
					LCD_data('*');				//escribir el numero presionado
					pas3 = num;
					while (rkey == 1)				//if still pressed stay here till is not as debouncer
					{				
						key = keypad_getkey();		//get the key pressed
						rkey  = read_key(key);		//read if the key is still pressed
					}
					
				}
				else if(n==3)						//para escribir el segundo numero
				{
					n++;
					LCD_data('*');				//escribir el numero presionado
					pas4 = num;
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
				pas= pas1*1000 + pas2*100 + pas3*10 + pas4;
				n=0;
				if(pas == 2222)
				{
					writeConfig();
					while(key !=16)
					{
						key = keypad_getkey();			//just in case * was pressed
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
							else if(key == 1){
								
								writeNumbers(num);
								while (rkey == 1)				//if still pressed stay here till is not as debouncer
								{				
									key = keypad_getkey();		//get the key pressed
									rkey  = read_key(key);		//read if the key is still pressed
								}
								LCD_command(0x90);				// ocultar el cursor
								if (vent == 1)
								{
									GPIOE_PCOR |= 0x10;	//turn off fan
									writeFanOff();
									key = 16;
									vent  = 0;
								}
								else
								{
									GPIOE_PSOR |= 0x10;	//turn on fan
									writeFanOn();
									key = 16;
									vent = 1;
								}
							}
							else if(key == 14)						//if # is pressed
							{
								pos = num1*10 + num2;
								if (pos != 10 && pos != 11 && pos != 12 && pos != 13)
								{
									writeNoValid();
									key  = 16;
								}
								else
								{
									askCon();
									n = 0;
								}
								
								while(key!=16)
								{
									key = keypad_getkey();			//just in case * was pressed
									rkey  = read_key(key); 				//read if the key was pressed
									num = getNum(key);
									if(key != 13 && key!=14 && key!=16 && key!= 0 && key!= 1 && key!= 9 && key!= 5) 
															
									//si key no es igual a null,*,#,D,C,B,A
									{
										if (n==0) 				//para escribir el primer numero
										{
											n++;	
											writeNumbers(num);				//escribir el numero presionado
											dc0 = num;
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
											dc1 = num;
											while (rkey == 1)				//if still pressed stay here till is not as debouncer
											{				
												key = keypad_getkey();		//get the key pressed
												rkey  = read_key(key);		//read if the key is still pressed
											}
											
										}
										
										else if(n==2)						//para escribir el segundo numero
										{
											n++;
											writeNumbers(num);				//escribir el numero presionado
											dc2 = num;
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
										dc = dc0 * 100 + dc1*10 + dc2;
										configuring();
										if (pos == 10)
										{
											dutycycleof10 = dc;
											key = 16;
											configure();
										}
										else if(pos == 11)
										{
											dutycycleof11 = dc;
											key = 16;
											configure();
										}
										else if(pos == 12)
										{
											dutycycleof12 = dc;
											key = 16;
											configure();
										}
										else if(pos == 13)
										{
											dutycycleof13 = dc;
											key = 16;
											configure();
										}
										else
										{
											
										}
									}
								}
								
							}
						}
						
					}
					
				}
				else
				{
					IncorrectPass(); 	//your username or password is incorrect please try again
					key  = 16;
				}
			}
		
		
		} 
	PORTA_ISFR = 0x00000006; 	// clear interrupt flag
	m=2;
}

void writeFanOn(void)
{
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the begging first line
	LCD_data('F');
	LCD_data('A');
	LCD_data('N');
	LCD_data(' ');
	LCD_data('T');
	LCD_data('U');
	LCD_data('R');
	LCD_data('N');
	LCD_data(' ');
	LCD_data('O');
	LCD_data('N');
	LCD_data('!');
	LCD_data('!');
	LCD_data('!');
	LCD_command(0x90);	
	delayMs(500);
}
void writeFanOff(void)
{
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the begging first line
	LCD_data('F');
	LCD_data('A');
	LCD_data('N');
	LCD_data(' ');
	LCD_data('T');
	LCD_data('U');
	LCD_data('R');
	LCD_data('N');
	LCD_data(' ');
	LCD_data('O');
	LCD_data('F');
	LCD_data('F');
	LCD_data('!');
	LCD_data('!');
	LCD_data('!');
	LCD_command(0x90);	
	delayMs(500);
}
void configure(void)
{
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the begging first line
	LCD_data('C');
	LCD_data('O');
	LCD_data('N');
	LCD_data('F');
	LCD_data('I');
	LCD_data('G');
	LCD_data('U');
	LCD_data('R');
	LCD_data('E');
	LCD_data('D');
	LCD_data('!');
	LCD_command(0x90);
	delayMs(500);
}
void configuring(void)
{
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the begging first line
	LCD_data('C');
	LCD_data('O');
	LCD_data('N');
	LCD_data('F');
	LCD_data('I');
	LCD_data('G');
	LCD_data('U');
	LCD_data('R');
	LCD_data('I');
	LCD_data('N');
	LCD_data('G');
	LCD_data('.');
	LCD_data('.');
	LCD_data('.');
	LCD_command(0x90);
	delayMs(500);
}
void askCon(void)
{
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the begging first line
	LCD_data('I');
	LCD_data('N');
	LCD_data('T');
	LCD_data('R');
	LCD_data('O');
	LCD_data('D');
	LCD_data('U');
	LCD_data('C');
	LCD_data('E');
	LCD_data(' ');
	LCD_data('N');
	LCD_data('U');
	LCD_data('M');
	LCD_command(0xC0);	  	//set cursor at the begging second line
	LCD_data('T');
	LCD_data('O');
	LCD_data(' ');
	LCD_data('N');
	LCD_data('E');
	LCD_data('W');
	LCD_data(' ');
	LCD_data('D');
	LCD_data('C');
	LCD_command(0x90);
	delayMs(500);
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the begging first line
	LCD_data('N');
	LCD_data('D');
	LCD_data('C');
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
	LCD_command(0x84);
}
void writeConfig(void)
{
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the begging first line
	LCD_data('I');
	LCD_data('N');
	LCD_data('T');
	LCD_data('R');
	LCD_data('O');
	LCD_data('D');
	LCD_data('U');
	LCD_data('C');
	LCD_data('E');
	LCD_data(' ');
	LCD_data('N');
	LCD_data('U');
	LCD_data('M');
	LCD_command(0xC0);	  	//set cursor at the begging second line
	LCD_data('T');
	LCD_data('O');
	LCD_data(' ');
	LCD_data('C');
	LCD_data('O');
	LCD_data('N');
	LCD_data('F');
	LCD_data('I');
	LCD_data('G');
	LCD_command(0x90);
	delayMs(500);
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the begging first line
	LCD_data('N');
	LCD_data('U');
	LCD_data('M');
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
	LCD_command(0x84);
}
void setInitialDuty(int n1, int n2, int n3, int n4){
	TPM1_C0V = (698+1)*n1/100;
	TPM1_C1V = (698+1)*n2/100;
	TPM2_C0V = (698+1)*n3/100;
	TPM2_C1V = (698+1)*n4/100;
}
void TPM1C0init(void){
		
	SIM_SCGC6 |= 0x06000000; 	// enable clock to TPM1 and TPM2 
	
	SIM_SOPT2 |= 0x01000000; 	// use MCGFLLCLK as timer counter clock  
	
	TPM1_SC = 0; 				// disable timer  
	TPM1_C0SC = 0x20 | 0x08; 	// edge-aligned, pulse high 
	TPM1_C1SC = 0x20 | 0x08; 	// edge-aligned, pulse high 
	TPM1_MOD = 698; 			// Set up modulo register for 50 Hz  
	TPM1_SC = 0x08;				// Set up PS /16
	
	TPM2_SC = 0; 				// disable timer  
	TPM2_C0SC = 0x20 | 0x08; 	// edge-aligned, pulse high 
	TPM2_C1SC = 0x20 | 0x08; 	// edge-aligned, pulse high 
	TPM2_MOD = 698; 			// Set up modulo register for 50 Hz  
	TPM2_SC = 0x08;				// Set up PS /16
	

}

void led_init(void){

	SIM_SCGC5 |= 0x2000;	//enable clk to port E
		
	//1st position
	PORTE_PCR20= 0x0300;
	//2nd position
	PORTE_PCR21= 0x0300;
	//3rd position
	PORTE_PCR22= 0x0300;
	//4th position
	PORTE_PCR23= 0x0300;
	
	//fan
	PORTE_PCR4 = 0x100;		//make PTE4 as GPIO
	GPIOE_PDDR |= 0x10;		//make PTE4 as output pin
	GPIOE_PCOR |= 0x10;		//turn off fan 
	//RED LED
	PORTE_PCR0 = 0x100;		//make PTE0 as GPIO
	GPIOE_PDDR |= 0x01;		//make PTE0 as output pin
	GPIOE_PCOR |= 0x01;		//turn off red LED 
	//GREEN LED
	PORTE_PCR1 = 0x100;		//make PTE1 as GPIO
	GPIOE_PDDR |= 0x02;		//make PTE1 as output pin
	GPIOE_PCOR |= 0x02;		//turn off green LED 
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
void newSaldo(int n)
{
	char buffer[4];					//array to save the temp
	int i;
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the begging first line
	LCD_data('Y');
	LCD_data('O');
	LCD_data('U');
	LCD_data('R');
	LCD_data(' ');
	LCD_data('N');
	LCD_data('E');
	LCD_data('W');
	LCD_data(' ');
	LCD_data('S');
	LCD_data('A');
	LCD_data('L');
	LCD_data('D');
	LCD_data('O');
	LCD_command(0xC0);
	LCD_data('I');
	LCD_data('S');
	LCD_data(':');
	LCD_data('$');
	sprintf(buffer, "%i", n);	//convert into an array the value/temperature
	if (n >= 100)
	{
		for(i=0; i<3; i++)
		{
			LCD_data(buffer[i]);		//write the temperature
		}
	}
	
	else if(n >= 10)
	{
		for(i=0; i<2; i++)
		{
			LCD_data(buffer[i]);		//write the temperature
		}
	}
	else if(n>=0)
	{
		for(i=0; i<1; i++)
		{
			LCD_data(buffer[i]);		//write the temperature
		}
	}
	LCD_command(0x90);
}

void noMoney(void)
{
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the begging first line
	LCD_data('Y');
	LCD_data('O');
	LCD_data('U');
	LCD_data(' ');
	LCD_data('D');
	LCD_data('O');
	LCD_data('N');
	LCD_data('T');
	LCD_data(' ');
	LCD_data('H');
	LCD_data('A');
	LCD_data('V');
	LCD_data('E');
	LCD_command(0xC0);
	LCD_data('E');
	LCD_data('N');
	LCD_data('O');
	LCD_data('U');
	LCD_data('G');
	LCD_data('H');
	LCD_data(' ');
	LCD_data('M');
	LCD_data('O');
	LCD_data('N');
	LCD_data('E');
	LCD_data('Y');
	LCD_command(0x90);
	delayMs(500);
	
	
}
void productSoldOut(void)
{
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the begging first line
	LCD_data('T');
	LCD_data('H');
	LCD_data('I');
	LCD_data('S');
	LCD_data(' ');
	LCD_data('P');
	LCD_data('R');
	LCD_data('O');
	LCD_data('D');
	LCD_data('U');
	LCD_data('C');
	LCD_data('T');
	LCD_command(0xC0);
	LCD_data('I');
	LCD_data('S');
	LCD_data(' ');
	LCD_data('S');
	LCD_data('O');
	LCD_data('L');
	LCD_data('D');
	LCD_data(' ');
	LCD_data('O');
	LCD_data('U');
	LCD_data('T');
	LCD_command(0x90);
	delayMs(500);
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the begging first line
	LCD_data('P');
	LCD_data('L');
	LCD_data('E');
	LCD_data('A');
	LCD_data('S');
	LCD_data('E');
	LCD_command(0xC0);
	LCD_data('T');
	LCD_data('R');
	LCD_data('Y');
	LCD_data(' ');
	LCD_data('A');
	LCD_data('G');
	LCD_data('A');
	LCD_data('I');
	LCD_data('N');
	LCD_command(0x90);
	delayMs(500);
}
void writeNoValid(void)
{
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the begging first line
	LCD_data('N');
	LCD_data('O');
	LCD_data(' ');
	LCD_data('V');
	LCD_data('A');
	LCD_data('L');
	LCD_data('I');
	LCD_data('D');
	LCD_command(0xC0);      // set cursor at the begging first line
	LCD_data('O');
	LCD_data('P');
	LCD_data('T');
	LCD_data('I');
	LCD_data('O');
	LCD_data('N');
	LCD_command(0x90);
	delayMs(500);
}
void IncorrectPass(void)
{
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the begging first line
	LCD_data('Y');
	LCD_data('O');
	LCD_data('U');
	LCD_data('R');
	LCD_data(' ');
	LCD_data('U');
	LCD_data('S');
	LCD_data('E');
	LCD_data('R');
	LCD_data(' ');
	LCD_data('O');
	LCD_data('R');
	LCD_command(0xC0);      // set cursor at the begging first line
	LCD_data('P');
	LCD_data('A');
	LCD_data('S');
	LCD_data('S');
	LCD_data('W');
	LCD_data('O');
	LCD_data('R');
	LCD_data('D');
	delayMs(500);
	LCD_command(0x90);
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the begging first line
	LCD_data('I');
	LCD_data('S');
	LCD_data(' ');
	LCD_command(0xC0);
	LCD_data('I');
	LCD_data('N');
	LCD_data('C');
	LCD_data('O');
	LCD_data('R');
	LCD_data('R');
	LCD_data('E');
	LCD_data('C');
	LCD_data('T');
	delayMs(500);
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the begging first line
	LCD_data('P');
	LCD_data('L');
	LCD_data('E');
	LCD_data('A');
	LCD_data('S');
	LCD_data('E');
	LCD_command(0xC0);
	LCD_data('T');
	LCD_data('R');
	LCD_data('Y');
	LCD_data(' ');
	LCD_data('A');
	LCD_data('G');
	LCD_data('A');
	LCD_data('I');
	LCD_data('N');
	LCD_command(0x90);
	delayMs(500);
	
}
void writeContra(void)
{
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the begging first line
	LCD_data('W');
	LCD_data('R');
	LCD_data('I');
	LCD_data('T');
	LCD_data('E');
	LCD_data(' ');
	LCD_data('P');
	LCD_data('A');
	LCD_data('S');
	LCD_data('S');
	LCD_data('W');
	LCD_data('O');
	LCD_data('R');
	LCD_data('D');
	LCD_command(0xC0);      // set cursor at the begging first line
	LCD_data('O');
	LCD_data('F');
	LCD_data(' ');
	LCD_data('U');
	LCD_data('S');
	LCD_data('E');
	LCD_data('R');
	LCD_command(0x90);
	delayMs(500);
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the begging first line
	LCD_data('P');
	LCD_data('A');
	LCD_data('S');
	LCD_data('S');
	LCD_data('W');
	LCD_data('O');
	LCD_data('R');
	LCD_data('D');
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
	LCD_command(0x89);		//set cursor to write the desire num
}
void writeUser(void)
{
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the begging first line
	LCD_data('W');
	LCD_data('R');
	LCD_data('I');
	LCD_data('T');
	LCD_data('E');
	LCD_data(' ');
	LCD_data('N');
	LCD_data('U');
	LCD_data('M');
	LCD_data('B');
	LCD_data('E');
	LCD_data('R');
	LCD_command(0xC0);      // set cursor at the begging first line
	LCD_data('O');
	LCD_data('F');
	LCD_data(' ');
	LCD_data('U');
	LCD_data('S');
	LCD_data('E');
	LCD_data('R');
	LCD_command(0x90);
	delayMs(500);
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the begging first line
	LCD_data('U');
	LCD_data('S');
	LCD_data('E');
	LCD_data('R');
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
	LCD_command(0x85);		//set cursor to write the desire num
	
}
void writeSaldo(int n )
{
	char buffer[3];					//array to save the temp
	int i;
	
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the begging first line
	LCD_data('Y');
	LCD_data('O');
	LCD_data('U');
	LCD_data(' ');
	LCD_data('H');
	LCD_data('A');
	LCD_data('V');
	LCD_data('E');
	LCD_data(' ');
	LCD_data('$');
	sprintf(buffer, "%i", n);	//convert into an array the value/temperature
	if (n >= 100)
	{
		for(i=0; i<3; i++)
		{
			LCD_data(buffer[i]);		//write the temperature
		}
	}
	
	else if(n >= 10)
	{
		for(i=0; i<2; i++)
		{
			LCD_data(buffer[i]);		//write the temperature
		}
	}
	else if(n>=0)
	{
		for(i=0; i<1; i++)
		{
			LCD_data(buffer[i]);		//write the temperature
		}
	}
	
	LCD_command(0xC0);      // set cursor at the begging first line
	LCD_data('B');
	LCD_data('U');
	LCD_data('Y');
	LCD_data('?');
	LCD_data(' ');
	LCD_data(' ');
	LCD_data('Y');
	LCD_data('=');
	LCD_data('1');
	LCD_data(' ');
	LCD_data('N');
	LCD_data('=');
	LCD_data('2');
	LCD_command(0x90); 
	
}
int writePrice(int n )
{
	char buffer[3];					//array to save the temp
	int i;
	int value = 0;
	
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the begging first line
	LCD_data('N');
	LCD_data('U');
	LCD_data('M');
	LCD_data(':');
	LCD_command(0xC0);      // set cursor at the begging first line
	LCD_data('B');
	LCD_data('U');
	LCD_data('Y');
	LCD_data('?');
	LCD_data(' ');
	LCD_data(' ');
	LCD_data('Y');
	LCD_data('=');
	LCD_data('1');
	LCD_data(' ');
	LCD_data('N');
	LCD_data('=');
	LCD_data('2');
	
	sprintf(buffer, "%i", n);	//convert into an array the value/temperature
	LCD_command(0x84);				//set cursor at the second line 1st position
	
	for(i=0; i<2; i++)
	{
		LCD_data(buffer[i]);		//write the temperature
	}
	
	LCD_command(0x8C);      // set cursor at the begging first line
	LCD_data('$');
	if ( n == 10)
	{
		LCD_data('2');
		LCD_data('2');
		LCD_command(0x90); 
		return value = 22;
	}
	else if(n == 11){
		LCD_data('9');
		LCD_command(0x90);
		return value = 9;
	}
	else if(n == 12){
		LCD_data('1');
		LCD_data('2');
		LCD_command(0x90);
		return value = 12;
	}
	else if(n == 13){
		LCD_data('1');
		LCD_data('5');
		LCD_command(0x90);
		return value = 15;
	}
	else
		return value;
	
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
	else if(value == 13)
		LCD_data('A');
}
void writeIntro(void){
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the begging first line
	LCD_data('I');
	LCD_data('N');
	LCD_data('T');
	LCD_data('R');
	LCD_data('O');
	LCD_data('D');
	LCD_data('U');
	LCD_data('C');
	LCD_data('E');
	LCD_data(' ');
	LCD_data('N');
	LCD_data('U');
	LCD_data('M');
	LCD_command(0xC0);	  	//set cursor at the begging second line
	LCD_data('T');
	LCD_data('O');
	LCD_data(' ');
	LCD_data('S');
	LCD_data('E');
	LCD_data('E');
	LCD_data(' ');
	LCD_data('P');
	LCD_data('R');
	LCD_data('I');
	LCD_data('C');
	LCD_data('E');
	LCD_command(0x90);
	delayMs(500);
	LCD_command(1);         // clear display 
	LCD_command(0x80);      // set cursor at the begging first line
	LCD_data('N');
	LCD_data('U');
	LCD_data('M');
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
	LCD_command(0x84);		//set cursor to write the desire num
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
	else if(value == 1)
		return 13;
	else
		return value;
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
