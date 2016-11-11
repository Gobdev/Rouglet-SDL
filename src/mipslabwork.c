/* mipslabwork.c

   This file written 2015 by F Lundevall

   This file should be changed by YOU! So add something here:

   This file modified 2015-12-24 by Ture Teknolog 

   Latest update 2015-08-28 by F Lundevall

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int mytime = 0x5957;
int timeoutcount = 0;
int prime = 1234567;

char textstring[] = "text, more text, and even more text!";

volatile char* trisE = (volatile char *) 0xbf886100;
volatile char* portE = (volatile char *) 0xbf886110;


/* Interrupt Service Routine */
void user_isr( void )
{
	// Timer 2 interrupt.
	if (IFS(0) & 0x100){
		IFS(0) &= ~0x100;
		timeoutcount++;
		if (timeoutcount >= 10){
			timeoutcount = 0;
			time2string( textstring, mytime );
			display_string( 3, textstring );
			display_update();
			tick( &mytime );
		}
	}

	// Switch 3 interrupt.
	if (IFS(0) & 0x8000){
		IFS(0) &= ~0x8000;
		++*portE;
	}
}

/* Lab-specific initialization goes here */
void labinit( void )
{
	*trisE = 0;
	*portE = 0;
	TRISD &= (~0xFE0);
	TRISD |= 0xFE0; 

	// Initialize Timer 2
	TMR2 = 0; // Set counter to 0.
	T2CON = 0x8070;	// Set timer on (bit 15), prescale 1:64 (bits 6-4), 16-bit counter (bit 3), internal clock (bit 1).
	PR2 = 31250; // Counter goes up to 31250, since 31250 * 64 * 10 = 80 000 000.

	// Initialize interrupts for Timer 2.
	IFS(0) &= ~0x100; // Set interrupt flag to 0.
	IEC(0) |= 0x100; // Enable interrupt for timer 2.
	IPC(2) |= 0x1F;	// Set highest priority (7 and 3).

	// Initialize interrupts for Switch 3.
	INTCON |= 0x8; // Rising edge priority (upwards = interrupt).
	IPC(3) |= 0x1E00; // Set priority 7:2.*/
	IFS(0) &= ~0x8000; // Set interrupt flag to 0.
	IEC(0) |= 0x8000; // Enable interrupt for switch 3.

	//enable_interrupt(); // Enable interrupts globally.
}


/* This function is called repetitively from the main program */
void labwork( void ) {
	prime = nextprime( prime );
	display_string( 0, itoaconv( prime ) );
	display_update();
}
