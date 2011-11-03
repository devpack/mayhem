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


void init_level_data(struct level_data* leveldat, char * bmpname,char *mini_bmpname, struct platform_data *platformdata,int nbplatforms)
{
	leveldat->bmpname=bmpname;
    leveldat->mini_bmpname=mini_bmpname;
	leveldat->nbplatforms=nbplatforms;
	leveldat->platformdata=platformdata;
}

int load_level(struct level_data * leveldat)
{
 leveldat->bitmap=load_bitmap(leveldat->bmpname,leveldat->colormap);
 leveldat->mini_bitmap=load_bitmap(leveldat->mini_bmpname,leveldat->mini_colormap);
 leveldat->mini_bitmap_buffer=create_bitmap(99,150);

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
}

