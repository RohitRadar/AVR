/*
 * pose.c
 *
 * Created: 29/09/2021 3:46:44 PM
 * Author : Rohit D
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/eeprom.h>

#define LCD_Dir  DDRC			/* Define LCD data port direction */
#define LCD_Port PORTC			/* Define LCD data port */
#define RS PC0				/* Define Register Select pin */
#define EN PC1 				/* Define Enable signal pin */

float write;
char String1[1];
char String2[4];
unsigned int value;
unsigned int analog;
unsigned int analogcheck=0;
int steps = 0;
unsigned int dist=0;
unsigned int dec=0;
unsigned int num=0;
int analogmode = 0;
unsigned int desired=0;
int starta;
int y;
int x;
int dir = 1;
char text[2];
int start=1;
float av;
int avint;

void LCD_Command( unsigned char cmnd )
{
	LCD_Port = (LCD_Port & 0x0F) | (cmnd & 0xF0); /* sending upper nibble */
	LCD_Port &= ~ (1<<RS);		/* RS=0, command reg. */
	LCD_Port |= (1<<EN);		/* Enable pulse */
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	_delay_us(200);
	LCD_Port = (LCD_Port & 0x0F) | (cmnd << 4);  /* sending lower nibble */
	LCD_Port |= (1<<EN);
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	_delay_ms(2);
}


void LCD_Char( unsigned char data )
{
	LCD_Port = (LCD_Port & 0x0F) | (data & 0xF0); /* sending upper nibble */
	LCD_Port |= (1<<RS);		/* RS=1, data reg. */
	LCD_Port|= (1<<EN);
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	_delay_us(200);
	LCD_Port = (LCD_Port & 0x0F) | (data << 4); /* sending lower nibble */
	LCD_Port |= (1<<EN);
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	_delay_ms(2);
}

void LCD_Init (void)			/* LCD Initialize function */
{
	LCD_Dir = 0xFF;			/* Make LCD port direction as o/p */
	_delay_ms(20);			/* LCD Power ON delay always >15ms */
	LCD_Command(0x02);		/* send for 4 bit initialization of LCD  */
	LCD_Command(0x28);              /* 2 line, 5*7 matrix in 4-bit mode */
	LCD_Command(0x0c);              /* Display on cursor off*/
	LCD_Command(0x06);              /* Increment cursor (shift cursor to right)*/
	LCD_Command(0x01);              /* Clear display screen*/
	_delay_ms(2);
}


void LCD_String (char *str)		/* Send string to LCD function */
{
	int i;
	for(i=0;str[i]!=0;i++)		/* Send each char of string till the NULL */
	{
		LCD_Char (str[i]);
	}
}

void LCD_String_xy (char row, char pos, char *str)	/* Send string to LCD with xy position */
{
	if (row == 0 && pos<16)
	LCD_Command((pos & 0x0F)|0x80);	/* Command of first row and required position<16 */
	else if (row == 1 && pos<16)
	LCD_Command((pos & 0x0F)|0xC0);	/* Command of first row and required position<16 */
	LCD_String(str);		/* Call LCD string function */
}

void LCD_Clear()
{
	LCD_Command (0x01);		/* Clear display */
	_delay_ms(2);
	LCD_Command (0x80);		/* Cursor at home position */
}

void LCD_right(){
	LCD_Command(0x14);
	LCD_Command(0x14);
	LCD_Command(0x14);
	LCD_Command(0x14);
	LCD_Command(0x14);
	LCD_Command(0x14);
}

void ADC_Init()
{
	DDRA=0x0;	
	ADCSRA = 0x87;	
	ADMUX = 0x40;
}

int ADC_Read(char channel)
{
	int Ain,AinLow;
	ADMUX=ADMUX|(channel & 0x0f);
	ADCSRA |= (1<<ADSC);		
	while((ADCSRA&(1<<ADIF))==0);	
	_delay_us(10);
	AinLow = (int)ADCL;		
	Ain = (int)ADCH*255;			
	Ain = Ain + AinLow;				
	return(Ain);			
}

void disp_dist(){
	write = steps*1.0;
	eeprom_write_float((float *) 20, write);
	if (steps<=0){
		steps=0;
		dist = 0;
		dec = 0;
		num = 0;
	}
	else{
		dist = 15*(steps/20);
		dec = dist%100;
		num = dist/100;
	}
	LCD_Command(0x90);
	LCD_right();
	LCD_Command(0x14);
	LCD_Command(0x14);
	LCD_Command(0x14);
	sprintf(text, "%d", num); /*2*2.5 ms*/
	if (num<10){
		LCD_String(" ");
		LCD_String(text);
	}
	else{
		LCD_String(text);
	}
	LCD_Command(0x14);
	sprintf(text, "%d", dec);
	if (dec==0){
		LCD_String("0");
		LCD_String("0");
	}
	else if(dec<10){
		LCD_String("0");
		LCD_String(text);
	}
	else{
		LCD_String(text);
	}
}

