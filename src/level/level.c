#include "level.h"
#include "room.h"
#include "../entities/enemies.h"

const int seed_size = 6;
char room_seeds[6 * 25 * 25] = {0};
int current_room[2] = {10, 10};

void level_init(){
    int x = current_room[0];
    int y = current_room[1];
    char* pointer = room_seeds + seed_size * (x + y * 10);
    generate_room_seed(pointer);
    set_current_room_to_seed(pointer);
    init_enemy(0, 0);
}

void level_draw(){
    room_draw();
}

void print_pointer(){
    int x = current_room[0];
    int y = current_room[1];
    print_hex(50, 24, (int) (room_seeds + seed_size * (x + y * 100)));
    print_hex(50, 18, (int) room_seeds);
}