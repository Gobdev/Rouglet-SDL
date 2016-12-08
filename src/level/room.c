#include <stdlib.h>
#include "room.h"
#include "../entities/player.h"
#include "../stdlib/graphics.h"
#include "../images/walls.h"
#include "../stdlib/rng.h"
#include "../stdlib/stdlib.h"

int max_width = 8;
int max_height = 4;
int width = 5;
int height = 2;
int corner_x = 66;
int corner_y = 2;
int current_corner_x = 66;
int current_corner_y = 2;
int square_size = 7;
char white_box[9] = {7, 7, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F};
char dot_pic[9] = {7, 7, 0x0, 0x0, 0x0, 0x8, 0x0, 0x0, 0x0};


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
    else if (rng > 2)
        return 1;
    return 0;
}

char generate_room_height(){
    int rng = get_random_int(100);
    if (rng > 54)
        return 4;
    else if (rng > 32)
        return 3;
    else if (rng > 5)
        return 2;
    return 1;
}

char generate_room_position(char room_width, char room_height){
    clearScreen();
    char x_pos = (char) get_random_int(max_width - (room_width + 1));
    char y_pos = (char) get_random_int(max_height - room_height);
    print_char(0, 0, x_pos);
    print_char(0, 6, y_pos);
    print_char(6, 0, max_width - room_width);
    print_char(6, 6, max_height - room_height);
    return (x_pos << 4) + y_pos; 
}

char generate_doors(){
    char doors = 0;
    if (get_random_int(100) > 20)
        doors += 8;
    if (get_random_int(100) > 20)
        doors += 4;
    if (get_random_int(100) > 20)
        doors += 2;
    if (get_random_int(100) > 20)
        doors++;
    return doors;
}

char generate_enemies(){
    return 0;
}

void generate_room_seed(char* pointer){
    int i, bytes;
    char room_width, room_height, room_pos;
    room_width = generate_room_width();
    room_height = generate_room_height();
    pointer[0] = (room_width << 4) + room_height;
    pointer[1] = generate_room_position(room_width, room_height);
    print_hex(0, 14, (int) pointer[1]);
    update_screen();
    delay_ms(1000);
    pointer[2] = generate_doors();
    pointer[3] = generate_enemies();
    pointer[4] = generate_enemies();
}

void set_current_room_to_seed(char* pointer){
    width = (int) (pointer[0] >> 4) + 1;
    height = (int) (pointer[0] & 0x0F);
    current_corner_x = 66 + square_size * ((unsigned int) (pointer[1] >> 4));
    current_corner_y = 2 + square_size * ((unsigned int) (pointer[1] & 0x07));
    char i, j;
    j = 0;
    /*for (i = 2; i < get_seed_size(width, height); i++){
        if (pointer[i] & 0xF0 == 0xF0)
            1;    
        j++;
        if (pointer[i] & 0x0F == 0x0F)
            1;
            //set_player_position(current_corner_x + (j % width) * square_size, current_corner_y + (j / height) * square_size);
        j++;
    }*/
    set_player_position(current_corner_x, current_corner_y); 
}

void room_init(char* seed){
    current_corner_x = 66 + square_size * get_random_int(max_width - width);
    current_corner_y = 2 + square_size * get_random_int(max_height - height);
}

void print_seed(char* seed){
    /*int seed_width = (int) (seed[0] >> 4);
    int seed_height = (int) (seed[0] & 0x0F);
    char bytes = get_seed_size(seed_width, seed_height);
    int i;
    print_int(0, 0, seed_width);
    print_int(4, 0, seed_height);
    print_int(8, 0, seed[1]);
    for (i = 2; i < bytes; i++){
        print_int(8 + 4 * i, 0, seed[i]);
    }*/
}

void print_room_info(){
    print_int(52, 0, current_corner_x);
    print_int(52, 6, current_corner_y);
}

void room_draw(){
    int i, j;
    for (i = 0; i < width; i++){
        paint_pic(current_corner_x + i * square_size, current_corner_y - square_size - 1, topWall);
        paint_pic(current_corner_x + i * square_size, current_corner_y + height * square_size + 1, bottomWall);
    }
    for (i = 0; i < height; i++){
        paint_pic(current_corner_x - square_size - 1, current_corner_y + i * square_size, leftWall);
        paint_pic(current_corner_x + width * square_size + 1, current_corner_y + i * square_size, rightWall);
    }
    for (i = 0; i < width; i++){
        for (j = 0; j < height; j++){
            paint_pic(current_corner_x + i * square_size, current_corner_y + j * square_size, dot_pic);
        }
    }
}