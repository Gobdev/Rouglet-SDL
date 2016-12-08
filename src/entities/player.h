#pragma once

void set_player_position(int x, int y);
void player_moveUp();
void player_moveDown();
void player_moveRight();
void player_moveLeft();
void player_level_up();
int player_get_attack_damage();
void player_draw_main_ui();
void player_draw_inventory_ui();
void player_draw();
void print_player_info();
void set_current_weapon(int low, int high, int item_index);
void draw_active_weapon();
void use_item(int item_index);
void add_to_inventory(int item_index);
void remove_item(int item_index);
int get_inventory_size();