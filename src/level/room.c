#include <stdlib.h>
#include "room.h"
#include "level.h"
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

char seed_has_top_door(char* seed){
    return (seed[2] >> 3) > 0;
}

char seed_has_left_door(char* seed){
    return (seed[2] & 0x7) > 0;
}

char seed_has_right_door(char* seed){
    return (seed[3] & 0x7) > 0;
}

char seed_has_bottom_door(char* seed){
    return (seed[3] >> 3) > 0;
}

int square_busy(int x, int y){
    int i;
    if (x == player_get_x() && y == player_get_y())
        return 1;
    for (i = 0; i < get_number_of_enemies(); i++){
        if (enemy_is_enabled(i) && x == enemy_get_x(i) && y == enemy_get_y(i))
            return 1;
    } 
    return 0;
}

void move_up(){
    char enemy_hit = enemy_on_square(player_get_x(), player_get_y() - 1);
    if (player_get_y() > 0 && !enemy_hit)
        player_move_up();
    else if (enemy_hit){
        player_damage_enemy(enemy_hit - 1);
        enemy_damage_player(enemy_hit - 1);
    }
    else if (top_door && player_get_x() == top_door - 1){
        enter_top_door();
    }
}

void move_left(){
    char enemy_hit = enemy_on_square(player_get_x() - 1, player_get_y());
    if (player_get_x() > 0 && !enemy_hit)
        player_move_left();
    else if (enemy_hit){
        player_damage_enemy(enemy_hit - 1);
        enemy_damage_player(enemy_hit - 1);
    }
    else if (left_door && player_get_y() == left_door - 1){
        enter_left_door();
    }
}

void move_right(){
    char enemy_hit = enemy_on_square(player_get_x() + 1, player_get_y());
    if (player_get_x() < width - 1 && !enemy_hit)
        player_move_right();
    else if (enemy_hit){
        player_damage_enemy(enemy_hit - 1);
        enemy_damage_player(enemy_hit - 1);
    }
    else if (right_door && player_get_y() == right_door - 1){
        enter_right_door();
    }
}

void move_down(){
    char enemy_hit = enemy_on_square(player_get_x(), player_get_y() + 1);
    if (player_get_y() < height - 1 && !enemy_hit)
        player_move_down();
    else if (enemy_hit){
        player_damage_enemy(enemy_hit - 1);
        enemy_damage_player(enemy_hit - 1);
    }
    else if (bottom_door && player_get_x() == bottom_door - 1){
        enter_bottom_door();
    }
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

char generate_vertical_door(int room_height){
    return (char) (get_random_int(room_height - 2) + 1);
}

char generate_horizontal_door(int room_width){
    return (char) (get_random_int(room_width - 1) + 1);
}

char generate_doors(int room_width, int room_height){
    char doors = 0;
    if (get_random_int(100) < 80)
        doors += generate_vertical_door(room_height);
    if (get_random_int(100) < 80)
        doors += (generate_horizontal_door(room_width) << 3);
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

void spawn_enemy(int enemy_number, char enemy_type){
    int x, y;
    x = get_random_int(width - 1);
    y = get_random_int(height - 1);
    int failsafe = 0;
    while(square_busy(x, y)){
        x = get_random_int(width - 1);
        y = get_random_int(height - 1);
        if (failsafe++ > 100){
            return;
        }
    }
    init_enemy(enemy_number, enemy_type, x, y);
}

void spawn_enemies(char* pointer){
    char first_enemy = (pointer[4] >> 4) & 0x0F;
    char second_enemy = pointer[4] & 0x0F;
    char third_enemy = (pointer[5] >> 4) & 0x0F;
    char fourth_enemy = pointer[5] & 0x0F;
    if (first_enemy)
        spawn_enemy(0, first_enemy - 1);
    if (second_enemy){
        spawn_enemy(1, second_enemy - 1);
    }
    if (third_enemy)
        spawn_enemy(2, third_enemy - 1);
    if (fourth_enemy && height * width > 4)
        spawn_enemy(3, fourth_enemy - 1);
}

void save_current_room_to_seed(){
    if (enemy_is_enabled(0)){
        current_seed[4] &= 0x0F;
        current_seed[4] |= get_enemy_seed(0) << 4;
    }
    if (enemy_is_enabled(1)){
        current_seed[4] &= 0xF0; 
        current_seed[4] |= get_enemy_seed(1);     
    }
    if (enemy_is_enabled(2)){
        current_seed[5] &= 0x0F;
        current_seed[5] |= get_enemy_seed(0) << 4;
    }
    if (enemy_is_enabled(3)){
        current_seed[5] &= 0xF0; 
        current_seed[5] |= get_enemy_seed(1);     
    }
    clear_enemies();
}

void set_current_room_to_seed(char* pointer){
    save_current_room_to_seed();
    current_seed = pointer;
    width = (int) (pointer[0] >> 4) + 1;
    height = (int) (pointer[0] & 0x0F);
    current_corner_x = 66 + square_size * ((unsigned int) (pointer[1] >> 4));
    current_corner_y = 2 + square_size * ((unsigned int) (pointer[1] & 0x07));
    top_door = current_seed[2] >> 3;
    left_door = current_seed[2] & 0x7;
    bottom_door = current_seed[3] >> 3;
    right_door = current_seed[3] & 0x7;
    spawn_enemies(pointer);
}

void room_init(char* seed){
    current_corner_x = 66 + square_size * get_random_int(max_width - width);
    current_corner_y = 2 + square_size * get_random_int(max_height - height);
}

void room_update(){
    
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
    for (i = 0; i < get_number_of_enemies(); i++){ 
        enemy_draw(i, current_corner_x + enemy_get_x(i) * square_size, current_corner_y + enemy_get_y() * square_size);
    }
    player_draw(current_corner_x + player_get_x() * square_size, current_corner_y + player_get_y() * square_size);
}