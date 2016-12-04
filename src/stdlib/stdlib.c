void intToString(int value, char* str){
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

int strlen(char* string){
    int i;
    for (i = 0; i < 128; i++){
        if (string[i] == 0){
            break;
        }
    }
    return i;
}
