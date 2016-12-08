#include <stdlib.h>
#include "room.h"
#include "../entities/player.h"
#include "../stdlib/graphics.h"
#include "../images/walls.h"
#include "../stdlib/rng.h"
#include "../stdlib/stdlib.h"


const int max_width = 8;
const int max_height = 4;
const int corner_x = 66;
const int corner_y = 2;
int width;
int height;
int current_corner_x;
int current_corner_y;

char* current_seed;

char top_door;
char left_door;
char right_door;
char bottom_door;

const int square_size = 7;
const char octothorpe[7] = {5, 5, 0xA, 0x1F, 0xA, 0x1F, 0xA};
const char dot_pic[9] = {7, 7, 0x0, 0x0, 0x0, 0x8, 0x0, 0x0, 0x0};


int get_corner_x(){
    return current_corner_x;
}

int get_corner_y(){
    return current_corner_y;
}

char generate_room_width(){
    int rng = get_random_int(100);
    if (rng > 74)
        return 7;
    else if (rng > 59)
        return 6;
    else if (rng > 44)
        return 5;
    else if (rng > 24)
        return 4;
    else if (rng > 13)
        return 3;
    else if (rng > 6)
        return 2;
    return 1;
}

char generate_room_height(){
    int rng = get_random_int(100);
    if (rng > 54)
        return 4;
    else if (rng > 24)
        return 3;
    return 2;
}

char generate_room_position(char room_width, char room_height){
    char x_pos = (char) get_random_int(max_width - (room_width + 1));
    char y_pos = (char) get_random_int(max_height - room_height);
    return (x_pos << 4) + y_pos; 
}

char generate_doors(int room_width, int room_height){
    char doors = 0;
    if (get_random_int(100) < 80)
        doors += (char) ((get_random_int(room_width - 1) + 1) << 3);
    if (get_random_int(100) < 80)
        doors += (char) (get_random_int(room_height - 1) + 1);
    return doors;
}

char get_random_enemy(){
    return (char) get_random_int(14) + 1;
}

char generate_enemies(char* pointer){
    int number_of_enemies = get_random_int(4);
    for (; number_of_enemies > 1; number_of_enemies -= 2){
        *pointer = (get_random_enemy() << 4) + get_random_enemy();
        pointer++;    
    }
    if (number_of_enemies == 1)
        *pointer = get_random_enemy() << 4;
}

void generate_room_seed(char* pointer){
    int i;
    for (i = 0; i < 6; i++)
        pointer[i] = 0;
    char room_width, room_height, room_pos;
    room_width = generate_room_width();
    room_height = generate_room_height();
    pointer[0] = (room_width << 4) + room_height;
    pointer[1] = generate_room_position(room_width, room_height);
    pointer[2] = generate_doors(room_width, room_height);
    pointer[3] = generate_doors(room_width, room_height);
    generate_enemies(pointer + 4);
}

void set_current_room_to_seed(char* pointer){
    current_seed = pointer;
    width = (int) (pointer[0] >> 4) + 1;
    height = (int) (pointer[0] & 0x0F);
    current_corner_x = 66 + square_size * ((unsigned int) (pointer[1] >> 4));
    current_corner_y = 2 + square_size * ((unsigned int) (pointer[1] & 0x07));
    top_door = current_seed[2] >> 3;
    left_door = current_seed[2] & 0x7;
    bottom_door = current_seed[3] >> 3;
    right_door = current_seed[3] & 0x7;
    set_player_position(current_corner_x, current_corner_y); 
}

void room_init(char* seed){
    current_corner_x = 66 + square_size * get_random_int(max_width - width);
    current_corner_y = 2 + square_size * get_random_int(max_height - height);
}

void print_seed(){
    print_char_hex(42, 0, current_seed[1]);
    print_char_hex(42, 6, current_seed[2]);
    print_char_hex(42, 12, current_seed[3]);
    print_char_hex(42, 18, current_seed[4]);
    print_char_hex(42, 24, current_seed[5]);
}

void print_room_info(){
    print_char(46, 0, current_seed[2] >> 3);
    print_char(46, 6, current_seed[2] & 0x7);
    print_char(46, 12, current_seed[3] >> 3);
    print_char(46, 18, current_seed[3] & 0x7);
}

void paint_top_wall(){
    int i;
    for (i = 0; i < width; i++){
        if (!(i == top_door - 1))
            paint_pic(current_corner_x + i * square_size, current_corner_y - square_size - 1, topWall);
    }
    if (top_door){
        int door_location = (top_door - 1) * square_size;
        for (i = current_corner_y - 1 - octothorpe[0]; i > - octothorpe[0]; i -= square_size)
            paint_pic(current_corner_x + door_location + 1, i, octothorpe);
    }
}

void paint_left_wall(){
    int i;
    for (i = 0; i < height; i++){
        if (!(i == left_door - 1))
            paint_pic(current_corner_x - square_size - 1, current_corner_y + i * square_size, leftWall);
    }
    if (left_door){
        int door_location = (left_door - 1) * square_size;
        for (i = current_corner_x - 1 - octothorpe[0]; i > corner_x - octothorpe[0] * 2; i -= square_size)
            paint_pic(i, current_corner_y + door_location + 1, octothorpe);
    }
}

void paint_right_wall(){
    int i;
    for (i = 0; i < height; i++){
        if (!(i == right_door - 1))
            paint_pic(current_corner_x + width * square_size + 1, current_corner_y + i * square_size, rightWall);
    }
    if (right_door){
        int door_location = (right_door - 1) * square_size;
        for (i = current_corner_x + 1 + square_size * width; i < get_oled_width(); i += square_size)
            paint_pic(i, current_corner_y + door_location + 1, octothorpe);
    }
}

void paint_bottom_wall(){
    int i;
    for (i = 0; i < width; i++){
        if (!(i == bottom_door - 1))
            paint_pic(current_corner_x + i * square_size, current_corner_y + height * square_size + 1, bottomWall);
    }
    if (bottom_door){
        int door_location = (bottom_door - 1) * square_size;
        for (i = current_corner_y + 1 + square_size * height; i < get_oled_height(); i += square_size)
            paint_pic(current_corner_x + door_location + 1, i, octothorpe);
    }
}

void room_draw(){
    int i, j;
    paint_top_wall();
    paint_left_wall();
    paint_right_wall();
    paint_bottom_wall();
    for (i = 0; i < width; i++){
        for (j = 0; j < height; j++){
            paint_pic(current_corner_x + i * square_size, current_corner_y + j * square_size, dot_pic);
        }
    }
}