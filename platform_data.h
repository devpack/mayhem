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

// edges of the map, where the ships will warp to the opposite edge
struct edge_data
    {
    int leftx, rightx;
    int topy, bottomy;
    };

struct level_data
	{
	char * bmpname;
    char * mini_bmpname;
    char * collision_bmpname;
	BITMAP * bitmap;
	PALETTE colormap;
	BITMAP * mini_bitmap;
	PALETTE mini_colormap;
    BITMAP * collision_bitmap;
	PALETTE collision_colormap;
    BITMAP * mini_bitmap_buffer;
	struct platform_data * platformdata;
	int nbplatforms;
    struct dca_data alldca[NB_DCA];
    bool use_dca;
    bool wall_collision;
    struct edge_data edgedata;
	};

void init_level_dca(struct dca_data *dca, int xsrc, int ysrc, int area, int delay);
void init_level_data(struct level_data* leveldat, char * bmpname, char *mini_bmpname, char *collision_bmpname, struct platform_data *platformdata, int nbplatforms, struct edge_data edgedata, bool use_dca, bool wall_collision);
int load_level(struct level_data* leveldat, int largeur, int hauteur);
void unload_level(struct level_data* leveldat);
#endif
