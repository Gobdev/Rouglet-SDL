#include <stdint.h>
#include <SDL2/SDL.h>

#include "input.h"
#include "graphics.h"


char allowPress[4] = {1,1,1,1}; //Allow a button to send signal
int toggle = 0;
char key_down = 0;
SDL_Event event;


char checkSwitches(char switchNumber){
	switch(switchNumber){
		case 1:
			return toggle;
			break;
		case 2:
			return 0;//bit index 9 in PORTD
			break;
		case 3:
			return 0;//bit index 10 in PORTD
			break;
		case 4:
			return 0;//bit index 11 in PORTD
			break;	
	}
	return 0;
}

char check_all_switches(){
	return 0;
}

char get_switch_state(){
	char return_char = 0;
	return_char += checkSwitches(4);
	return_char += checkSwitches(3) << 1;
	return_char += checkSwitches(2) << 2;
	return_char += checkSwitches(1);
	return return_char;
}


int buttonPress(int timeout){
	int b = 0;
	int i = 0;
	char switch_state = get_switch_state();
	while (!b && (timeout == 0 || i < timeout)){
        
		SDL_PollEvent(&event);
		// key_down tracks key state to make sure that
		// key presses are not repeated
		// it only works with one key press at a time
        if(event.type == SDL_QUIT) exit(0);
        else if(event.type == SDL_KEYDOWN && key_down == 0) {
			key_down = 1;
            switch(event.key.keysym.sym)
            {
                case SDLK_DOWN:
                    b = 1;
                    break;
                case SDLK_UP:
                    b = 2;
                    break;
                case SDLK_RIGHT:
                    b = 3;
                    break;
                case SDLK_LEFT:
                    b = 4;
                    break;
                case SDLK_RCTRL:
					toggle = !toggle;
					b = 5;
                    break;
            }
		}
		else if (event.type == SDL_KEYUP) {
			key_down = 0;
		}
        
		//Delay for good measure
		SDL_Delay(50);
		i++;
	}
	return b;
}
