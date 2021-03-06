#pragma once

void delay_ms(int milliseconds);
void char_to_string(char value, char* str);
void int_to_string(int value, char* str);
void int_to_hex_string(unsigned int value, char* str);
int min_int(int first, int second);
char mod(char first, char second);
int intlen(int integer);
void concat_3_strings(int max_len, char* result, char* first, char* second, char* third);
int random_drop_rarity(int n);
int calculate_drop(int weapon_amount);
float pow_custom(float number, int power);
