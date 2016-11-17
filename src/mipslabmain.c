/* mipslabmain.c

   This file written 2015 by Axel Isaksson,
   modified 2015 by F Lundevall

   Latest update 2015-08-28 by F Lundevall

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include "images/character_sprites.c"
#include "images/UI.c"


void paintImage(int x, int y, const char* image){
	paintPic(x, y, image);
}

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
	//display_string(0, "GOBLET GOBLE");
	//display_string(1, "T GOBLET GOB");
	//display_string(2, "LET GOBLET G");
	//display_string(3, "OBLET GOBLET");
	//display_update();
	
	//display_image(96, icon);
	
	//labinit(); /* Do any lab-specific initialization */
	int i, j, k;
	i = 0;
	j = 0;
	k = 0;
	enable_debug();
	while( 1 )
	{
		++i;
		++j;
		if (++k > 3)
			k = 0;
		clearScreen();

		delayMs(2);
		/*printText((i + 0) % 128, (j + 0) % 32);
		printText((i + 64) % 128, (j + 5) % 32);
		printText((i + 20) % 128, (j + 10) % 32);
		printText((i + 84) % 128, (j + 15) % 32);
		printText((i + 40) % 128, (j + 20) % 32);
		printText((i + 104) % 128, (j + 25) % 32);
		printText((i + 60) % 128, (j + 30) % 32);*/
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
		updateScreen();
	}
	return 0;
}
