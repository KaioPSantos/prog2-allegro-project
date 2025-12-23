#include "enemy.h"
#include <allegro5/allegro.h>
#include <stdlib.h>
#include <math.h>

extern ALLEGRO_BITMAP* spritesheet_enemy;

#define ENEMY_SX   TILE_SIZE
#define ENEMY_SY    TILE_SIZE
#define ENEMY_SPEED 5       /* px por frame – um pouco mais lento */
#define DIR_STEPS  10       /* frames antes de escolher nova direção */


void init_enemy(Enemy* e, int x, int y) {
    e->tx = e->target_tx = x;
    e->ty = e->target_ty = y;
    e->px = x * TILE_SIZE;
    e->py = y * TILE_SIZE;
    e->moving = 0;
    e->dir_timer = DIR_STEPS;
    e->dir = rand() % 4;
}


void choose_new_dir(Enemy* e) {
    for (int i = 0; i < 8; i++) {
        int try_dir = rand() % 4;
        int nx = e->tx, ny = e->ty;
        if (try_dir == 0) ny--;
        if (try_dir == 1) ny++;
        if (try_dir == 2) nx--;
        if (try_dir == 3) nx++;

        if (nx >= 0 && ny >= 0 && nx < MAP_WIDTH && ny < MAP_HEIGHT &&
            !is_wall(nx, ny)) {
            e->dir = try_dir;
            e->target_tx = nx;
            e->target_ty = ny;
            e->moving = 1;
            e->dir_timer = DIR_STEPS;
            return;
        }
    }
    e->dir_timer = 15;
}
int sx = 0;
int sy = 0;

void update_enemy(Enemy* e) {
    if (!e->moving) {
        sy = 0;
        if (e->dir_timer-- <= 0) choose_new_dir(e);
    }

    if (e->moving) {
        float dstx = e->target_tx * TILE_SIZE;
        float dsty = e->target_ty * TILE_SIZE;

        if (e->px < dstx) e->px += ENEMY_SPEED;
        if (e->px > dstx) e->px -= ENEMY_SPEED;
        if (e->py < dsty) e->py += ENEMY_SPEED;
        if (e->py > dsty) e->py -= ENEMY_SPEED;

        if (e->px < dstx) sx = 2;
        if (e->px > dstx) sx = 3;
        if (e->py < dsty) sx = 0;
        if (e->py > dsty) sx = 1;

        if (fabsf(e->px - dstx) < ENEMY_SPEED && fabsf(e->py - dsty) < ENEMY_SPEED) {
            e->px = dstx; e->py = dsty;
            e->tx = e->target_tx;
            e->ty = e->target_ty;
            e->moving = 0;
        }

        sy = (sy+1)%3;

    }

}

void draw_enemy(Enemy* e) {
    al_draw_bitmap_region(
        spritesheet_enemy,
        sx*ENEMY_SX, ENEMY_SY*sy,
        TILE_SIZE, TILE_SIZE,
        e->px, e->py, 0);
}
