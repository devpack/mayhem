#include "player_info.h"
#include <string.h>

void init_player_info(struct player_info* player, char *name,int nblives, struct vaisseau_data * ship)
  {
	strncpy(player->name,name,MAXPLAYERNAME);
	player->nblives=nblives;
	player->score=0;
	player->ship=ship;
  }

void player_exploded(struct player_info* player)
{
	//player lives are now reduced when the explosion finishes to deal with gameover 
    //player->nblives--;
	player->ship->explode=true;
    player->ship->thrust=itofix(0);
    player->ship->shield=false;
    player->ship->fire=false;
}

bool player_gameover(struct player_info* player)
{
    return((player->nblives < 0) ? true : false);
}