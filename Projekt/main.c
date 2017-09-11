#include <pic32mx.h>	/* Declarations of system-specific addresses etc */
#include <stdint.h>		/* Declarations of uint_32 and the like */
#include "mipslab.h"	/* Declatations for these labs */

char textbuffer[4][16];
char textstring[] = "Vi gillar D-TEK";

#define TMR2PERIOD ((80000000 / 256) / 10) //Timerperioden, samma som i lab3
#if TMR2PERIOD > 0xffff
#error "Timer period is too big."
#endif

int mytime = 0x0000; 	//timerns tid
int maxtime = 0x0010; 	//tiden då användaren pausat för länge
int timeoutcount=0;		//interrupträknare för sekunder
int blinkcount = 0; 	//blinkcounter for pause
int blinkcount2 = 0; 	//blinkcounter for running

const uint8_t const font[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 94, 0, 0, 0, 0,
	0, 0, 4, 3, 4, 3, 0, 0,
	0, 36, 126, 36, 36, 126, 36, 0,
	0, 36, 74, 255, 82, 36, 0, 0,
	0, 70, 38, 16, 8, 100, 98, 0,
	0, 52, 74, 74, 52, 32, 80, 0,
	0, 0, 0, 4, 3, 0, 0, 0,
	0, 0, 0, 126, 129, 0, 0, 0,
	0, 0, 0, 129, 126, 0, 0, 0,
	0, 42, 28, 62, 28, 42, 0, 0,
	0, 8, 8, 62, 8, 8, 0, 0,
	0, 0, 0, 128, 96, 0, 0, 0,
	0, 8, 8, 8, 8, 8, 0, 0,
	0, 0, 0, 0, 96, 0, 0, 0,
	0, 64, 32, 16, 8, 4, 2, 0,
	0, 62, 65, 73, 65, 62, 0, 0,
	0, 0, 66, 127, 64, 0, 0, 0,
	0, 0, 98, 81, 73, 70, 0, 0,
	0, 0, 34, 73, 73, 54, 0, 0,
	0, 0, 14, 8, 127, 8, 0, 0,
	0, 0, 35, 69, 69, 57, 0, 0,
	0, 0, 62, 73, 73, 50, 0, 0,
	0, 0, 1, 97, 25, 7, 0, 0,
	0, 0, 54, 73, 73, 54, 0, 0,
	0, 0, 6, 9, 9, 126, 0, 0,
	0, 0, 0, 102, 0, 0, 0, 0,
	0, 0, 128, 102, 0, 0, 0, 0,
	0, 0, 8, 20, 34, 65, 0, 0,
	0, 0, 20, 20, 20, 20, 0, 0,
	0, 0, 65, 34, 20, 8, 0, 0,
	0, 2, 1, 81, 9, 6, 0, 0,
	0, 28, 34, 89, 89, 82, 12, 0,
	0, 0, 126, 9, 9, 126, 0, 0,
	0, 0, 127, 73, 73, 54, 0, 0,
	0, 0, 62, 65, 65, 34, 0, 0,
	0, 0, 127, 65, 65, 62, 0, 0,
	0, 0, 127, 73, 73, 65, 0, 0,
	0, 0, 127, 9, 9, 1, 0, 0,
	0, 0, 62, 65, 81, 50, 0, 0,
	0, 0, 127, 8, 8, 127, 0, 0,
	0, 0, 65, 127, 65, 0, 0, 0,
	0, 0, 32, 64, 64, 63, 0, 0,
	0, 0, 127, 8, 20, 99, 0, 0,
	0, 0, 127, 64, 64, 64, 0, 0,
	0, 127, 2, 4, 2, 127, 0, 0,
	0, 127, 6, 8, 48, 127, 0, 0,
	0, 0, 62, 65, 65, 62, 0, 0,
	0, 0, 127, 9, 9, 6, 0, 0,
	0, 0, 62, 65, 97, 126, 64, 0,
	0, 0, 127, 9, 9, 118, 0, 0,
	0, 0, 38, 73, 73, 50, 0, 0,
	0, 1, 1, 127, 1, 1, 0, 0,
	0, 0, 63, 64, 64, 63, 0, 0,
	0, 31, 32, 64, 32, 31, 0, 0,
	0, 63, 64, 48, 64, 63, 0, 0,
	0, 0, 119, 8, 8, 119, 0, 0,
	0, 3, 4, 120, 4, 3, 0, 0,
	0, 0, 113, 73, 73, 71, 0, 0,
	0, 0, 127, 65, 65, 0, 0, 0,
	0, 2, 4, 8, 16, 32, 64, 0,
	0, 0, 0, 65, 65, 127, 0, 0,
	0, 4, 2, 1, 2, 4, 0, 0,
	0, 64, 64, 64, 64, 64, 64, 0,
	0, 0, 1, 2, 4, 0, 0, 0,
	0, 0, 48, 72, 40, 120, 0, 0,
	0, 0, 127, 72, 72, 48, 0, 0,
	0, 0, 48, 72, 72, 0, 0, 0,
	0, 0, 48, 72, 72, 127, 0, 0,
	0, 0, 48, 88, 88, 16, 0, 0,
	0, 0, 126, 9, 1, 2, 0, 0,
	0, 0, 80, 152, 152, 112, 0, 0,
	0, 0, 127, 8, 8, 112, 0, 0,
	0, 0, 0, 122, 0, 0, 0, 0,
	0, 0, 64, 128, 128, 122, 0, 0,
	0, 0, 127, 16, 40, 72, 0, 0,
	0, 0, 0, 127, 0, 0, 0, 0,
	0, 120, 8, 16, 8, 112, 0, 0,
	0, 0, 120, 8, 8, 112, 0, 0,
	0, 0, 48, 72, 72, 48, 0, 0,
	0, 0, 248, 40, 40, 16, 0, 0,
	0, 0, 16, 40, 40, 248, 0, 0,
	0, 0, 112, 8, 8, 16, 0, 0,
	0, 0, 72, 84, 84, 36, 0, 0,
	0, 0, 8, 60, 72, 32, 0, 0,
	0, 0, 56, 64, 32, 120, 0, 0,
	0, 0, 56, 64, 56, 0, 0, 0,
	0, 56, 64, 32, 64, 56, 0, 0,
	0, 0, 72, 48, 48, 72, 0, 0,
	0, 0, 24, 160, 160, 120, 0, 0,
	0, 0, 100, 84, 84, 76, 0, 0,
	0, 0, 8, 28, 34, 65, 0, 0,
	0, 0, 0, 126, 0, 0, 0, 0,
	0, 0, 65, 34, 28, 8, 0, 0,
	0, 0, 4, 2, 4, 2, 0, 0,
	0, 120, 68, 66, 68, 120, 0, 0,
};

