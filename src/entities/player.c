#include "player.h"
#include "../stdlib/rng.h"
#include "../stdlib/graphics.h"
#include "../images/UI.h"
#include "../images/character_sprites.h"

int player_hp = 160;
int player_max_hp = 160;
int player_atk_low = 135;
int player_atk_high = 165;
int player_gold = 0;
int player_level = 1;
int player_x = 64;
int player_y = 12;

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

void player_draw_ui(){
    paint_pic(0, 0, ui1);
    print_int(10, 2, player_atk_low);
    print_text(22, 2, "-");
    print_int(26, 2, player_atk_high);
    print_int(10, 10, player_hp);
    print_text(22, 10, "/");
    print_int(26, 10, player_max_hp);
    print_int(10, 18, player_gold);
    print_int(25, 26, player_level);
}

void player_draw(){
    paint_pic(player_x, player_y, smileyMan);
}
