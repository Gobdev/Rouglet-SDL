/* game.c
	Written by Johan Ribom and Simon Edström Kawaji.
	Based on the file 'mipslabmain.c' that was written in 2015 by Axel Isaksson, and modified in 2015 by F Lundevall.
*/

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "stdlib/graphics.h"
#include "stdlib/rng.h"
#include "stdlib/input.h"
#include "stdlib/ui.h"
#include "images/character_sprites.h"
#include "images/UI.h"
#include "images/title.h"

/* Non-Maskable Interrupt; something bad likely happened, so hang */
void _nmi_handler(){for(;;);}
/* This function is called upon reset, before .data and .bss is set up */
void _on_reset(){}

/*
	Setup function, run before main() function.	
*/
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

	TRISE = 0;


    TMR2 = 0; // Set counter to 0.
    T2CON = 0x8000; // Set timer on (bit 15), prescale 1:256 (bits 6-4), 16-bit counter (bit 3), internal clock (bit 1).
    PR2 = 65535; // Counter goes up to 31250, since 31250 * 256 * 10 = 80 000 000.

	inititalize_display();
	enable_debug();
	rng_init(0);
}

void title_screen(){
	paint_pic(0, 0, roguelet_title);
	updateScreen();
	buttonPress();
	clearScreen();
	int seed = TMR2;
	print_text(35, 14, "Seed: ");
	print_int(56, 14, seed);
	rng_init(seed);
	updateScreen();
	delay_ms(300);
	clearScreen();
	updateScreen();
}

int main(void) {
	int i, j, k, xPos, yPos;
	i = -4;
	j = -4;
	k = 0;
	xPos = 105;
	yPos = 16;
	int exp = 0;
	int button = 5;
	int level = 0;
	title_screen();
	while( 1 )
	{
		i += 7;
		j += 7;
		if (++k > 3)
			k = 0;
		clearScreen();

		if (button == 0)
			button = buttonPress();
		switch(button){
			case 1:
				yPos += 7;
				break;
			case 2:
				yPos -= 7;
				break;
			case 3:
				xPos += 7;
				break;
			case 4:
				xPos -= 7;
				break;
			default:
				break;
		}
		
		paint_pic(0, 0, ui1);
		/*print_text(10, 2, "135-238");
		print_text(10, 10, "45/60");
		print_text(10, 18, "80");
		print_text(25, 26, "3");*/
		paint_pic((i + 0) % 142 - 7, (j + 0) % 46 - 7, goblet2);
		paint_pic((i + 64) % 142 - 7, (j + 5) % 46 - 7, goblet2);
		paint_pic((i + 20) % 142 - 7, (j + 10) % 46 - 7, goblet2);
		paint_pic((i + 84) % 142 - 7, (j + 15) % 46 - 7, goblet2);
		paint_pic((i + 40) % 142 - 7, (j + 20) % 46 - 7, goblet2);
		paint_pic((i + 104) % 142 - 7, (j + 25) % 46 - 7, goblet2);
		paint_pic((i + 60) % 142 - 7, (j + 30) % 46 - 7, goblet2);
		
		paint_pic(xPos, yPos, smileyMan);

		if (exp > 100){
			exp = 0;
			level++;
		}
		updateExpBar(100, exp += 3);
		printLevel(level);
		print_int(10, 2, rng_function());
		updateScreen();
		button = 0;
	}
	return 0;
}
