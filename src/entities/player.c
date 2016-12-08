#include "player.h"
#include "../stdlib/rng.h"
#include "../stdlib/graphics.h"
#include "../stdlib/stdlib.h"
#include "../images/UI.h"
#include "../images/character_sprites.h"

int player_hp = 40;
int player_max_hp = 995;
int player_atk_low = 135;
int player_atk_high = 165;
int player_gold = 0;
int player_level = 1;
int player_x = 66;
int player_y = 16;

void set_player_position(int new_x, int new_y){
    player_x = new_x;
    player_y = new_y;
}
void player_moveUp(){
    player_y += 7;
}
void player_moveDown(){
    player_y -= 7;
}
void player_moveRight(){
    player_x += 7;
}
void player_moveLeft(){
    player_x -= 7;
}

void player_level_up(){
    player_level++;
    player_atk_high += 3;
    player_atk_low += 3;
    player_max_hp += 10;
    player_hp += 10;
}

int player_get_attack_damage(){
    int extra_dmg = get_random_int(player_atk_high - player_atk_low);
    return player_atk_low + extra_dmg;
}

void player_draw_ui(){
    paint_pic(0, 0, ui1);
    print_int(10, 2, min_int(999, player_atk_low));
    print_text(22, 2, "-");
    print_int(26, 2, min_int(999, player_atk_high));
    print_int(10, 10, min_int(999, player_hp));
    print_text(22, 10, "/");
    print_int(26, 10, min_int(999, player_max_hp));
    print_int(10, 18, min_int(9999999, player_gold));
    print_int(25, 26, min_int(999, player_level));
}

void player_draw(){
    paint_pic(player_x, player_y, smileyMan);
}

void print_player_info(){
    print_int(52, 14, player_x);
    print_int(52, 20, player_y);
}