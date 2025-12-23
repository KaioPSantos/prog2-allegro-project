#include "player.h"
#include "map.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

extern ALLEGRO_BITMAP* spritesheet;
int movendo = 0;
void init_player(Player* p, int x, int y) {
    p->x = x;
    p->y = y;
    p->s = 1;
    p->si = 0;
}

void update_player(Player* p) {
    ALLEGRO_KEYBOARD_STATE key_state;
    al_get_keyboard_state(&key_state);



    int new_x = p->x;
    int new_y = p->y;

    if (al_key_down(&key_state, ALLEGRO_KEY_UP)){    new_y--; p->si = 1;movendo=1;}
    else if (al_key_down(&key_state, ALLEGRO_KEY_DOWN)){    new_y++; p->si = 0;movendo=1;}
    else if (al_key_down(&key_state, ALLEGRO_KEY_LEFT)){    new_x--; p->si = 3;movendo=1;}
    else if (al_key_down(&key_state, ALLEGRO_KEY_RIGHT)){    new_x++; p->si = 2;movendo=1;}
    else if (!al_key_down(&key_state, ALLEGRO_KEY_UP) &&
        !al_key_down(&key_state, ALLEGRO_KEY_DOWN) &&
        !al_key_down(&key_state, ALLEGRO_KEY_LEFT) &&
        !al_key_down(&key_state, ALLEGRO_KEY_RIGHT)){movendo=0;}


    if(movendo){
        p->s = (p->s+1)%3;
    }

    if (!is_wall(new_x, new_y)) {
    p->x = new_x;
    p->y = new_y;
    }

    if (is_saida(p->x, p->y)) {
        int next_id = current_map->id + 1;
        if (next_id > 5) {
            next_id = 1; // volta para o mapa 1 após o 5
        }
        switch_to_map(next_id);
        p->x = 1;
        p->y = 1;
    }

}

void draw_player(Player* p) {
    al_draw_bitmap_region(spritesheet, 40*p->si, 3+40*p->s*movendo, 40,40, p->x * 40, p->y * 40, 0);
}
