#include <stdint.h>
#include <pic32mx.h>
#include "ui.h"
/*
void paintUI(){

}
*/
void updateExpBar(int maxExp, int currentExp){
	float expRatio = currentExp/maxExp;
	int LEDcount = (int) expRatio/0.125;	// 1/8 = 0.125 (8 LEDS)
	PORTE = ~(((char) 0xFF) << LEDcount);
}