/* mipslabwork.c

   This file written 2015 by F Lundevall

   This file should be changed by YOU! So add something here:

   This file modified 2015-12-24 by Ture Teknolog 

   Latest update 2015-08-28 by F Lundevall

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

#define TMR2PERIOD ((80000000 / 256) / 10)
#if TMR2PERIOD > 0xffff
#error "Timer period is too big."
#endif

int timeoutcount=0;
int prime = 1234567;
int mytime = 0x5957;
char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{

  //Surprise Assignment
  // När SW2 ändrar status - få klockan att ticka 3 sekunder
  // Ändra init oh user_isr
  if(IFS(0) & 0x800){
    tick( &mytime );
    tick( &mytime );
    tick( &mytime );  
  }
  //Vid varje timeout, alltså 10-dels sekund, lägg till 1 till timeoutcount
  if(IFS(0) & 0x100){
    timeoutcount++;
    IFS(0) = 0;
  }
  //Då 1000 ms gått, ticka klockan och återställ timeoutcountern samt uppdatera displayen.
  if(timeoutcount == 10){
    time2string( textstring, mytime );
    display_string( 3, textstring );
    display_update();
    tick( &mytime );
    timeoutcount = 0;

    //Få lysdioder att blinka.
    volatile int * porte = (volatile int *) 0xbf886110;
    *porte += 0x1;
  }
}

/* Lab-specific initialization goes here */
void labinit( void )
{
  //0x100 initierar endast timern
  IEC(0) = 0x900; //Enables both timer 2 and External interrupt 2
  IPC(2) = 4;

  volatile int * trise = (volatile int *) 0xbf886100;
  *trise = *trise & 0xfff0;
  

  TRISD = 0xf80f;
  TRISDSET = (0x7f << 5);  


  T2CON = 0x70;
  PR2 = TMR2PERIOD;
  TMR2 = 0;
  T2CONSET = 0x8000; /* Start the timer */  
  enable_interrupt();
}

/* This function is called repetitively from the main program */
void labwork( void )
{
  prime = nextprime( prime );
  display_string( 0, itoaconv( prime ) );
  display_update();
}
