#include <SDL2/SDL.h>
#include <stdlib.h>
#include "../level/level.h"
#include "lib.h"
#include "rng.h"


char mod(char first, char second){
    char return_char = first % second;
    if (return_char < 0){
        return_char += second;
    }
	return return_char;
}

void reset_timer(){
    
}

void delay_ms(int milliseconds){
    SDL_Delay(milliseconds); 
}


void char_to_string(char value, char* str){
    char valueCopy; 
    int i;
    i = 0;
    if (value < 0){
        str[0] = 0x2D;                  // Negative sign  
        str++;
        value = ~value + 1;             // For negative numbers
    }
    for (valueCopy = value; valueCopy >= 10; valueCopy /= 10){
        i++;
    }
    for (;i >= 0; i--){
        str[i] = (value % 10) + 0x30;
        value /= 10;
    }
}

void int_to_string(int value, char* str){
	str = itoa(value, str, 10);
}

void int_to_hex_string(unsigned int value, char* str){
    unsigned int valueCopy;
    int i;
    i = 0;
    for (valueCopy = value; valueCopy >= 16; valueCopy /= 16){
        i++;
    }
    for (;i >= 0; i--){
        if (value % 16 < 10)
            str[i] = (value % 16) + 0x30;
        else
            str[i] = (value % 16) + 0x37;
        value /= 16;
    }
}


int intlen(int integer){
    int i = 1;
    if (integer < 0){
        i++;
        integer = ~integer + 1;
    }
    while(integer > 9){
        integer /= 10;
        i++;
    }
    return i;
}

int min_int(int first, int second){
    if (first <= second)
        return first;
    return second;
}



void concat_3_strings(int max_len, char* result, char* first, char* second, char* third){
    int i, j, k;
    for (i = 0; i < first[i] && i < max_len - 1; i++)
        result[i] = first[i];
    for (j = 0; i < second[j] && i < max_len - 1; i++)
        result[i] = second[j++];
    for (k = 0; i < third[k] && i < max_len - 1; i++)
        result[i] = third[k++];
    result[i] = 0;
}

float pow_custom(float number, int power){
    int i;
    float original_number = number;
    for (i = 0; i < power - 1; i++){
        number = number * original_number;
    }
    return number;
}

float get_room_average(){
    return (get_current_room_x() + get_current_room_y()) / 2.0f;
}

int inner_formel(int x){
    return (int) 200 * pow_custom((0.8f + (float) get_room_average() / 25 * 0.5f), x);
}

int random_drop_rarity(int n){
    int sum = 0;
    int i;
    for (i = 0; i <= n; i++){
        sum += inner_formel(i);
    }
    int r = get_random_int(sum);
    sum = 0;
    for (i = 0; i < n; i++){
        sum += inner_formel(i);
        if (r < sum)
            return i;
    }
    return i;
}

int calculate_drop(int weapon_amount){
    int random = get_random_int(100);    
    if (random > 70){
        int random_type = get_random_int(100);
        if (random_type > 70){
            return random_drop_rarity(weapon_amount - 1) + 1;
        } else {
            return 15 + get_random_int(4);
        }
    } else {
        return 0;
    }
}
