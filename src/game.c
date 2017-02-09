/* game.c
	Written by Johan Ribom and Simon Edström Kawaji.
	Based on the file 'mipslabmain.c' that was written in 2015 by Axel Isaksson, and modified in 2015 by F Lundevall.
*/

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <time.h>
#include <SDL2/SDL.h>
#include "entities/player.h"
#include "stdlib/graphics.h"
#include "stdlib/rng.h"
#include "stdlib/input.h"
#include "stdlib/ui.h"
#include "images/title.h"
#include "level/level.h"
#include "entities/enemies.h"

void main_game_state();
void inventory_game_state();
void draw_inventory(int selected_index);
void reset_game();

const char white_square[7] = {5, 5, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F};
char pointer[34] = {0};
int inventory_position1[] = {42, 2};
int inventory_position2[] = {42, 12};
int inventory_position3[] = {42, 22};
int game_over = 0;

/* Non-Maskable Interrupt; something bad likely happened, so hang */
void _nmi_handler(){for(;;);}
/* This function is called upon reset, before .data and .bss is set up */
void _on_reset(){}


void title_screen(){
	int i;
	for (i = 0; i < 32; i++){
		clearScreen();
		paint_pic(0, 32 - i, roguelet_title);
		update_screen();
		delay_ms(100);
	}	
	buttonPress(0);
	int seed = time(NULL);
	clearScreen();
	print_text(35, 14, "Seed: ");
	print_int(56, 14, seed);
	rng_init(seed);
	update_screen();
	reset_timer();
	buttonPress(1000);
	clearScreen();
}


int main(int argc, char* argv[]){
	int i, j, k, xPos, yPos, game_state, inventory_index;
    SDL_Event event;
    //printf( "Test1\n"); 
     
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0 ) 
    { 
		SDL_Log( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
       return -1;
    }

	initialize_display();

	inventory_index = 0;
	game_state = 0; //inventory(1) or main game(0)
	int button;
	title_screen();
	level_init();
	player_draw_main_ui();
	level_draw();
	update_screen();
    SDL_Log("Entering main loop\n");
	while(!game_over)
	{

		button = buttonPress(0);
		game_state = checkSwitches(1);

		if (game_state == 0){
			main_game_state(button);
			level_update();
		} else {
			inventory_game_state(button, &inventory_index);
		}
		clearScreen();
		if (game_state == 0){
			player_draw_main_ui();
		} else {
			draw_inventory(inventory_index);
		}
		level_draw();
		update_screen();
		delay_ms(100); 								// Delay to avoid double button presses.
	}
	reset_game();
	return 0;
}

void main_game_state(int button){
	if (button == 0)
		button = buttonPress();

	switch(button){
		case 1:
			move_down();
			break;
		case 2:
			move_up();
			break;
		case 3:
			move_right();
			break;
		case 4:
			move_left();
			break;
		default:
			break;
	}

	player_loot();
}

void draw_equipped_item_box(int selected_index){
	if (selected_index == 0){								// Selected index is the one at the absolute top, edge case.
		if(get_active_weapon_index() == 0)
			paint_pic(0, inventory_position1[1], menu_box);
		if(get_active_weapon_index() == 1)
			paint_pic(0, inventory_position2[1], menu_box);
		if(get_active_weapon_index() == 2)
			paint_pic(0, inventory_position3[1], menu_box);
	
	} else if (selected_index == get_inventory_size() - 1){ // Selected index is the one at the absolute bottom, the other edge case.
		if(get_active_weapon_index() == get_inventory_size() - 3)
			paint_pic(0, inventory_position1[1], menu_box);
		if(get_active_weapon_index() == get_inventory_size() - 2)
			paint_pic(0, inventory_position2[1], menu_box);
		if(get_active_weapon_index() == get_inventory_size() - 1)
			paint_pic(0, inventory_position3[1], menu_box);
	
	} else {												// Normal case, selected index is in the middle.
		if(get_active_weapon_index() == selected_index - 1)
			paint_pic(0, inventory_position1[1], menu_box);
		if(get_active_weapon_index() == selected_index)
			paint_pic(0, inventory_position2[1], menu_box);
		if(get_active_weapon_index() == selected_index + 1)
			paint_pic(0, inventory_position3[1], menu_box);
	}
}

void draw_inventory(int selected_index){
	int i;
	int displayed_items[3];

	player_draw_inventory_ui();	
	if (get_inventory_element((int) get_active_weapon_index())){
		draw_equipped_item_box(selected_index);
	}
	if (selected_index == 0){
		paint_pic(2, inventory_position1[1] + 2, menu_dot);
		for (i = 0 ; i < 3 ; i++){
			displayed_items[i] = get_inventory_element(i);
		}
	} else if (selected_index == get_inventory_size() - 1){
		paint_pic(2, inventory_position3[1] + 2, menu_dot);
		for (i = 0 ; i < 3 ; i++){
			displayed_items[i] = get_inventory_element(i + get_inventory_size() - 3);
		}
	} else {
		paint_pic(2, inventory_position2[1] + 2, menu_dot);
		for (i = 0 ; i < 3 ; i++){
			displayed_items[i] = get_inventory_element(selected_index - 1 + i);
		}
	}
	paint_from_items(inventory_position1[0], inventory_position1[1], displayed_items[0]);
	paint_from_items(inventory_position2[0], inventory_position2[1], displayed_items[1]);
	paint_from_items(inventory_position3[0], inventory_position3[1], displayed_items[2]);
}

void inventory_game_state(int button, int* selected_index){
	switch(button){
		case 1:
			if(*selected_index < get_inventory_size() - 1){
				(*selected_index)++;
			}
			break;
		case 2:
			if(*selected_index > 0){	
				(*selected_index)--;
			}
			break;
		case 3:
			drop_item(*selected_index);
			break;
		case 4:
			use_item(*selected_index);
			break;
		default:
			break;
	}

}

void reset_game(){
	game_over = 0;
	reset_level();
	reset_player();
	main(0,0);
}

void gameover(){
	game_over = 1;
}
