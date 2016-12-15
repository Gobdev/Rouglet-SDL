#pragma once

void set_player_position(int x, int y);
void player_move_up();
void player_move_down();
void player_move_right();
void player_move_left();
void player_level_up();
void player_get_gold(int gold);
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
int get_inventory_element(int index);
void player_damage_enemy(int enemy);
void player_take_damage(int damage);
void drop_item(int item_index);
void reset_player();