#include "level.h"
#include "room.h"
#include "../entities/enemies.h"
#include "../entities/player.h"
#include "../stdlib/graphics.h"

const int seed_size = 6;
const int level_length = 25;
int current_room[2] = {0, 0};
int right = 0;
char room_seeds[6 * 25 * 25] = {0};

char* get_current_room_pointer();

char get_room_width_from_seed(char* seed){
    return (int) (seed[0] >> 4) + 1;
}

int get_current_room_average(){
    return (current_room[0] + current_room[1]) / 2;
}

char get_room_height_from_seed(char* seed){
    return (int) (seed[0] & 0x0F);
}

void level_draw(){
    room_draw();
}

void level_update(){
    room_update();
}

void print_pointer(){
    char* pointer = get_current_room_pointer();
    print_char_hex(42, 0, pointer[1]);
    print_char_hex(42, 6, pointer[2]);
    print_char_hex(42, 12, pointer[3]);
    print_char_hex(42, 18, pointer[4]);
    print_char_hex(42, 24, pointer[5]);
}

int get_current_room_x(){
    return current_room[0];
}

int get_current_room_y(){
    return current_room[1];
}

char* get_room_pointer(int x, int y){
    return room_seeds + seed_size * (x + y * level_length);
}

char* get_current_room_pointer(){
    return get_room_pointer(current_room[0], current_room[1]);
}

char room_exists(int x, int y){
    return x >= 0 && x < level_length && y >= 0 && y < level_length;
}

int room_set(int x, int y){
    return get_room_pointer(x, y)[0] > 0;
}

void level_init(){
    char* pointer = get_current_room_pointer();
    generate_room_seed(pointer);
    int room_width = get_room_width_from_seed(pointer);
    int room_height = get_room_height_from_seed(pointer);
    pointer[2] &= 0;
    //pointer[2] |= (generate_horizontal_door(room_width) << 3) + generate_vertical_door(room_height);
    pointer[3] &= 0;
    pointer[3] |= (generate_horizontal_door(room_width) << 3) + generate_vertical_door(room_height);  
    pointer[4] = 0x01;
    pointer[5] = 0x00;
    set_current_room_to_seed(pointer);
}

void fix_current_room_doors(){
    char* pointer = get_current_room_pointer();
    int room_height = get_room_height_from_seed(pointer);
    int room_width = get_room_width_from_seed(pointer);

    if (room_exists(current_room[0] - 1, current_room[1])){
        if (seed_has_right_door(get_room_pointer(current_room[0] - 1, current_room[1]))){
            pointer[2] &= 0xF8;
            pointer[2] |= generate_vertical_door(room_height);
        } else if (room_set(current_room[0] - 1, current_room[1])) {
            pointer[2] &= 0xF8;
        }
    } else {
        pointer[2] &= 0xF8;
    }

    if (room_exists(current_room[0] + 1, current_room[1])){
        if (seed_has_left_door(get_room_pointer(current_room[0] + 1, current_room[1]))){
            pointer[3] &= 0xF8;
            pointer[3] |= generate_vertical_door(room_height);
        } else if (room_set(current_room[0] + 1, current_room[1])) {
            pointer[3] &= 0xF8;
        }
    } else {
        pointer[3] &= 0xF8;
    }

    if (room_exists(current_room[0], current_room[1] - 1)){
        if (seed_has_bottom_door(get_room_pointer(current_room[0], current_room[1] - 1))){
            pointer[2] &= 0x07;
            pointer[2] |= (generate_horizontal_door(room_width) << 3);
        } else if (room_set(current_room[0], current_room[1] - 1)) {
            pointer[2] &= 0x07;
        }
    } else {
        pointer[2] &= 0x07;
    }

    if (room_exists(current_room[0], current_room[1] + 1)){
        if (seed_has_top_door(get_room_pointer(current_room[0], current_room[1] + 1))){
            pointer[3] &= 0x07;
            pointer[3] |= (generate_horizontal_door(room_width) << 3);
        } else if (room_set(current_room[0], current_room[1] + 1)) {
            pointer[3] &= 0x07;
        }
    } else {
        pointer[3] &= 0x07;
    }
}


void enter_top_door(){
    current_room[1]--;
    char* pointer = get_current_room_pointer();
    if (!pointer[0]){
        generate_room_seed(pointer);
        fix_current_room_doors();
    }
    int room_height = get_room_height_from_seed(pointer);
    int door_pos = pointer[3] >> 3;
    set_player_position(door_pos - 1, room_height - 1);
    set_current_room_to_seed(pointer);
}

void enter_left_door(){
    current_room[0]--;
    char* pointer = get_current_room_pointer();
    if (!pointer[0]){
        generate_room_seed(pointer);
        fix_current_room_doors();
    }
    int room_width = get_room_width_from_seed(pointer);
    int door_pos = pointer[3] & 0x7;
    set_player_position(room_width - 1, door_pos - 1);
    set_current_room_to_seed(pointer);
}

void enter_right_door(){
    current_room[0]++;
    right++;
    char* pointer = get_current_room_pointer();
    if (!pointer[0]){
        generate_room_seed(pointer);
        fix_current_room_doors();
    }
    int door_pos = pointer[2] & 0x7;
    set_player_position(0, door_pos - 1);
    set_current_room_to_seed(pointer);
}

void enter_bottom_door(){
    current_room[1]++;
    char* pointer = get_current_room_pointer();
    if (!pointer[0]){
        generate_room_seed(pointer);
        fix_current_room_doors();
    }
    int door_pos = pointer[2] >> 3;
    set_player_position(door_pos - 1, 0);
    set_current_room_to_seed(pointer);
}

void reset_level(){
    int i;
    for (i = 0; i < level_length*level_length*seed_size; i++){
        room_seeds[i] = 0;
    }
    current_room[0] = 0;
    current_room[1] = 0;
}