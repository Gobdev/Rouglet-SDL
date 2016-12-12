#pragma once

char* get_enemy_name(int enemy);
char enemy_is_enabled(int enemy);
char enemy_is_alive(int enemy);
int enemy_get_x(int enemy);
int enemy_get_y(int enemy);
char get_enemy_seed(int enemy);
void clear_enemies();
void delete_enemy(int enemy);
void init_enemy(int enemy, int type, int x, int y);
void enemy_draw(int enemy, int x, int y);
void enemy_damage_player(int enemy);
void enemy_take_damage(int enemy, int damage);