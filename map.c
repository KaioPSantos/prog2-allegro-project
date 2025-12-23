#include "map.h"
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

int map[MAP_HEIGHT][MAP_WIDTH];
extern ALLEGRO_BITMAP* tiles;
Map* current_map = NULL;


void add_map(int id, const char* filename) {
    Map* new_map = malloc(sizeof(Map));
    new_map->id = id;
    new_map->next = NULL;

    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Erro ao abrir %s\n", filename);
        free(new_map);
        return;
    }

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH;) {
            int ch = fgetc(file);
            if (ch == EOF) break;
            if (ch >= '0' && ch <= '9') {
                new_map->tiles[y][x] = ch - '0';
                x++;
            }
        }
    }

    fclose(file);

    if (!current_map) {
        current_map = new_map;
    } else {
        Map* temp = current_map;
        while (temp->next) temp = temp->next;
        temp->next = new_map;
    }
}

int get_map(int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT || !current_map)
        return -1;
    return current_map->tiles[y][x];
}

void add_multiple_maps(int quantidade, const char* base) {
    char filename[64];

    for (int i = 1; i <= quantidade; i++) {
        if (i == 1) {
            strcpy(filename, base);
        } else {
            snprintf(filename, sizeof(filename), "map%d.txt", i);
        }

        add_map(i, filename);
    }
}

void free_all_maps() {
    Map* temp = current_map;
    while (temp) {
        Map* next = temp->next;
        free(temp);
        temp = next;
    }
    current_map = NULL;
}

