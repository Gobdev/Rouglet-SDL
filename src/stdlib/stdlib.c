#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "stdlib.h"

void reset_timer(){
    TMR2 = 0; // Set counter to 0.
    T2CON = 0x8070; // Set timer on (bit 15), prescale 1:256 (bits 6-4), 16-bit counter (bit 3), internal clock (bit 1).
    PR2 = 312; // Counter goes up to 312, 31250 * 256 * 1000 is almost 80 000 000.
}

void delay_ms(int milliseconds){
    TMR2 = 0;
    int i = 0;
    while (i < milliseconds){
        if (IFS(0) & 0x100){
            IFS(0) &= ~0x100; // Reset flag.
            i++;              // Increment time counter every 100 ms.
        }
    }
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
    int valueCopy, i;
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

int strlen(char* string){
    int i;
    for (i = 0; i < 128; i++){
        if (string[i] == 0){
            break;
        }
    }
    return i;
}

int min_int(int first, int second){
    if (first <= second)
        return first;
    return second;
}
