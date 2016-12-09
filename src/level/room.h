#pragma once


char seed_has_top_door(char* seed);
char seed_has_left_door(char* seed);
char seed_has_right_door(char* seed);
char seed_has_bottom_door(char* seed);
void move_up();
void move_left();
void move_right();
void move_down();
void room_draw();
void room_init();
char generate_vertical_door(int room_height);
char generate_horizontal_door(int room_width);
void generate_room_seed(char* pointer);
void set_current_room_to_seed(char* seed);
void print_seed();
void print_pointer();
void print_room_info();
int get_corner_x();
int get_corner_y();