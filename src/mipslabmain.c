/* mipslabmain.c

   This file written 2015 by Axel Isaksson,
   modified 2015 by F Lundevall

   Latest update 2015-08-28 by F Lundevall

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include "images/character_sprites.c"

const char whitebox[7] = {0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F};
char H[7] = {0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00, 0x00};
char E[7] = {0x7F, 0x49, 0x49, 0x41, 0x41, 0x00, 0x00};
char N[7] = {0x7F, 0x06, 0x18, 0x60, 0x7F, 0x00, 0x00};
char R[7] = {0x7F, 0x09, 0x19, 0x29, 0x46, 0x00, 0x00};
char I[7] = {0x41, 0x41, 0x7F, 0x41, 0x41, 0x00, 0x00};
char K[7] = {0x7F, 0x08, 0x14, 0x22, 0x41, 0x00, 0x00};

void printText(int x, int y){
	paint7x7(x, y, H);
	paint7x7(x + 6, y, E);
	paint7x7(x + 12, y, N);
	paint7x7(x + 18, y, R);
	paint7x7(x + 24, y, I);
	paint7x7(x + 32, y, K);
}

void paintImage(int x, int y, const char* image){
	paint7x7(x, y, image);
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
	int i, j, k,xPos,yPos;
	i = -4;
	j = -4;
	k = 0;
	xPos = 0;
	yPos = 0;
	enable_debug();
	while( 1 )
	{
		++i;
		++j;
		if (++k > 3)
			k = 0;
		clearScreen();
		delayMs(5);
		/*
		print_debug(0, "New");
		print_debug(1, "I am mad scientist.");
		print_debug(2, "Is so cool!");
		print_debug(3, "Sonuvabich?");
		*/

		/*printText((i + 0) % 128, (j + 0) % 32);
		printText((i + 64) % 128, (j + 5) % 32);
		printText((i + 20) % 128, (j + 10) % 32);
		printText((i + 84) % 128, (j + 15) % 32);
		printText((i + 40) % 128, (j + 20) % 32);
		printText((i + 104) % 128, (j + 25) % 32);
		printText((i + 60) % 128, (j + 30) % 32);*/
		/*
		paintImage((i + 0) % 128, (j + 0) % 32, sword);
		paintImage((i + 64) % 128, (j + 5) % 32, shield);
		paintImage((i + 20) % 128, (j + 10) % 32, spooks);
		paintImage((i + 84) % 128, (j + 15) % 32, armor);
		paintImage((i + 40) % 128, (j + 20) % 32, goblet2);
		paintImage((i + 104) % 128, (j + 25) % 32, lootBox);
		paintImage((i + 60) % 128, (j + 30) % 32, goblet1);
		*/
		paintImage(xPos,yPos, smileyMan);

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

		if(pressedButton(4)){
			PORTE |= 0x8;
			xPos -= 7;

		}else{
			PORTE &= 0x7;
		}

		updateScreen();
	}
	return 0;
}
