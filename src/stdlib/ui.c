#include <stdint.h>
#include <pic32mx.h>
#include "ui.h"
#include "graphics.h"
#include "../entities/player.h"

void paintInventory(){

}

char level_str[15] = {0};

void updateExpBar(int maxExp, int currentExp){
	float expRatio = (float)currentExp/(float)maxExp;
	int LEDcount = (int) (expRatio * 8);	// 1/8 = 0.125 (8 LEDS)
	//PORTESET = 0xFF;
    PORTE = ~(((char) 0xFF) << LEDcount);
}

void printLevel(int level){
    print_int(25, 26, level);
}