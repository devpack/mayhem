#ifndef __COLLISION_H__
#define __COLLISION_H__
#include <allegro.h>
#include "vaisseau_data.h"

typedef struct RGB * PALETTEPTR;

bool test_collision(struct player_view * pv, struct level_data *currentlevel);
bool test_collision_ship2ship(struct vaisseau_data * vaisseau1,struct vaisseau_data *vaisseau2, PALETTEPTR commonpalette);
bool testcollision_bullet4pix(BITMAP *bmp,int x,int y);
bool collision_tir_ship(struct vaisseau_data * v,struct vaisseau_data *allv, int nombre_vaisseau);
bool collision_backtir_ship(struct vaisseau_data * v,struct vaisseau_data *allv, int nombre_vaisseau);

bool collision_debris_ship(struct vaisseau_data * v,struct vaisseau_data *allv, int nombre_vaisseau);
bool collision_dca_ship(struct vaisseau_data * v,struct dca_data *alldca, int nb_dca);

void bounce_vaisseau(struct vaisseau_data *vaisseau);
#endif
