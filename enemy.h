#pragma once
#include "map.h"

typedef struct {
    /* posição atual em tiles */
    int tx, ty;

    /* posição suave em pixels */
    float px, py;

    /* destino em tiles */
    int target_tx, target_ty;

    int moving;        /* 0 parado, 1 em trânsito */

    int dir_timer;
    int dir;
} Enemy;


void init_enemy(Enemy* e, int x, int y);
void update_enemy(Enemy* e);           // (exemplo: movimento randômico)
void draw_enemy(Enemy* e);