void draw_map() {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            int centro  = get_map(x, y);
            int direita = get_map(x + 1, y);
            int baixo   = get_map(x, y + 1);
            int esq     = get_map(x - 1, y);
            int cima    = get_map(x, y - 1);
            int ce      = get_map(x - 1, y - 1);
            int cd      = get_map(x + 1, y - 1);
            int bd      = get_map(x + 1, y + 1);
            int be      = get_map(x - 1, y + 1);

            int src_x = 0, src_y = 0;

            if (centro == 0 || centro == 3) {
                // Verde (livre)
                src_x = 0 * TILE_SIZE;
                src_y = 0 * TILE_SIZE;

            } else if (centro == 2) {
                // Azul (ex: água)
                src_x = 1 * TILE_SIZE;
                src_y = 0 * TILE_SIZE;

            } else if (centro == 1) {

                al_draw_bitmap_region(
                tiles,
                src_x, src_y,
                TILE_SIZE, TILE_SIZE,
                x * TILE_SIZE, y * TILE_SIZE,
                0
            );

                // ░ Quinas internas (para o interior de paredes)
                if (direita == 1 && baixo == 1 && cima == 0 && esq == 0 && bd == 1) {
                    src_x = 0 * TILE_SIZE;
                    src_y = 3 * TILE_SIZE; // ┌ canto sup-esquerdo (interno)

                } else if (direita == 1 && baixo == 1 && (cima == 0 || cima == 2) && (esq == 0 || esq == 2) && (bd == 2 || bd == 0)) {
                    src_x = 2 * TILE_SIZE;
                    src_y = 5 * TILE_SIZE; //
                     if(bd == 2){
                    al_draw_bitmap_region(
                    tiles,
                    40, 0,
                    20, 20,
                    (x * TILE_SIZE+20), (y * TILE_SIZE+20),
                    0
                 );
                    }

                } else if (direita == 1 && esq == 1 && baixo == 1 && be != 1 && bd != 1) {
                    src_x = 0 * TILE_SIZE;
                    src_y = 6 * TILE_SIZE; // parede de uma linha baixo

                } else if (direita == 1 && esq == 1 && cima == 1 && ce != 1 && cd != 1) {
                    src_x = 0 * TILE_SIZE;
                    src_y = 7 * TILE_SIZE; // parede de uma linha baixo

                } else if (cima == 1 && esq == 1 && baixo == 1 && be != 1 && ce != 1) {
                    src_x = 1 * TILE_SIZE;
                    src_y = 6 * TILE_SIZE; // parede de uma linha baixo

                } else if (direita == 1 && cima == 1 && baixo == 1 && cd != 1 && bd != 1) {
                    src_x = 1 * TILE_SIZE;
                    src_y = 7 * TILE_SIZE; // parede de uma linha baixo

                } else if ((esq == 1 && get_map(x-2, y) == 0) && (direita == 0 || direita == 2) && cima != 1 && baixo != 1) {
                    src_x = 1 * TILE_SIZE;
                    src_y = 5 * TILE_SIZE; // parede de uma linha baixo

                } else if ((direita == 1 && get_map(x+2, y) == 0) && (esq == 0 || esq == 2) && cima != 1 && baixo != 1) {
                    src_x = 1 * TILE_SIZE;
                    src_y = 5 * TILE_SIZE; // parede de uma linha baixo

                } else if ((cima == 1 && get_map(x, y-2) == 0) && (baixo == 0 || baixo == 2) && direita != 1 && esq != 1) {
                    src_x = 0 * TILE_SIZE;
                    src_y = 5 * TILE_SIZE; // parede de uma linha baixo

                } else if ((baixo == 1 && get_map(x, y+2) == 0) && (cima == 0 || cima == 2) && direita != 1 && esq != 1) {
                    src_x = 0 * TILE_SIZE;
                    src_y = 5 * TILE_SIZE; // parede de uma linha baixo


                } else if ((direita == 0 || direita == 2) && (esq == 0 || esq == 2) && (baixo == 0 || baixo == 2) && cima == 1) {
                    src_x = 3 * TILE_SIZE;
                    src_y = 4 * TILE_SIZE; // parede de uma linha baixo

                } else if ((direita == 0 || direita == 2) && (esq == 0 || esq == 2) && (cima == 0 || cima == 2) && baixo == 1) {
                    src_x = 0 * TILE_SIZE;
                    src_y = 4 * TILE_SIZE; // parede de uma linha cima

                } else if ((direita == 0 || direita == 2) && (baixo == 0 || baixo == 2) && (cima == 0 || cima == 2) && esq == 1) {
                    src_x = 1 * TILE_SIZE;
                    src_y = 4 * TILE_SIZE; //

                } else if ((esq == 0 || esq == 2) && (baixo == 0 || baixo == 2) && (cima == 0 || cima == 2) && direita == 1) {
                    src_x = 2 * TILE_SIZE;
                    src_y = 4 * TILE_SIZE; //

                } else if ((direita == 0 || direita == 2) && (esq == 0 || esq == 2) && cima == 1 && baixo == 1) {
                    src_x = 0 * TILE_SIZE;
                    src_y = 5 * TILE_SIZE;
                    if(esq == 2){
                        al_draw_bitmap_region(
                        tiles,
                        40, 0,
                        20, TILE_SIZE,
                        x * TILE_SIZE, y * TILE_SIZE,
                        0
                 );}if(direita == 2){
                        al_draw_bitmap_region(
                        tiles,
                        40, 0,
                        20, TILE_SIZE,
                        (x * TILE_SIZE+20), y * TILE_SIZE,
                        0
                 );
                    }

                } else if (direita == 1 && esq == 1 && (cima == 0 || cima == 2) &&(baixo == 0 || baixo == 2)) {
                    src_x = 1 * TILE_SIZE;
                    src_y = 5 * TILE_SIZE; //
                    if(baixo == 2){
                        al_draw_bitmap_region(
                        tiles,
                        40, 0,
                        TILE_SIZE, TILE_SIZE/2,
                        x * TILE_SIZE, y * TILE_SIZE+20,
                        0
                 );
                    }if(cima == 2){
                        al_draw_bitmap_region(
                        tiles,
                        40, 0,
                        40, 40,
                        (x * TILE_SIZE), (y * TILE_SIZE-20),
                        0
                 );
                    }


                } else if (direita == 1 && baixo == 1 && (cima == 2 || cima == 0) && (esq == 2 || esq == 0)) {
                    src_x = 0 * TILE_SIZE;
                    src_y = 3 * TILE_SIZE; // ┌ canto sup-esquerdo (interno)
                    al_draw_bitmap_region(
                    tiles,
                    40, 0,
                    TILE_SIZE, TILE_SIZE,
                    x * TILE_SIZE, y * TILE_SIZE,
                    0
                 );

                } else if (esq == 1 && baixo == 1 && cima == 0 && direita == 0 && be == 1) {
                    src_x = 3 * TILE_SIZE;
                    src_y = 3 * TILE_SIZE; // ┐ canto sup-direito (interno)

                } else if (esq == 1 && baixo == 1 && (cima == 0 || cima == 2) && (direita == 0 || direita == 2) && (be == 2 || be == 0)) {
                    src_x = 3 * TILE_SIZE;
                    src_y = 5 * TILE_SIZE; //
                     if(be == 2){
                    al_draw_bitmap_region(
                    tiles,
                    40, 0,
                    20, 20,
                    (x * TILE_SIZE), (y * TILE_SIZE+20),
                    0
                 );
                    }

                } else if (esq == 1 && baixo == 1 && (cima == 2 || cima == 0) && (direita == 2 || direita == 0) && be == 1) {
                    src_x = 3 * TILE_SIZE;
                    src_y = 3 * TILE_SIZE; // ┐ canto sup-direito (interno)
                    al_draw_bitmap_region(
                    tiles,
                    40, 0,
                    TILE_SIZE, TILE_SIZE,
                    x * TILE_SIZE, y * TILE_SIZE,
                    0
                 );

                } else if (direita == 1 && cima == 1 && baixo == 0 && esq == 0 && cd == 1) {
                    src_x = 1 * TILE_SIZE;
                    src_y = 3 * TILE_SIZE; // └ canto inf-esquerdo (interno)

                } else if (direita == 1 && cima == 1 && (baixo == 2 || baixo == 0) && (esq == 2 || esq == 0) && cd == 1) {
                    src_x = 1 * TILE_SIZE;
                    src_y = 3 * TILE_SIZE; // └ canto inf-esquerdo (interno)
                    al_draw_bitmap_region(
                    tiles,
                    40, 0,
                    TILE_SIZE, TILE_SIZE,
                    x * TILE_SIZE, y * TILE_SIZE,
                    0
                 );

                } else if (direita == 1 && cima == 1 && (baixo == 0 || baixo == 2) && (esq == 0 || esq == 2) && (cd == 2 || cd == 0)) {
                    src_x = 2 * TILE_SIZE;
                    src_y = 6 * TILE_SIZE;
                     if(cd == 2){
                    al_draw_bitmap_region(
                    tiles,
                    40, 0,
                    20, 20,
                    (x * TILE_SIZE+20), (y * TILE_SIZE),
                    0
                 );
                    }


                } else if (esq == 1 && cima == 1 && baixo == 0 && direita == 0 && ce == 1) {
                    src_x = 2 * TILE_SIZE;
                    src_y = 3 * TILE_SIZE; // ┘ canto inf-direito (interno)

                } else if (esq == 1 && cima == 1 && (baixo == 2 || baixo == 0) && (direita == 2 || direita == 0) && ce == 1) {
                    src_x = 2 * TILE_SIZE;
                    src_y = 3 * TILE_SIZE; // ┘ canto inf-direito (interno)

                    al_draw_bitmap_region(
                    tiles,
                    40, 0,
                    TILE_SIZE, TILE_SIZE,
                    x * TILE_SIZE, y * TILE_SIZE,
                    0
                 );
                } else if (esq == 1 && cima == 1 && (baixo == 0 || baixo == 2) && (direita == 0 || direita == 2) && (ce == 2 || ce == 0)) {
                    src_x = 3 * TILE_SIZE;
                    src_y = 6 * TILE_SIZE;
                    if(ce == 2){
                    al_draw_bitmap_region(
                    tiles,
                    40, 0,
                    20, 20,
                    (x * TILE_SIZE), (y * TILE_SIZE),
                    0
                 );
                    }

                } else if (esq == 1 && cima == 0 && baixo == 1 && direita == 1) {
                    src_x = 3 * TILE_SIZE;
                    src_y = 2 * TILE_SIZE; // │ topo meio (parede voltada pra cima)

                // ░ Cantos externos (bordas externas do mapa)
                } else if (direita == 1 && baixo == 1 && bd == 0) {
                    src_x = 0 * TILE_SIZE;
                    src_y = 1 * TILE_SIZE; // ▄ canto sup-esquerdo (externo)

                } else if (direita == 1 && baixo == 1 && bd == 2) {
                    al_draw_bitmap_region(
                    tiles,
                    40, 0,
                    TILE_SIZE, TILE_SIZE,
                    x * TILE_SIZE, y * TILE_SIZE,
                    0
                 );
                    src_x = 0 * TILE_SIZE;
                    src_y = 1 * TILE_SIZE; // ▄ canto sup-esquerdo (externo)

                } else if (esq == 1 && baixo == 1 && be == 0) {
                    src_x = 1 * TILE_SIZE;
                    src_y = 1 * TILE_SIZE; // ▄ canto sup-direito (externo)

                } else if (esq == 1 && baixo == 1 && be == 2) {
                    src_x = 1 * TILE_SIZE;
                    src_y = 1 * TILE_SIZE;
                    al_draw_bitmap_region(
                    tiles,
                    40, 0,
                    TILE_SIZE, TILE_SIZE,
                    x * TILE_SIZE, y * TILE_SIZE,
                    0
                 );
                 }else if (esq == 1 && cima == 1 && ce == 0) {
                    src_x = 3 * TILE_SIZE;
                    src_y = 1 * TILE_SIZE; // ▀ canto inf-direito (externo)

                }else if (esq == 1 && cima == 1 && ce == 2) {
                    src_x = 3 * TILE_SIZE;
                    src_y = 1 * TILE_SIZE; // ▀ canto inf-direito (externo)
                    al_draw_bitmap_region(
                tiles,
                40, 0,
                TILE_SIZE, TILE_SIZE,
                x * TILE_SIZE, y * TILE_SIZE,
                0
            );


                } else if (direita == 1 && cima == 1 && cd == 0) {
                    src_x = 2 * TILE_SIZE;
                    src_y = 1 * TILE_SIZE; // ▀ canto inf-esquerdo (externo)

                // ░ Lados (paredes retas)
                } else if (direita == 1 && cima == 1 && cd == 2) {
                    src_x = 2 * TILE_SIZE;
                    src_y = 1 * TILE_SIZE; // ▀ canto inf-esquerdo (externo)
                    al_draw_bitmap_region(
                tiles,
                40, 0,
                TILE_SIZE, TILE_SIZE,
                x * TILE_SIZE, y * TILE_SIZE,
                0
            );


                // ░ Lados (paredes retas)
                } else if (direita == 1 && baixo == 0 && esq == 1) {
                    src_x = 0 * TILE_SIZE;
                    src_y = 2 * TILE_SIZE; // ┬ Parede virada para baixo

                } else if (direita == 1 && baixo == 2 && esq == 1) {
                    src_x = 0 * TILE_SIZE;
                    src_y = 2 * TILE_SIZE; // ┬ Parede virada para baixo
                    al_draw_bitmap_region(
                tiles,
                40, 0,
                TILE_SIZE, TILE_SIZE,
                x * TILE_SIZE, y * TILE_SIZE,
                0
            );



                } else if (direita == 0 && cima == 1 && baixo == 1) {
                    src_x = 2 * TILE_SIZE;
                    src_y = 2 * TILE_SIZE; // Parede virada para direita

                } else if (direita == 2 && cima == 1 && baixo == 1) {
                    src_x = 2 * TILE_SIZE;
                    src_y = 2 * TILE_SIZE; // Parede virada para direita
                    al_draw_bitmap_region(
                tiles,
                40, 0,
                TILE_SIZE, TILE_SIZE,
                x * TILE_SIZE, y * TILE_SIZE,
                0
            );

                } else if (baixo == 1 && cima == 1 && esq == 0) {
                    src_x = 1 * TILE_SIZE;
                    src_y = 2 * TILE_SIZE; // Parede virada para esquerda

                } else if (baixo == 1 && cima == 1 && esq == 2) {
                    src_x = 1 * TILE_SIZE;
                    src_y = 2 * TILE_SIZE; // Parede virada para esquerda
                    al_draw_bitmap_region(
                tiles,
                40, 0,
                TILE_SIZE, TILE_SIZE,
                x * TILE_SIZE, y * TILE_SIZE,
                0
            );

                } else if (baixo == 0 && cima == 0 && esq == 0 && direita == 0) {
                    src_x = 3 * TILE_SIZE;
                    src_y = 0 * TILE_SIZE; // base

                // ░ fallback padrão (tijolo cheio)
                } else if (baixo == 2 && cima == 2 && esq == 2 && direita == 2) {
                    src_x = 3 * TILE_SIZE;
                    src_y = 0 * TILE_SIZE; // base
                    al_draw_bitmap_region(
                tiles,
                40, 0,
                TILE_SIZE, TILE_SIZE,
                x * TILE_SIZE, y * TILE_SIZE,
                0
            );


                // ░ fallback padrão (tijolo cheio)
                } else if (direita == 1 && cima == 0 && esq == 1) {
                    src_x = 3 * TILE_SIZE;
                    src_y = 2 * TILE_SIZE; // Parede virada para cima

                } else if (direita == 1 && cima == 2 && esq == 1) {
                    src_x = 3 * TILE_SIZE;
                    src_y = 2 * TILE_SIZE; // Parede virada para cima
                    al_draw_bitmap_region(
                tiles,
                40, 0,
                TILE_SIZE, TILE_SIZE,
                x * TILE_SIZE, y * TILE_SIZE,
                0
            );

                }
                else {
                    src_x = 2 * TILE_SIZE;
                    src_y = 0 * TILE_SIZE; // bloco padrão
                }

            } else {
                continue; // valor inválido, ignora
            }

            al_draw_bitmap_region(
                tiles,
                src_x, src_y,
                TILE_SIZE, TILE_SIZE,
                x * TILE_SIZE, y * TILE_SIZE,
                0
            );
        }
    }
}


int is_wall(int x, int y) {
    int value = get_map(x, y);
    return (value == 1 || value == 2);
}

int is_saida(int x, int y) {
    return get_map(x, y) == 3;
}

void switch_to_map(int id) {
    Map* temp = current_map;
    while (temp) {
        if (temp->id == id) {
            current_map = temp;
            return;
        }
        temp = temp->next;
    }
    printf("Mapa %d nao encontrado\n", id);
}
