#ifndef __UTILS_H__
#define __UTILS_H__

#include <allegro.h>

BITMAP* create_clear_bitmap(int w,int h);
int __assert_success(int error,char *info);
int create_sprite_buffer_screen();
BITMAP* sprite_buffer_screen();
void clean_sprite_buffer_screen();

#endif