void initiatePorts(){
	//Initierar PORTD som INPUT för SWITCHAR.
	TRISD = 0xffff;
	/* Från labb 3 */
	/* Set up peripheral bus clock */
        /* OSCCONbits.PBDIV = 1; */
    OSCCONCLR = 0x100000; /* clear PBDIV bit 1 */
	OSCCONSET = 0x080000; /* set PBDIV bit 0 */
	
	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;

	//Initierar PORTE som OUTPUT för LEDs
	TRISECLR = 0xFF;

	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9); //sätter en etta på den nioende biten
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;
	
	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);
	
	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	/* SPI2STAT bit SPIROV = 0; */
	SPI2STATCLR = 0x40;
	/* SPI2CON bit CKP = 1; */
    SPI2CONSET = 0x40;
	/* SPI2CON bit MSTEN = 1; */
	SPI2CONSET = 0x20;
	/* SPI2CON bit ON = 1; */
	SPI2CONSET = 0x8000;


  	T2CON = 0x70;
  	PR2 = TMR2PERIOD;

  	/* Start the timer */  
  	T2CONSET = 0x8000; 
	/* Copy-Paste slutar */

  	//Egen kod
	/* INITIERING ADC */
	//Turn off the ADC
	AD1CON1CLR = 0x8000;
	 
	//int channel = 0x8, alltså fjärde biten i byten; 
	//säger åt ADCn att pin 8 är analog (sätter ad1pcfg<8> till 0)
	AD1PCFG = ~(1 << 8);

	TRISB |= 0x100; //A2 finns på PORTB bit 8, sätt A2 som input
	
	//AD1CHS |= (channel << 16); //sets CHOSA = channel
	//AD1CHS |= (channel << 24); //sets CHOSB = channel
	//Sätter CHOSA och CHOSB till 0x8 för bestämma var vi läser ifrån
	AD1CHS = (0x8 << 16 ) | (0x8 << 24);

	/* AD1CON1bits.FORM = 010 , AD1CON1< */
	AD1CON1 |= (0x4 << 8); //sets FORM of AD1CON to 32bit
	AD1CON1 |= (0x7 << 5); //sets SSRC, Sample Clock Source, to 111, 0x7
	AD1CON2 = 0x1; 			//Set VCFG to 0, CSCNA = 0, BUFM = 0, ALTS = 1,  along with everything else
	AD1CON3 |= (0x1 << 15); //Set ADRC to 1 for oscillator as clock source
	//AD1CON3 |= (0xC << 8); //Set TAD to 12 (SAMC)
	
	AD1CON1 |= 0x8000;		//turn on ADC
	AD1CON1 |= (0x1 << 1);	//start Sampling
	AD1CON1 &= ~0x01; 		//clear Done-flag
}

