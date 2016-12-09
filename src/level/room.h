#pragma once

void room_draw();
void room_init();
void generate_room_seed(char* pointer);
void set_current_room_to_seed(char* seed);
void print_seed();
void print_pointer();
void print_room_info();
int get_corner_x();
int get_corner_y();