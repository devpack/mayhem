#include "option.h"

void option_time(struct option_data *opt)
{
   if(!opt->active)
   {
      if(opt->time_out > opt->inactive_time)
      {
      opt->active=true;
      opt->time_out=0;
      }
      else opt->time_out++;
   }
   else
   {
      if(opt->time_in > opt->active_time)
      {
      opt->active=false;
      opt->time_in=0;
      }
      else opt->time_in++;
   }
}

bool test_pos_option(struct option_data *opt, struct level_data *currentlevel, int x, int y)
{
   unsigned long address_bmp;
   unsigned char pixelcolor;
   bmp_select(currentlevel->bitmap);
   int ligne;

   for(ligne=0; ligne < opt->option_sprite->h; ligne++)
   {
      address_bmp = bmp_read_line(currentlevel->bitmap, ligne + y);
      for(int colonne=0; colonne < opt->option_sprite->w; colonne++)
      {
      pixelcolor = bmp_read8(address_bmp + colonne + x);
      if ((currentlevel->colormap[pixelcolor].r !=0) ||
          (currentlevel->colormap[pixelcolor].g !=0) ||
          (currentlevel->colormap[pixelcolor].b !=0))
      return(false);
      }
   }
   return(true);
}

void init_option(struct option_data *opt, struct level_data *currentlevel)
{

   if(opt->time_in==0)
   {
   opt->x = rand()%792;
   opt->y = rand()%1200;

      if(!test_pos_option(opt, currentlevel, opt->x, opt->y))
      {
      opt->print_it=false;
      opt->active=false;
      opt->time_out = opt->active_time+1;
      }
      else opt->print_it=true;

   opt->type = rand()%NB_OPT_TYPE;
   }
}

int take_option(struct option_data *opt, struct vaisseau_data *allv, int nbplayers)
{
   struct vaisseau_data *v;
   for(int i=0; i<nbplayers; i++)
   {
   v = &allv[i];
   if(abs((v->xpos+16)-(opt->x+6))<=(26+12)/2 && abs((v->ypos+16)-(opt->y+6))<=(28+12)/2)
   return i;
   }
   return -1;
}

void attrib_option(struct option_data *opt, struct vaisseau_data *allv, int test)
{
   struct vaisseau_data *v;
   if(test!=-1)
   {
   v = &allv[test];
   v->option_type = opt->type;

   if(opt->type == 0)                  // 0 = fuel max
   v->fuel = v->max_fuel;

   if(opt->type == 1)                  // 1 = 1/2 fuel
   v->fuel = v->fuel/2;

   if(opt->type == 2) {                // 2 = more time shield on
   v->max_shield_force = 214;
   v->speed_shield_force_down = 1;
   }

   if(opt->type == 3) {                // 3 = 1/2 shield
   v->speed_shield_force_down = 2;
   v->max_shield_force = v->max_shield_force/2;
   }


   opt->active=false;
   opt->time_out=0;
   opt->time_in=0;

   }
   if(v->option_type == 4 && !(v->landed||v->rebound))             // 4 = ship rotate
   v->angle = v->angle + 1;

}

void draw_option(struct option_data *opt, struct player_view *views, int nbplayers)
{
   if(opt->print_it)
   {
      struct player_view *view=views;
      for(int i=0; i<nbplayers; i++)
      {
      draw_sprite(view->back_map_buffer, opt->option_sprite,
      view->bordersize+view->w/2+opt->x-view->player->ship->xpos,
      view->bordersize+view->h/2+opt->y-view->player->ship->ypos);
      view++;
      }
   }
}

void gestion_option(struct option_data *opt, struct level_data *currentlevel, struct vaisseau_data *allv, struct player_view *views, int nbplayers, int nbviews)
{
   int test;
   option_time(opt);                         // alternance option active ou pas

   if(opt->active)
   {
   init_option(opt, currentlevel);           // init la pos + type de l'option

   test = take_option(opt, allv, nbplayers); // -1 = pas prise, sinon num du vaisseau qui prend option
   attrib_option(opt, allv, test);           // gestion type option avec num vaisseau

   draw_option(opt, views, nbviews);       // affiche sprite option

   }
}



int init_option_data(struct option_data *opt, char *option_sprite_name, int active_time, int inactive_time)
{
   srand(time(NULL));

   opt->active = false;
   opt->time_out = 0;
   opt->time_in = 0;
   opt->type = 0;
   opt->x = rand()%792;
   opt->y = rand()%1200;
   opt->print_it=false;
   opt->active_time = active_time;
   opt->inactive_time = inactive_time;

   opt->option_sprite = load_bitmap(option_sprite_name,opt->option_sprite_colors);
   if(opt->option_sprite) return(0);
   else return(-1);
  
}

void unload_option(struct option_data *opt)
{
   if (opt->option_sprite) destroy_bitmap(opt->option_sprite);
}

