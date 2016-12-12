#include "enemies.h"
#include "enemy_arrays.h"
#include "../stdlib/graphics.h"
#include "../stdlib/input.h"
#include "../stdlib/stdlib.h"

const int number_of_enemies = 4;

char* enemy_names[15] = {"ghost",
                         "firekid",
                         "frog",
                         "slime",
                         "eye",
                         "mreye",
                         "fetus",
                         "spooks",
                         "skeleton",
                         "flowerman",
                         "alien",
                         "alienking",
                         "evilthing",
                         "oni",
                         "knight"};
char enemy_enabled[4] = {0};
int enemy_max_hp[4];
int enemy_hp[4];
int enemy_atk_low[4];
int enemy_atk_high[4];
int enemy_x[4];
int enemy_y[4];
int enemy_exp_on_kill[4];
char enemy_type[4];
char* enemy_name[4];
char* enemy_sprite[4]; 

char enemy_is_enabled(int enemy){
    return enemy_enabled[enemy];
}


char enemy_is_alive(int enemy){
    return (char) (enemy_hp[enemy] > 0);
}

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

int enemy_next_to_player(int enemy){
    return abs(player_get_x() - enemy_x[enemy]) + abs(player_get_y() - enemy_y[enemy]) == 1;
}

char get_enemy_seed(int enemy){
    if (enemy_get_hp(enemy) > 0)
        return enemy_type[enemy] + 1;
    return 0;
}

void enemy_move_up(int enemy){
    enemy_y[enemy]--;
}

void enemy_move_down(int enemy){
    enemy_y[enemy]++;
}

void enemy_move_left(int enemy){
    enemy_x[enemy]--;
}

void enemy_move_right(int enemy){
    enemy_x[enemy]++;
}

char* get_enemy_name(int enemy){
    return (char*) enemy_name[enemy];
}

void enemy_print_damage(int enemy, int damage){
    char text1[20] = {0};
    char text2[20] = {0};

    concat_3_strings(20, text1, get_enemy_name(enemy), " hits you", "");
    int int_len = intlen(damage);
    char damage_string[4] = {0};
    int_to_string(damage, damage_string);
    concat_3_strings(20, text2, "for ", damage_string, " damage.");
    
    pop_up_text(text1, text2);
}

void enemy_damage_player(int enemy){
    int damage = enemy_atk_low[enemy] + get_random_int(enemy_atk_high[enemy] - enemy_atk_low[enemy]);
    if (damage > 0){
        enemy_print_damage(enemy, damage);
        player_take_damage(damage);
    }
}

void enemy_take_damage(int enemy, int damage){
    enemy_hp[enemy] -= damage;
    if (enemy_hp[enemy] <= 0){
        player_receive_exp(enemy_exp_on_kill[enemy]);
        char text[20] = {0};
        concat_3_strings(20, text, get_enemy_name(enemy), " dies.", "");
        pop_up_text(text, "");
    }
}

void disable_enemy(int enemy){
    enemy_enabled[enemy] = 0;
}

void delete_enemy(int enemy){
    enemy_enabled[enemy] = 0;
    enemy_max_hp[enemy] = 0;
    enemy_hp[enemy] = 0;
    enemy_atk_low[enemy] = 0;
    enemy_atk_high[enemy] = 0;
    enemy_x[enemy] = 0;
    enemy_y[enemy] = 0;
    enemy_exp_on_kill[enemy] = 0;
    enemy_sprite[enemy] = 0;
}

void clear_enemies(){
    delete_enemy(0);
    delete_enemy(1);
    delete_enemy(2);
    delete_enemy(3);
}

char* enemy_get_sprite(int enemy){
    return enemy_sprite[enemy];
}

void init_enemy(int enemy, int type, int x, int y){
    enemy_type[enemy] = (char) type;
    enemy_enabled[enemy] = 1;
    enemy_name[enemy] = enemy_names[type];
    enemy_x[enemy] = x;
    enemy_y[enemy] = y;
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
    int i;
    for (i = 0; i < number_of_enemies; i++){
        print_int(42, 0 + 6 * i, enemy_type[i]);
    }
}

char enemy_on_square(int x, int y){
    int i;
    for (i = 0; i < number_of_enemies; i++){
        if (enemy_hp[i] > 0 && enemy_x[i] == x && enemy_y[i] == y)
            return i + 1;
    }
    return 0;
}