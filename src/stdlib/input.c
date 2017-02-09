#include <stdint.h>
#include <SDL2/SDL.h>

#include "input.h"
#include "graphics.h"


char allowPress[4] = {1,1,1,1}; //Allow a button to send signal
SDL_Event event;


char checkButton(char buttonNumber){
	switch(buttonNumber){
		case 1:
			return 0;
			//bit index 1 in PORTF
		case 2:
			return 0;
			//bit index 5 in PORTD
		case 3:
			return 0;
			//bit index 6 in PORTD
		case 4:
			return 0;
			//bit index 7 in PORTD
	}
}

char pressedButton(char buttonNumber){
	if(checkButton(buttonNumber)){
		if(allowPress[buttonNumber - 1] == 1){
			allowPress[buttonNumber - 1] = 0;
			return 1;
		}else{
			return 0;
		}
	}else{
		allowPress[buttonNumber - 1] = 1;
		return 0;
	}
}

char checkSwitches(char switchNumber){
	switch(switchNumber){
		case 1:
			return 0; //bit index 8 in PORTD
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
}

char check_all_switches(){
	return 0;
}

char get_switch_state(){
	char return_char = 0;
	return_char += checkSwitches(4);
	return_char += checkSwitches(3) << 1;
	return_char += checkSwitches(2) << 2;
	return_char += checkSwitches(1) << 3;
	return return_char;
}

int buttonPress(int timeout){
	int b = 0;
	int i = 0;
	char switch_state = get_switch_state();
	while (!b && (timeout == 0 || i < timeout)){
        SDL_PollEvent(&event);

        if(event.type == SDL_QUIT) exit(0);
        else if(event.type == SDL_KEYDOWN) {
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
                case KMOD_SHIFT:
                    b = 5;
                    break;
            }
        }
        
        SDL_Delay(50);
	}
	return b;
}
