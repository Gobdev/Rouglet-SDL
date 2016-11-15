#include <stdint.h>
#include <pic32mx.h>

int getButtons(void){ //bit 4-7 in port D
	return (PORTD & 0xE0) >> 5;
}

int getSwitches(void){ //bit 8-11 in port D
	return (PORTD & 0xF00) >> 8;
}

char checkButton(char buttonNumber){
	switch(buttonNumber){
		case 1:
			return (getBtns() & 0x10);
			break;
		case 2:
			return (getBtns() & 0x20);
			break;
		case 3:
			return (getBtns() & 0x40);
			break;
		case 4:
			return (getBtns() & 0x80);
			break;
	}
}

char checkSwitches(char switchNumber){
	switch(switchNumber){
		case 1:
			return (getSwitches & 0x80);
			break;
		case 2:
			return (getSwitches & 0x100);
			break;
		case 3:
			return (getSwitches & 0x200);
			break;
		case 4:
			return (getSwitches & 0x400);
			break;	
	}
}