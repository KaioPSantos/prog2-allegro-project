#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
    int x, y;
    int s,si;
} Player;

void init_player(Player* p, int x, int y);
void update_player(Player* p);
void draw_player(Player* p);

#endif