/* Copy-paste från labb 3 börjar */
/* Declare a helper function which is local to this file */
static void num32asc( char * s, int ); 

#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)

/* quicksleep:
   A simple function to create a small delay.
   Very inefficient use of computing resources,
   but very handy in some special cases. */
void quicksleep(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}
uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while(!(SPI2STAT & 1));
	return SPI2BUF;
}
void display_init(void){
  	DISPLAY_CHANGE_TO_COMMAND_MODE; //Clear PortF<4>
	quicksleep(10);
	DISPLAY_ACTIVATE_VDD; //Clear PortF<6>
	quicksleep(1000000);
	
	spi_send_recv(0xAE);
	DISPLAY_ACTIVATE_RESET; //Clear PortG<9>
	quicksleep(10);
	DISPLAY_DO_NOT_RESET; //Set PortG<9>
	quicksleep(10);
	
	spi_send_recv(0x8D);
	spi_send_recv(0x14);
	
	spi_send_recv(0xD9);
	spi_send_recv(0xF1);
	
	DISPLAY_ACTIVATE_VBAT; //Clear PortF<5>
	quicksleep(10000000);
	
	spi_send_recv(0xA1);
	spi_send_recv(0xC8);
	
	spi_send_recv(0xDA);
	spi_send_recv(0x20);
	
	spi_send_recv(0xAF);
}
//Bestämmer begränsningarna för displayen, antal rader och tecken
void display_string(int line, char *s){
	int i;
	if(line < 0 || line >= 4)
		return;
	if(!s)
		return;
	
	for(i = 0; i < 16; i++)
		if(*s) {
			textbuffer[line][i] = *s;
			s++;
		} else
			textbuffer[line][i] = ' ';
}
void display_update(void) {
	int c, i, j, k;
	//int c;
	for(i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;
		spi_send_recv(0x22);
		spi_send_recv(i);
		
		spi_send_recv(0x0);
		spi_send_recv(0x10);
		
		DISPLAY_CHANGE_TO_DATA_MODE;
		
		for(j = 0; j < 16; j++) {
			c = textbuffer[i][j];
			if(c & 0x80)
				continue;
			
			for(k = 0; k < 8; k++)
				spi_send_recv(font[c*8 + k]);
		}
	}
}

/* Copy paste slutar */

int getsw(void){
	int switches = PORTD >> 8;
	switches = switches & 0xf;
	return switches;		
}

