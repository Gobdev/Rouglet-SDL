#include "level.h"
#include "room.h"

char room_seeds[34 * 20 * 20] = {0};
int current_room[2] = {10, 10};

void level_init(){
    int x = current_room[0];
    int y = current_room[1];
    char* pointer = room_seeds + 34 * (x + y * 10);
    generate_room_seed(pointer);
    pointer[2] |= 0xF0;
    set_current_room_to_seed(pointer);
}

void level_draw(){
    room_draw();
}

void print_pointer(){
    int x = current_room[0];
    int y = current_room[1];
    print_hex(50, 24, (int) (room_seeds + 34 * (x + y * 100)));
    print_hex(50, 18, (int) room_seeds);
}