#include <stdint.h>
#include <pic32mx.h>

/*
int getButtons(void){ //bit 1 in port F and bit 5-7 in port D
	return (PORTD & 0xE0) >> 5;
}
*/

/*
int getSwitches(void){ //bit 8-11 in port D
	return (PORTD & 0xF00) >> 8;
}
*/
char checkButton(char buttonNumber){
	switch(buttonNumber){
		case 1:
			return (PORTF & 0x2); //bit index 1 in PORTF
			break;
		case 2:
			return (PORTD & 0x20); //bit index 5 in PORTD
			break;
		case 3:
			return (PORTD & 0x40); //bit index 6 in PORTD
			break;
		case 4:
			return (PORTD & 0x80); //bit index 7 in PORTD
			break;
	}
}

char checkSwitches(char switchNumber){
	switch(switchNumber){
		case 1:
			return (PORTD & 0x80); //bit index 8 in PORTD
			break;
		case 2:
			return (PORTD & 0x100);//bit index 9 in PORTD
			break;
		case 3:
			return (PORTD & 0x200);//bit index 10 in PORTD
			break;
		case 4:
			return (PORTD & 0x400);//bit index 11 in PORTD
			break;	
	}
}