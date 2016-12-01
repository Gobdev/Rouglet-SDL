/* mipslabmain.c

   This file written 2015 by Axel Isaksson,
   modified 2015 by F Lundevall

   Latest update 2015-08-28 by F Lundevall

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include "graphics/graphics.h"
#include "graphics/images/character_sprites.h"
#include "graphics/images/UI.h"
#include "rng/rng.h"

/*
	The three functions below were copied directly from the file "stubs.c" used in the labs.
*/
/* Non-Maskable Interrupt; something bad likely happened, so hang */
void _nmi_handler(){for(;;);}
/* This function is called upon reset, before .data and .bss is set up */
void _on_reset(){}
/* This function is called before main() is called, you can do setup here */
void _on_bootstrap(){
	/* Set up peripheral bus clock */
    /* OSCCONbits.PBDIV = 1; */
    OSCCONCLR = 0x100000; /* clear PBDIV bit 1 */
	OSCCONSET = 0x080000; /* set PBDIV bit 0 */
	
	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;
	
	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;
	
	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);

	inititalize_display();
	enable_debug();
	rng_init(0);
}

void paintImage(int x, int y, const char* image){
	paintPic(x, y, image);
}

int main(void) {
	int i, j, k, xPos, yPos;
	i = -4;
	j = -4;
	k = 0;
	xPos = 105;
	yPos = 16;
	while( 1 )
	{
		++i;
		++j;
		if (++k > 3)
			k = 0;
		clearScreen();

		delayMs(5);
		paintImage(0, 0, ui1);
		printText(10, 2, "135-238");
		printText(10, 10, "45/60");
		printText(10, 18, "80");
		printText(25, 26, "3");
		paintImage((i + 0) % 142 - 7, (j + 0) % 46 - 7, goblet2);
		paintImage((i + 64) % 142 - 7, (j + 5) % 46 - 7, goblet2);
		paintImage((i + 20) % 142 - 7, (j + 10) % 46 - 7, goblet2);
		paintImage((i + 84) % 142 - 7, (j + 15) % 46 - 7, goblet2);
		paintImage((i + 40) % 142 - 7, (j + 20) % 46 - 7, goblet2);
		paintImage((i + 104) % 142 - 7, (j + 25) % 46 - 7, goblet2);
		paintImage((i + 60) % 142 - 7, (j + 30) % 46 - 7, goblet2);
		paintImage(xPos, yPos, smileyMan);
		
		//print_int(rng_function());

		if(pressedButton(4)){
			PORTE |= 0x8;
			xPos -= 7;

		}else{
			PORTE &= 0x7;
		}
		if(pressedButton(1)){
			PORTE |= 0x1;
			yPos += 7;
		}else{
			PORTE &= 0xE;
		}

		if(pressedButton(2)){
			PORTE |= 0x2;
			yPos -= 7;

		}else{
			PORTE &= 0xD;
		}

		if(pressedButton(3)){
			PORTE |= 0x4;
			xPos += 7;

		}else{
			PORTE &= 0xB;
		}

		updateScreen();
	}
	return 0;
}
