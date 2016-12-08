/* game.c
	Written by Johan Ribom and Simon Edström Kawaji.
	Based on the file 'mipslabmain.c' that was written in 2015 by Axel Isaksson, and modified in 2015 by F Lundevall.
*/

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "entities/player.h"
#include "stdlib/graphics.h"
#include "stdlib/rng.h"
#include "stdlib/input.h"
#include "stdlib/ui.h"
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

void set_game_state(){

}

int main(void) {
	int i, j, k, xPos, yPos, game_state;
	game_state = 0; //inventory(1) or main game(0)
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
		game_state = checkSwitches(1);
		i += 7;
		j += 7;
		if (++k > 3)
			k = 0;
		clearScreen();

		if (button == 0)
			button = buttonPress();
		switch(button){
			case 1:
				player_moveUp();
				break;
			case 2:
				player_moveDown();
				break;
			case 3:
				player_moveRight();
				break;
			case 4:
				player_moveLeft();
				break;
			default:
				break;
		}
		
		if (exp > 100){
			exp = 0;
			player_level_up();
		}
		
		if(game_state == 0){
			player_draw_main_ui();
		}else{
			player_draw_inventory_ui();
		}
		player_draw();

		updateExpBar(100, exp += 3);
		updateScreen();
		button = 0;
	}
	return 0;
}
