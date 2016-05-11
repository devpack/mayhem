#include "platform_data.h"

void init_level_dca(struct dca_data *dca, int xsrc, int ysrc, int area, int delay)
{
   dca->xsrc=xsrc;
   dca->ysrc=ysrc;
   dca->area=area;
   dca->delayed=0;
   dca->delay=delay;
   dca->shoot=false;

   struct tir_data *dca_tir;
      for(int i=0; i<MAX_DCA_TIR; i++)
      {
      dca_tir=&(dca->dca_tir[i]);
      dca_tir->x=dca->xsrc;
      dca_tir->y=dca->ysrc;
      dca_tir->xposprecise=itofix(0);
      dca_tir->yposprecise=itofix(0);
      dca_tir->dx=itofix(0);
      dca_tir->dy=itofix(0);
      dca_tir->free=true;
      }
}


void init_level_data(struct level_data* leveldat, char * bmpname, char *mini_bmpname, char *collision_bmpname, struct platform_data *platformdata, int nbplatforms, struct edge_data edgedata, bool use_dca, bool wall_collision)
{
	leveldat->bmpname=bmpname;
    leveldat->mini_bmpname=mini_bmpname;
    leveldat->collision_bmpname=collision_bmpname;
	leveldat->nbplatforms=nbplatforms;
	leveldat->platformdata=platformdata;
    leveldat->use_dca=use_dca;
    leveldat->wall_collision=wall_collision;
    leveldat->edgedata=edgedata;
}

int load_level(struct level_data * leveldat, int largeur, int hauteur)
{
 leveldat->bitmap=load_bitmap(leveldat->bmpname,leveldat->colormap);
 leveldat->mini_bitmap=load_bitmap(leveldat->mini_bmpname,leveldat->mini_colormap);
 leveldat->collision_bitmap=load_bitmap(leveldat->collision_bmpname, leveldat->collision_colormap);
 leveldat->mini_bitmap_buffer=create_bitmap(largeur*(99/800.0), hauteur*(150/600.0));

	if (leveldat->bitmap && leveldat->mini_bitmap)
	{
		set_palette(leveldat->colormap);
		return 0;
	}
	else
		return -1;
}

void unload_level(struct level_data * leveldat)
{
   if (leveldat->bitmap) destroy_bitmap(leveldat->bitmap);
   if (leveldat->mini_bitmap) destroy_bitmap(leveldat->mini_bitmap);
   if (leveldat->mini_bitmap_buffer) destroy_bitmap(leveldat->mini_bitmap_buffer);
   if (leveldat->collision_bitmap) destroy_bitmap(leveldat->collision_bitmap);
}

