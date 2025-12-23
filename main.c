#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "map.h"
#include "player.h"
#include "enemy.h"

ALLEGRO_BITMAP* tiles = NULL;
ALLEGRO_BITMAP* spritesheet = NULL;
ALLEGRO_BITMAP* spritesheet_enemy = NULL;

typedef struct {
    int x, y;
    int collected;
} Collectible;

void set_enemies(Enemy enemies[2], int map_id) {
    if (map_id == 1) {
        init_enemy(&enemies[0], 5, 5);
        init_enemy(&enemies[1], 10, 10);
    } else if (map_id == 2) {
        init_enemy(&enemies[0], 3, 7);
        init_enemy(&enemies[1], 14, 2);
    } else if (map_id == 3) {
        init_enemy(&enemies[0], 8, 8);
        init_enemy(&enemies[1], 1, 12);
    } else if (map_id == 4) {
        init_enemy(&enemies[0], 2, 2);
        init_enemy(&enemies[1], 13, 11);
    } else if (map_id == 5) {
        init_enemy(&enemies[0], 6, 6);
        init_enemy(&enemies[1], 18, 5);
    } else {
        init_enemy(&enemies[0], 1, 1);
        init_enemy(&enemies[1], 2, 2);
    }
}


int main() {
    al_init();
    al_install_keyboard();
    al_init_primitives_addon();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    ALLEGRO_DISPLAY* display = al_create_display(MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE);
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_FONT* font = al_create_builtin_font();

    al_register_event_source(queue, al_get_keyboard_event_source());

    tiles = al_load_bitmap("tiles.png");
    if (!tiles)
        printf("erro tiles\n");
    al_convert_mask_to_alpha(tiles, al_map_rgb(255, 255, 255));

    spritesheet = al_load_bitmap("amogus.png");
    if (!spritesheet)
        printf("erro spritesheet\n");
    al_convert_mask_to_alpha(spritesheet, al_map_rgb(255, 255, 255));

    spritesheet_enemy = al_load_bitmap("amogus inimigo.png");
    if (!spritesheet_enemy)
        printf("erro spritesheet\n");
    al_convert_mask_to_alpha(spritesheet_enemy, al_map_rgb(255, 255, 255));

    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_display_event_source(display));

    add_multiple_maps(5, "map.txt");

    Player pacman;
    init_player(&pacman, 1, 1);

    Enemy enemies[2];
    set_enemies(enemies, current_map->id);

    Collectible item;
    item.x = 15;
    item.y = 10;
    item.collected = 0;

    al_start_timer(timer);

    enum { STATE_PLAYING, STATE_GAME_OVER, STATE_VICTORY };
    int state = STATE_PLAYING;

    int running = 1;
    int tempo = 0;
    int last_map_id = current_map->id;


    while (running) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        if (current_map->id != last_map_id) {
            set_enemies(enemies, current_map->id);
            last_map_id = current_map->id;
        }


        if (ev.type == ALLEGRO_EVENT_TIMER) {
            if (state == STATE_PLAYING) {
                tempo++;
                if (tempo % 3 == 0)
                    update_player(&pacman);

                update_enemy(&enemies[0]);
                update_enemy(&enemies[1]);

                // Colisão com inimigos
                for (int i = 0; i < 2; i++) {
                    if (pacman.x == enemies[i].tx && pacman.y == enemies[i].ty) {
                        state = STATE_GAME_OVER;
                    }
                }

                // Coletar item no mapa 5
                if (current_map->id == 5 && !item.collected &&
                    pacman.x == item.x && pacman.y == item.y) {
                    item.collected = 1;
                    state = STATE_VICTORY;
                }
            }

            // Desenhar
            al_clear_to_color(al_map_rgb(0, 0, 0));

            if (state == STATE_PLAYING) {
                draw_map();
                draw_player(&pacman);
                draw_enemy(&enemies[0]);
                draw_enemy(&enemies[1]);

                if (current_map->id == 5 && !item.collected) {
                    al_draw_filled_rectangle(
                        item.x * TILE_SIZE,
                        item.y * TILE_SIZE,
                        item.x * TILE_SIZE + TILE_SIZE,
                        item.y * TILE_SIZE + TILE_SIZE,
                        al_map_rgb(255, 255, 0)
                    );
                }

            } else {
                // Fundo preto cobrindo toda tela
                al_draw_filled_rectangle(
                    0, 0,
                    MAP_WIDTH * TILE_SIZE,
                    MAP_HEIGHT * TILE_SIZE,
                    al_map_rgb(0, 0, 0)
                );

                const char* msg1 = (state == STATE_GAME_OVER) ? "GAME OVER" : "VOCE VENCEU!";
                const char* msg2 = "Aperte ENTER para reiniciar";

                al_draw_text(
                    font,
                    al_map_rgb(255, 255, 255),
                    MAP_WIDTH * TILE_SIZE / 2,
                    MAP_HEIGHT * TILE_SIZE / 2 - 10,
                    ALLEGRO_ALIGN_CENTER,
                    msg1
                );
                al_draw_text(
                    font,
                    al_map_rgb(255, 255, 255),
                    MAP_WIDTH * TILE_SIZE / 2,
                    MAP_HEIGHT * TILE_SIZE / 2 + 10,
                    ALLEGRO_ALIGN_CENTER,
                    msg2
                );
            }

            al_flip_display();
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if ((state == STATE_GAME_OVER || state == STATE_VICTORY) &&
                ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                // Reiniciar jogo no mapa 1
                void free_all_maps();
                add_multiple_maps(5, "map.txt");
                switch_to_map(1);
                init_player(&pacman, 1, 1);
                set_enemies(enemies, 1);
                item.collected = 0;
                state = STATE_PLAYING;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = 0;
        }
    }

    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_bitmap(tiles);
    al_destroy_bitmap(spritesheet);
    al_destroy_font(font);

    return 0;
}
