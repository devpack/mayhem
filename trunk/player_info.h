#ifndef __PLAYER_INFO_H__
#define __PLAYER_INFO_H__
#include "vaisseau_data.h" 

#define MAXPLAYERNAME 20

struct player_info {
	 char name[MAXPLAYERNAME];
	 int nblives;
	 int score;
	 struct vaisseau_data * ship;
};

void init_player_info(struct player_info* player, char *name,int nblives, struct vaisseau_data * ship);
void player_exploded(struct player_info* player);

#endif