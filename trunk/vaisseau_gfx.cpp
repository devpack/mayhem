#include "vaisseau_gfx.h"

bool init_vaisseau_gfx_from_file(struct vaisseau_gfx* vaisseau,char * normal,char * thrust, char * shield)
  {
	vaisseau->sprite=load_bitmap(normal,vaisseau->sprite_colors);
	vaisseau->sprite_shield=load_bitmap(shield,vaisseau->sprite_shield_colors);
	vaisseau->sprite_thrust=load_bitmap(thrust,vaisseau->sprite_thrust_colors);
	return ! (vaisseau->sprite && vaisseau->sprite_shield && vaisseau->sprite_thrust);
  }


void cleanup_vaisseau_gfx(struct vaisseau_gfx* vaisseau)
  {
	if (vaisseau->sprite) destroy_bitmap(vaisseau->sprite);
	if (vaisseau->sprite_shield) destroy_bitmap(vaisseau->sprite_shield);
	if (vaisseau->sprite_thrust) destroy_bitmap(vaisseau->sprite_thrust);
  }

BITMAP  *sprite_explode;
PALETTE sprite_explode_color;
BITMAP  * sprite_explosion_frames[24];

int init_sprite_explosion(char *bmpname)
{
  sprite_explode = load_bitmap(bmpname, sprite_explode_color);
  int i;
  for(i=0;i<24;i++)
  {
	sprite_explosion_frames[i]=create_bitmap(32,32);
	blit(sprite_explode,sprite_explosion_frames[i],32*i, 0,0,0,32,32);
  }
  return (!sprite_explode);
}

void cleanup_sprite_explosion()
{
  int i;
  for(i=0;i<24;i++)
	  destroy_bitmap(sprite_explosion_frames[i]);
  destroy_bitmap(sprite_explode);
}

BITMAP* get_sprite_explosion()
{
	return sprite_explode;
}

BITMAP* get_sprite_explosion_frame(int i)
{
	return sprite_explosion_frames[i/2];
}