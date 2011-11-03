#ifndef __VAISSEAU_GFX_H__
#define __VAISSEAU_GFX_H__

#include <allegro.h>

  struct vaisseau_gfx {
	BITMAP * sprite;
	BITMAP * sprite_thrust;
	BITMAP * sprite_shield;
	PALETTE  sprite_colors;
	PALETTE  sprite_thrust_colors;
	PALETTE  sprite_shield_colors; 
  };



bool init_vaisseau_gfx_from_file(struct vaisseau_gfx* vaisseau,char * normal,char * thrust, char * shield);
void cleanup_vaisseau_gfx(struct vaisseau_gfx* vaisseau);
int init_sprite_explosion(char *bmpname);
BITMAP* get_sprite_explosion();
void cleanup_sprite_explosion();
BITMAP* get_sprite_explosion_frame(int i);
#endif