// Timer-relaterade funktioner 
// tick hämtad från labb 3 given kod i labb 3 
void tick( unsigned int * timep )
{
  /* Get current value, store locally */
  register unsigned int t = * timep;
  t += 1; /* Increment local copy */
  
  /* If result was not a valid BCD-coded time, adjust now */

  if( (t & 0x0000000f) >= 0x0000000a ) t += 0x00000006;
  if( (t & 0x000000f0) >= 0x00000060 ) t += 0x000000a0;
  /* Seconds are now OK */

  if( (t & 0x00000f00) >= 0x00000a00 ) t += 0x00000600;
  if( (t & 0x0000f000) >= 0x00006000 ) t += 0x0000a000;
  /* Minutes are now OK */

  if( (t & 0x000f0000) >= 0x000a0000 ) t += 0x00060000;
  if( (t & 0x00ff0000) >= 0x00240000 ) t += 0x00dc0000;
  /* Hours are now OK */

  if( (t & 0x0f000000) >= 0x0a000000 ) t += 0x06000000;
  if( (t & 0xf0000000) >= 0xa0000000 ) t = 0;
  /* Days are now OK */

  * timep = t; /* Store new value */
}



/* Huvudprogram */ 
int main() {
	initiatePorts();	//Initierar portarna
	display_init();		//Startar displayen
	display_update();
	

	int sw = getsw();
	for(;;){
		
		AD1CON1 &= ~0x02;				//end sampling, start converting
		while(AD1CON1&0x02);			//wait until we have 10 bits of information
		    							//"wait until acquistition is done"
		while( ! (AD1CON1&0x01) );		//wait for conversion being finished
		 								//aka, wait for Done-bit turning into 1

		AD1CON1 = AD1CON1|0x2; 			//resume sampling
		AD1CON1 &= ~0x01; 				//clear Done-flag
		if(sw & 0x1 == 0x1){

			mytime = 0x0;
			display_string(0, "How you feeling?");
			int mooooist = ADC1BUF0;

			//Vattnad
			if (mooooist <300){
				if(IFS(0) & 0x100){
					blinkcount2++;
					if(blinkcount2 % 10 == 0){
						blinkcount2 = 0;
						PORTE = 0x01;
					}
					else{
						PORTE = 0x0;
					}
					IFS(0) = 0;
				}	
				display_string(1, "I´m Super-moist");
				display_string(2, "Please no Water");
				display_string(3, "Leave me alone");
				display_update();

			}
			//Lagom vattnad
			else if(mooooist > 299 && mooooist < 500){
				if(IFS(0) & 0x100){			//När Interrupt-flaggan från timern IFS(0)<9> är 1. (T2IF)
					blinkcount2++;
					if(blinkcount2 % 7 == 0){
						PORTE = 0x55; 		//ska blinka
					}
					else{
						PORTE = 0x0;
					}
					IFS(0) = 0;
				}
				display_string(1, "I´m lagom");
				display_string(2, "Happy for now");
				display_string(3, "Please dont go");
				display_update();

			}
			//Torr-Supertorr
			else{
				if(IFS(0) & 0x100){ 
					blinkcount2++;
					if(blinkcount2 % 5 == 0){
						PORTE = 0xFF; //ska blinka
					}
					else{
						PORTE = 0x0;
					}
					IFS(0) = 0;
				}

				display_string(1, "I´m DRYY");
				display_string(2, "Water me...");
				display_string(3, "Or I´ll die....");
				display_update();
			}

		}

		else {
			//Om switchen är nere och timern tickar:
			if(IFS(0) & 0x100){
				//Räkna timer, om timer gått för långt, börja lysa med alla LEDS
				if(maxtime <= mytime){
					if (blinkcount % 2 == 1){
						PORTE = 0xFF;
					}
					else{
						PORTE = 0xAA;
					}

				}
				timeoutcount++;
				if((timeoutcount % 10) == 0){
					/*  Konvertera mytime till sträng
						Skriv ut till skärmen
						Ticka klockan */
						timeoutcount = 0;
						blinkcount++;
						time2string(textstring, mytime);
						display_string(1, textstring);
						display_update();
						tick(&mytime);
				}
				else{
					PORTE = 0x0; //Släcker LEDs	
				}
				IFS(0) = 0;

			}

			display_string(0,"Paused, timer:");
			display_string(2, "Start by turning");
			display_string(3, "switch 1 on");
			display_update();
		}
		sw = getsw();
	}
	return 0;
}
