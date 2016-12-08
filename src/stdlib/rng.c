#include "rng.h"

unsigned short current_value;
unsigned short S0;
short S1;

void rng_init(unsigned short input){
    current_value = input;
}

/*
    An rng function based on the function used in Super Mario 64.
    Source for this code can be found here: 'https://www.youtube.com/watch?v=MiuLeTE2MeQ'.
    This function is a bijection from the set of all possible values of an unsigned short to the same set.
    In the cycle of this set used in the function, 65534 return values are possible.
*/
unsigned short rng_function(){
    if (current_value == 0x560A) 
        current_value = 0;
    S0 = (unsigned char) current_value << 8;
    S0 = S0 ^ current_value;
    current_value = ((S0 & 0xFF) << 8) | ((S0 & 0xFF00) >> 8);
    S0 = ((unsigned char) S0 << 1) ^ current_value;
    S1 = (S0 >> 1) ^ 0xFF80;
    if ((S0 & 1) == 0) 
        current_value = S1 ^ 0x1FF4;
    else 
        current_value = S1 ^ 0x8180;
    return (unsigned short) current_value;
}

int get_random_int(int max){
    if (max < 1)
        return 0;
    return ((int) rng_function()) % (max + 1);
}

char get_random_char(char max){
    if (max < 1)
        return 0;
    return ((char) rng_function()) % (max + 1);
}