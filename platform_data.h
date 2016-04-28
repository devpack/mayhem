#ifndef __PLATFORM_DATA__
#define __PLATFORM_DATA__
#include <allegro.h>
#include "vaisseau_data.h"

#define USE_DCA 0
#define NB_DCA 2
#define MAX_DCA_TIR 3

struct dca_data {
    int xsrc, ysrc;
    int area;
    int delayed;
    int delay;
    bool shoot;
    struct tir_data dca_tir[MAX_DCA_TIR];
};

struct platform_data
  {
	int xmin,xmax;
	int yflat;
  };

struct level_data
	{
	char * bmpname;
    char * mini_bmpname;
	BITMAP * bitmap;
	PALETTE colormap;
	BITMAP * mini_bitmap;
	PALETTE mini_colormap;
    BITMAP * mini_bitmap_buffer;
	struct platform_data * platformdata;
	int nbplatforms;
    struct dca_data alldca[NB_DCA];
    bool use_dca;
    bool wall_collision;
	};

void init_level_dca(struct dca_data *dca, int xsrc, int ysrc, int area, int delay);
void init_level_data(struct level_data* leveldat, char * bmpname, char *mini_bmpname, struct platform_data *platformdata, int nbplatforms, bool use_dca, bool wall_collision);
int load_level(struct level_data* leveldat, int largeur, int hauteur);
void unload_level(struct level_data* leveldat);
#endif
