#pragma once

#define TILE_SIZE 40
#define MAP_WIDTH 20
#define MAP_HEIGHT 20

typedef struct Map {
    int id;
    int tiles[MAP_HEIGHT][MAP_WIDTH];
    struct Map* next;
} Map;

extern Map* current_map;

void add_map(int id, const char* filename);
void draw_map();
void add_multiple_maps(int quantidade, const char* base);
void free_all_maps();
int get_map(int x, int y);
int is_wall(int x, int y);
int is_saida(int x, int y);
void switch_to_map(int id);
