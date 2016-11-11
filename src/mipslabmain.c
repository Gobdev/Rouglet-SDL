/* mipslabmain.c

   This file written 2015 by Axel Isaksson,
   modified 2015 by F Lundevall

   Latest update 2015-08-28 by F Lundevall

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int main(void) {
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
	display_string(0, "GOBLET GOBLE");
	display_string(1, "T GOBLET GOB");
	display_string(2, "LET GOBLET G");
	display_string(3, "OBLET GOBLET");
	display_update();
	
	display_image(96, icon);
	
	labinit(); /* Do any lab-specific initialization */
	int i, j, k;
	k = 0;
	while( 1 )
	{
		if (++k > 3)
			k = 0;
		clearScreen();
		delayMs(5);
		for (i = 0; i < 128 / 4; i++){
			for (j = 0; j < 32; j++){
				paintOnePixel(i * 4 + k, j);
			}
		}
		updateScreen();
	}
	return 0;
}
