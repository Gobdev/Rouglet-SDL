#pragma once

void delay_ms(int milliseconds);
void char_to_string(char value, char* str);
void int_to_string(int value, char* str);
void int_to_hex_string(unsigned int value, char* str);
int strlen(char* string);
int intlen(int integer);
void concat_3_strings(int max_len, char* result, char* first, char* second, char* third);
int min_int();