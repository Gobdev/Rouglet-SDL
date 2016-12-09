#include "level.h"
#include "room.h"
#include "../entities/enemies.h"
#include "../entities/player.h"
#include "../stdlib/graphics.h"

const int seed_size = 6;
const int level_length = 25;
char room_seeds[6 * 25 * 25] = {0};
int current_room[2] = {10, 10};
char left_spawned = 0;
char right_spawned = 0;

char get_room_width_from_seed(char* seed){
    return (int) (seed[0] >> 4) + 1;
}

char get_room_height_from_seed(char* seed){
    return (int) (seed[0] & 0x0F);
}

void level_init(){
    int x = current_room[0];
    int y = current_room[1];
    char* pointer = room_seeds + seed_size * (x + y * 10);
    generate_room_seed(pointer);
    int room_width = get_room_width_from_seed(pointer);
    int room_height = get_room_height_from_seed(pointer);
    pointer[2] &= 0;
    pointer[2] |= (generate_horizontal_door(room_width) << 3) + generate_vertical_door(room_height);
    pointer[3] &= 0;
    pointer[3] |= (generate_horizontal_door(room_width) << 3) + generate_vertical_door(room_height);     
    set_current_room_to_seed(pointer);
    init_enemy(0, 0, 1, 1);
}

void level_draw(){
    room_draw();
    if (left_spawned)
        print_text(42, 0, "left");
    if (right_spawned)
        print_text(42, 6, "right");
}

void print_pointer(){
    int x = current_room[0];
    int y = current_room[1];
    print_hex(50, 24, (int) (room_seeds + seed_size * (x + y * 100)));
    print_hex(50, 18, (int) room_seeds);
}

char* get_room_pointer(int x, int y){
    return room_seeds + seed_size * (x + y * level_length);
}

char* get_current_room_pointer(){
    return get_room_pointer(current_room[0], current_room[1]);
}


void fix_current_room_doors(){
    char* pointer = get_current_room_pointer();
    int room_height = get_room_height_from_seed(pointer);
    int room_width = get_room_width_from_seed(pointer);

    left_spawned = 0;
    right_spawned = 0;

    if (seed_has_right_door(get_room_pointer(current_room[0] - 1, current_room[1]))){
        pointer[2] &= 0xF8;
        pointer[2] |= generate_vertical_door(room_height);
        left_spawned = 1;
    }
    if (seed_has_left_door(get_room_pointer(current_room[0] + 1, current_room[1]))){
        pointer[3] &= 0xF8;
        pointer[3] |= generate_vertical_door(room_height);
        right_spawned = 1;
    }
    if (seed_has_bottom_door(get_room_pointer(current_room[0], current_room[1] - 1))){
        pointer[2] &= 0x07;
        pointer[2] |= (generate_horizontal_door(room_width) << 3);
    }
    if (seed_has_top_door(get_room_pointer(current_room[0], current_room[1] + 1))){
        pointer[3] &= 0x07;
        pointer[3] |= (generate_horizontal_door(room_width) << 3);
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