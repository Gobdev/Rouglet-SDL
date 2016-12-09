#include "enemies.h"
#include "enemy_arrays.h"
#include "../stdlib/graphics.h"

int enemy_max_hp[4];
int enemy_hp[4];
int enemy_atk_low[4];
int enemy_atk_high[4];
int enemy_x[4];
int enemy_y[4];
int enemy_exp_on_kill[4];
char* enemy_sprite[4]; 

int get_enemy_hp(int enemy){
    return enemy_hp[enemy];
}

int get_enemy_attack_low(int enemy){
    return enemy_atk_low[enemy];
}

int get_enemy_attack_high(int enemy){
    return enemy_atk_high[enemy];
}

int get_enemy_x(int enemy){
    return enemy_x[enemy];
}

int get_enemy_y(int enemy){
    return enemy_y[enemy];
}

void delete_enemy(int enemy){
    enemy_max_hp[enemy] = 0;
    enemy_hp[enemy] = 0;
    enemy_atk_low[enemy] = 0;
    enemy_atk_high[enemy] = 0;
    enemy_x[enemy] = 0;
    enemy_y[enemy] = 0;
    enemy_exp_on_kill[enemy] = 0;
    enemy_sprite[enemy] = 0;
}

char* get_enemy_sprite(int enemy){
    return enemy_sprite[enemy];
}

void init_enemy(int enemy, int type){
    type = 0;
    enemy_x[enemy] = 0;
    enemy_y[enemy] = 0;
    enemy_max_hp[enemy] = enemy_array[6 * type];
    enemy_hp[enemy] = enemy_array[6 * type + 1];
    enemy_atk_low[enemy] = enemy_array[6 * type + 2];
    enemy_atk_high[enemy] = enemy_array[6 * type + 3];
    enemy_exp_on_kill[enemy] = enemy_array[6 * type + 4];
    enemy_sprite[enemy] = enemy_array[6 * type + 5];
}

void draw_enemy(int enemy){
    paint_pic(enemy_x[enemy], enemy_y[enemy], enemy_sprite[enemy]);
}