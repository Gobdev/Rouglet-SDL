#include "enemies.h"
#include "enemy_arrays.h"
#include "../stdlib/graphics.h"

const int number_of_enemies = 4;
int enemy_max_hp[4];
int enemy_hp[4];
int enemy_atk_low[4];
int enemy_atk_high[4];
int enemy_x[4];
int enemy_y[4];
int enemy_exp_on_kill[4];
char* enemy_sprite[4]; 

int enemy_get_hp(int enemy){
    return enemy_hp[enemy];
}

int enemy_get_attack_low(int enemy){
    return enemy_atk_low[enemy];
}

int enemy_get_attack_high(int enemy){
    return enemy_atk_high[enemy];
}

int enemy_get_x(int enemy){
    return enemy_x[enemy];
}

int enemy_get_y(int enemy){
    return enemy_y[enemy];
}

int get_number_of_enemies(){
    return number_of_enemies;
}

void enemy_damage_player(int enemy){
    player_take_damage(enemy_atk_low[enemy] + get_random_int(enemy_atk_high[enemy] - enemy_atk_low[enemy]));
}

void enemy_take_damage(int enemy, int damage){
    enemy_hp[enemy] -= damage;
    if (enemy_hp[enemy] <= 0)
        delete_enemy(enemy);
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

char* enemy_get_sprite(int enemy){
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
    enemy_sprite[enemy] = (char*) enemy_array[6 * type + 5];
}

void enemy_draw(int enemy, int x, int y){
    if (enemy_hp[enemy] > 0)
        paint_pic(x, y, enemy_sprite[enemy]);
}

char enemy_on_square(int x, int y){
    int i;
    for (i = 0; i < number_of_enemies; i++){
        if (enemy_hp[i] > 0 && enemy_x[i] == x && enemy_y[i] == y)
            return i + 1;
    }
    return 0;
}