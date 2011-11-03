#ifndef __OPTION_H__
#define __OPTION_H__

#include <allegro.h>
#include "player_view.h"
#include "vaisseau_data.h"
#include "platform_data.h"

#define  NB_OPT_POS  6
#define  NB_OPT_TYPE 6


struct option_data {
   int    x, y;
   int    type;
   int    time_in;
   int    time_out;
   bool   active;
   bool   print_it;
   int    active_time;
   int    inactive_time;
   BITMAP *option_sprite;
   PALETTE option_sprite_colors;
};

void unload_option(struct option_data *opt);
int init_option_data(struct option_data *opt, char *option_sprite_name, int active_time, int inactive_time);
void gestion_option(struct option_data *opt, struct level_data *currentlevel,struct vaisseau_data *allv, struct player_view *views, int nbplayers, int nbviews);

#endif
