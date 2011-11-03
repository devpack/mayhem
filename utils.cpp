#include "utils.h"

BITMAP* create_clear_bitmap(int w,int h)
	{
	BITMAP *buffer;
	buffer = create_bitmap(w, h);     // place pour big image de fond
	clear_bitmap(buffer);   // Nettoyage
	return buffer;
	}

int __assert_success(int error,char *info)
  {
	if (error)                                  // 
	{  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);          //
		 allegro_message(info);                     //
	}
	return error;
  }

BITMAP * screen_sprite_buffer=NULL;

int create_sprite_buffer_screen()
{
  screen_sprite_buffer = create_clear_bitmap(32, 32);                // create bitmap pour le petit buffer
  return !screen_sprite_buffer;
}

BITMAP* sprite_buffer_screen()
{
	return screen_sprite_buffer;
}

void clean_sprite_buffer_screen()
{
	if (screen_sprite_buffer) destroy_bitmap(screen_sprite_buffer);
}