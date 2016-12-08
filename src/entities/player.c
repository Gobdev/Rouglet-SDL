#include "player.h"
#include "../stdlib/rng.h"
#include "../stdlib/graphics.h"
#include "../images/UI.h"
#include "../images/character_sprites.h"

enum items {SWORD1,SWORD2,SWORD3,SPEAR1,SPEAR2,HAMMER,GREATSWORD,PICKAXE,AXE,HALBERD,SMALL_AXE,TRIDENT,STAFF,BATTLE_AXE,POTION,BOMB};

int weapon_low_list[] = {10,20,30,40,50,60,70,80,90,100,110,120,130,140};
int weapon_high_list[] = {10,20,30,40,50,60,70,80,90,100,110,120,130,140};

int player_max_hp = 160;
int player_hp = player_max_hp;
int player_atk_low = 135;
int player_atk_high = 165;
int weapon_atk_low = 0;
int weapon_atk_high = 0;
int player_gold = 0;
int player_level = 1;
int player_x = 64;
int player_y = 12;
int inventory_size = 15;
int potion_heal_amount = 50;
int inventory[inventory_size] = {0};
int active_weapon_index = 0;

void set_current_weapon(int low,int high,int item_index){
    weapon_atk_low = attack_low;
    weapon_atk_high = attack_high;
    active_weapon_index = item_index;
}

void draw_active_weapon(){
    //TODO
}

void use_item(int item_index){
    int item_id = inventory[index];
    switch(){
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
        case default:
            set_current_weapon(weapon_low_list[item_id], weapon_high_list[item_id], item_index);
    }
}

void add_to_inventory(int item_id){
    for(int i = 0 ; i < inventory_size ; i++){
        if(inventory[i] == 0){
            inventory[i] = item_id;
        }
    }
}

void remove_item(int item_index){
    for(int i = item_index ; i < inventory_size - 1 ; i++){
        inventory[i] = inventory[i+1];
    }
    inventory[inventory_size - 1] = 0;
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

void player_draw_ui(){
    paint_pic(0, 0, ui1);
    print_int(10, 2, (player_atk_low + weapon_atk_low));
    print_text(22, 2, "-");
    print_int(26, 2, (player_atk_high + weapon_atk_high));
    print_int(10, 10, player_hp);
    print_text(22, 10, "/");
    print_int(26, 10, player_max_hp);
    print_int(10, 18, player_gold);
    print_int(25, 26, player_level);
}

void player_draw(){
    paint_pic(player_x, player_y, smileyMan);
}