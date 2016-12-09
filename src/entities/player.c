#include "player.h"
#include "../stdlib/rng.h"
#include "../stdlib/graphics.h"
#include "../stdlib/stdlib.h"
#include "../images/UI.h"
#include "../images/character_sprites.h"

enum items {EMPTY, SWORD1, SWORD2, SWORD3, SPEAR1, SPEAR2, HAMMER, GREATSWORD, PICKAXE, AXE, HALBERD, SMALL_AXE, TRIDENT, BATTLE_AXE, POTION, BOMB};

int weapon_low_list[] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130};
int weapon_high_list[] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130};

int player_max_hp = 160;
int player_hp = 160;
int player_atk_low = 135;
int player_atk_high = 165;
int weapon_atk_low = 0;
int weapon_atk_high = 0;
int player_gold = 0;
int player_level = 1;
int player_x = 0;
int player_y = 0;
int inventory_size = 5;
int potion_heal_amount = 50;
int inventory[5] = {0};
int active_weapon_index = 100;

int get_inventory_size(){
    return inventory_size;
}

int get_inventory_element(int index){
    return inventory[index];
}

int player_get_x(){
    return player_x;
}

int player_get_y(){
    return player_y;
}

void set_current_weapon(int low,int high,int item_index){
    weapon_atk_low = low;
    weapon_atk_high = high;
    active_weapon_index = item_index;
}

int get_active_weapon_index(){
    return active_weapon_index;
}


void set_player_position(int new_x, int new_y){
    player_x = new_x;
    player_y = new_y;
}


void draw_active_weapon(){

}

void use_item(int item_index){
    int item_id = inventory[item_index];
    switch(item_id){
        case POTION:
            if(player_hp + potion_heal_amount >= player_max_hp){
                player_hp = player_max_hp;
            }else{
                player_hp += potion_heal_amount;
            }
            remove_item(item_index);
            break;
        case BOMB:
            break; //TODO
        default:
            set_current_weapon(weapon_low_list[item_id], weapon_high_list[item_id], item_index);
            break;
    }
}


//TODO if inveotory is full, dont remove item form ground
void add_to_inventory(int item_id){
    int i;
    if (inventory[inventory_size - 1] == 0){
        for (i = 0; i < inventory_size; i++){
            if (inventory[i] == 0){
                inventory[i] = item_id;
                break;
            }
        }  
    }
}

void remove_item(int item_index){
    int i;
    if (item_index == active_weapon_index)
        set_current_weapon(0,0,100);
    for (i = item_index ; i < inventory_size - 1 ; i++){
        inventory[i] = inventory[i+1];
    }
    inventory[inventory_size - 1] = 0;
}

void player_damage_enemy(int enemy){
    enemy_take_damage(enemy, player_atk_low + get_random_int(player_atk_high - player_atk_low));
}

void player_take_damage(int damage){
    player_hp -= damage;
}

void player_move_up(){
    player_y --;
}
void player_move_down(){
    player_y ++;
}
void player_move_right(){
    player_x ++;
}
void player_move_left(){
    player_x --;
}

void player_level_up(){
    player_level++;
    player_atk_high += 3;
    player_atk_low += 3;
    player_max_hp += 10;
    player_hp += 10;
}

int player_get_attack_damage(){
    int extra_dmg = get_random_int((player_atk_high + weapon_atk_high) - (player_atk_low + weapon_atk_low));
    return player_atk_low + weapon_atk_low + extra_dmg;
}

void player_draw_inventory_ui(){
    paint_pic(0, 0, inventory_ui);
}

void player_draw_main_ui(){
    paint_pic(0, 0, main_ui);
    print_int(10, 2, min_int(999, player_atk_low + weapon_atk_low));
    print_text(22, 2, "-");
    print_int(26, 2, min_int(999, player_atk_high + weapon_atk_high));
    print_int(10, 10, min_int(999, player_hp));
    print_text(22, 10, "/");
    print_int(26, 10, min_int(999, player_max_hp));
    print_int(10, 18, min_int(9999999, player_gold));
    print_int(25, 26, min_int(999, player_level));
}

void player_draw(int x, int y){
    paint_pic(x, y, smileyMan);
}

void print_player_info(){

}