void disp_av(){
	write = steps*1.0;
	eeprom_write_float((float *) 20, write);
	if (steps<=0){
		steps=0;
		dist = 0;
		dec = 0;
		num = 0;
	}
	else{
		dist = 15*(steps/20);
		dec = dist%100;
		num = dist/100;
	}
	LCD_Command(0xC3);
	sprintf(text, "%d", num); /*2*2.5 ms*/
	if (num<10){
		LCD_String(" ");
		LCD_String(text);
	}
	else{
		LCD_String(text);
	}
	LCD_String(".");
	sprintf(text, "%d", dec);
	if (dec==0){
		LCD_String("0");
		LCD_String("0");
	}
	else if(dec<10){
		LCD_String("0");
		LCD_String(text);
	}
	else{
		LCD_String(text);
	}
}

void disp_an(){
	value = ADC_Read(0);
	value = value*(5000/1023);
	desired = 40*(value/25);
	analog = value/1000;
	sprintf(String1,"%d",analog);
	LCD_Command(0xC0);
	LCD_Command(0x14);
	LCD_Command(0x14);
	LCD_Command(0x14);
	if (analog==0){
		LCD_String("0");
	}
	else{
		LCD_String(String1);
	}
	LCD_String(".");
	analog = value - (1000*analog);
	sprintf(String2,"%d",analog);
	if (analog==0){
		LCD_String("000");
	}
	else if (analog<10){
		LCD_String("00");
		LCD_String(String2);
	}
	else if(analog<100){
		LCD_String("0");
		LCD_String(String2);
	}
	else{
		LCD_String(String2);
	}
	analogcheck=0;
}

void homepos(){
	while(((PIND & (1<<PIND3))==0)&&(start>0)){
		write = steps*1.0;
		eeprom_write_float((float *) 20, write);
		steps=steps-1;
		if ((steps%20)==0){
			disp_dist();
			if (analogmode==0){
				disp_av();	
			}
		}
		if (PIND &(1<<PIND7)){
			_delay_ms(50);
			if (PIND&(1<<PIND7)){
				start=0-start;
			}
		}
		PORTB |= (1<<PINB0);
		_delay_ms(1);
		PORTB &= ~(1<<PINB0);
		_delay_ms(1);
	}
	_delay_ms(10);
	if (((PIND & (1<<PIND3))==0)&&(start>0)){
		homepos();
	}
	steps=0;
}

void go(int y){
	x=0;
	while(x<y){
		x=x+1;
		if (PIND &(1<<PIND7)){
			_delay_ms(100);
			if (PIND&(1<<PIND7)){
				start=0-start;
			}
		}
		if ((steps%20)==0){
			disp_dist();
			if (analogmode==0){
				disp_av();
			}
		}
		if (start>0){
			steps = steps+(1*dir);
			PORTB ^= (1<<PINB0);
			_delay_ms(15);
			PORTB ^= (1<<PINB0);
			_delay_ms(15);
		}
	}
}

void comphome(){
	LCD_Command(0x80);
	LCD_String("Homing       ");
	PORTB &= ~(1<<PINB4);
	PORTB &= ~(1<<PINB1);
	homepos();
	PORTB |= (1<<PINB1);
	y=333;
	go(y);
	LCD_Command(0x80);
	LCD_String("Trigger On");
	PORTB |= (1<<PINB4);
	steps=y;
}

