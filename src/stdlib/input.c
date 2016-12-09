#include <stdint.h>
#include <pic32mx.h>
#include "input.h"
#include "graphics.h"


char allowPress[4] = {1,1,1,1}; //Allow a button to send signal


char checkButton(char buttonNumber){
	switch(buttonNumber){
		case 1:
			return PORTF & 0x2;
			//bit index 1 in PORTF
		case 2:
			return PORTD & 0x20;
			//bit index 5 in PORTD
		case 3:
			return PORTD & 0x40;
			//bit index 6 in PORTD
		case 4:
			return PORTD & 0x80;
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
			return (PORTD & 0x100) >> 8; //bit index 8 in PORTD
			break;
		case 2:
			return (PORTD & 0x200) >> 9;//bit index 9 in PORTD
			break;
		case 3:
			return (PORTD & 0x400) >> 10;//bit index 10 in PORTD
			break;
		case 4:
			return (PORTD & 0x800) >> 11;//bit index 11 in PORTD
			break;	
	}
}

char check_all_switches(){
	if ((PORTD & 0x200) >> 9) //bit index 9 in PORTD
		return 2;
	else if ((PORTD & 0x100) >> 8) //bit index 8 in PORTD
		return 1;
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
	TMR2 = 0;
	int b = 0;
	reset_timer();
	int i = 0;
	char switch_state = get_switch_state();
	while (!b && (timeout == 0 || i < timeout)){
		if (pressedButton(1))
			b = 1;
		else if (pressedButton(2))
			b = 2;
		else if (pressedButton(3))
			b = 3;
		else if (pressedButton(4))
			b = 4;
		else if (get_switch_state() != switch_state)
			b = 5;
		if (IFS(0) & 0x100){
            IFS(0) &= ~0x100; // Reset flag.
            i++;              // Increment time counter every 100 ms.
        }
	}
	return b;
}