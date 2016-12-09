#pragma once

void enable_debug();
void delay_ms(int milliseconds);
void inititalize_display();
void print_debug(int page, char* string);
void print_int(int x, int y, int value);
void print_char(int x, int y, char value);
void print_char_hex(int x, int y, char value);
void print_hex(int x, int y, int value);
void clearScreen();
void paintOnePixel(int x, int y);
void paint_pic(int x, int y, const unsigned char* pic);
void print_text(int x, int y, char* str);
void update_screen();
void paint_from_items(int x, int y, int item_id);
int get_oled_height();
int get_oled_width();