int main(void)
{	
	write = eeprom_read_float((float *) 20);
	steps = write/1;
	if (steps<=0){
		steps=0;
		dist = 0;
		dec = 0;
		num = 0;
	}
	else{
		dist = 15*(steps/20);
		dec = dist%100;
		num = dist/100;
	}
	ADC_Init();
	MCUCSR |=(1<<JTD);
	MCUCSR |=(1<<JTD);
	DDRB |= 0b00010011;
	DDRD |= 0b00000000;
	PORTD = 0b11111111;
	LCD_Init();		
	LCD_Command(0x80);
	LCD_String("Trigger On");	
	LCD_Command(0xC0);
	LCD_String("AV:     kV");
	LCD_Command(0x90);
	LCD_String("Distance:");
	LCD_Command(0x14);
	LCD_Command(0x14);
	LCD_String(".");
	LCD_Command(0x14);
	LCD_Command(0x14);
	LCD_String("mm");
	LCD_Command(0xD0);
	if (PINB&(1<<PINB3)){
		LCD_String("Mode:Remote ");		
	}
	else{
		LCD_String("Mode:Local ");
		comphome();
	}
	while (1)
    {
		while (analogmode==0){
			if (PIND &(1<<PIND7)){
				start=0-start;
				_delay_ms(500);
			}
			if (PINB & (1<<PINB3)){
				analogmode=1;
				_delay_ms(500);
				LCD_Command(0xC0);
				LCD_String("AV:     V ");
				LCD_Command(0xD0);
				LCD_String("Mode:Remote   ");
				disp_an();
				comphome();
			}
			if ((steps%20)==0){
				disp_dist();
				disp_av();
			}
			if (PIND & (1<<PIND3)){
				_delay_ms(10);
				if (PIND & (1<<PIND3)){	
					steps=0;
				}
			}
			if (PIND & (1<<PIND2)){
				_delay_ms(10);
				if (PIND & (1<<PIND2)){
					steps=8000;
				}
			}
			if (PIND & (1<<PIND4)){
				LCD_Command(0x80);
				LCD_String("           ");
				PORTB &= ~(1<<PINB4);
				if (PIND & (1<<PIND3)){
					_delay_ms(10);
					if (PIND & (1<<PIND3)){
						steps=0;
					}
				}
				else{
					dir=1;
					PORTB |= (1<<PINB1);
					y=20;
					go(y);
				}
				LCD_Command(0x80);
				LCD_String("Trigger On");
				PORTB |= (1<<PINB4);
			}
			if (PIND & (1<<PIND5)){
				LCD_Command(0x80);
				LCD_String("           ");
				PORTB &= ~(1<<PINB4);
				if(PIND & (1<<PIND2)){
					_delay_ms(10);
					if (PIND & (1<<PIND2)){
						steps=8000;	
					}
				}
				else{
					dir=-1;
					PORTB &= ~(1<<PINB1);
					y=20;
					go(y);
				}
				LCD_Command(0x80);
				LCD_String("Trigger On");
				PORTB |= (1<<PINB4);
			}
			if (PIND & (1<<PIND1)){
				comphome();
			}
		}
		
		while (analogmode==1){
			if (PIND &(1<<PIND7)){
				start=0-start;
				_delay_ms(500);
			}
			if (PINB & (1<<PINB2)){
				_delay_ms(500);
				LCD_Command(0xC0);
				LCD_String("AV:     kV");
				LCD_Command(0xD0);
				LCD_String("Mode:Local    ");
				analogmode=0;
				comphome();
			}
			if ((steps%20)==0){
				disp_dist();
				LCD_Command(0x80);
				LCD_String("           ");
				PORTB &= ~(1<<PINB4);
			}
			if (analogcheck>20){
				disp_an();
			}
			analogcheck=analogcheck+1;
			starta=1;
			if (steps>desired){
				PORTB &= ~(1<<PINB1);
				dir=-1;
			}
			else if (steps<desired){
				PORTB |= (1<<PINB1);
				dir=1;
			}
			else{
				starta=-1;
				LCD_Command(0x80);
				LCD_String("Trigger On");
				PORTB |= (1<<PINB4);
			}
			if (starta==1){
				y=1;
				go(1);
			}
		}
	}
}
/*
A0: Analog Input
B0: Motor Pulse (HIGH:+ve dir)
B1: Motor Direction
B2: Up/Down Home Position Reach Signal
D1: Home Position (B1 HIGH, Go Down)
D2: Down Stopper(-ve dir)
D3: Up Stopper (+ve dir)
D4: Go up
D5: Go Down
D6: Analog Choose
D7: 
*/
/*update:
D5:analog Choose
*/
/*
RPM DISP 150
CHECK ANALOG PROPER
*/

/*
void dispav(){
	LCD_Command(0xC3);
	av = (3.0*steps)/400.0;
	avint=av/1;
	sprintf(text, "%d", avint);
	if (avint<1){
		LCD_String(" 0");
	}
	else if (av<10){
		LCD_String(" ");
		LCD_String(text);
	}
	else{
		LCD_String(text);
	}
	LCD_String(".");
	av=av*100.0;
	avint=av/1;
	avint=avint%100;
	sprintf(text, "%d", avint);
	if (avint<1){
		LCD_String("00");
	}
	else if (av<10){
		LCD_String("0");
		LCD_String(text);
	}
	else{
		LCD_String(text);
	}
}
*/