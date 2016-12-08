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

int checkSwitches(char switchNumber){
	switch(switchNumber){
		case 1:
			return (PORTD & 0x100); //bit index 8 in PORTD
			break;
		case 2:
			return (PORTD & 0x200);//bit index 9 in PORTD
			break;
		case 3:
			return (PORTD & 0x400);//bit index 10 in PORTD
			break;
		case 4:
			return (PORTD & 0x800);//bit index 11 in PORTD
			break;	
	}
}

int buttonPress(int timeout){
	TMR2 = 0;
	int b = 0;
	reset_timer();
	int i = 0;
	while (!b && (timeout == 0 || i < timeout)){
		if (pressedButton(1))
			b = 1;
		else if (pressedButton(2))
			b = 2;
		else if (pressedButton(3))
			b = 3;
		else if (pressedButton(4))
			b = 4;
		if (IFS(0) & 0x100){
            IFS(0) &= ~0x100; // Reset flag.
            i++;              // Increment time counter every 100 ms.
        }
	}
	return b;
}