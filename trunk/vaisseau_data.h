#ifndef __VAISSEAU_DATA_H__
#define __VAISSEAU_DATA_H__
#include <allegro.h>

#include "vaisseau_gfx.h"

#define MAX_TIR 20



struct debris_data {
    int   x, y;
    fixed xposprecise,yposprecise;
    fixed ax, ay;
    fixed vx, vy;
    fixed impultion;
    int   angle;
    bool  active;
};

struct tir_data {
	int x, y;
    fixed xposprecise, yposprecise;
	fixed dx, dy;
	bool free;
};

struct vaisseau_data {
	// special for each ship
	fixed mass;
	fixed thrust_max;
	int anglestep;

    int fuel;
    int shield_force;

	int max_fuel;
	int speed_fuel_down;
	int speed_fuel_up;
    bool refueling;
	int max_shield_force;
	int speed_shield_force_down;
	int speed_shield_force_up;

    int explode_count;
    bool explode;
    int  option_type;

	int impactx,impacty;
	fixed ax,ay;
	fixed vx,vy;
	fixed xposprecise,yposprecise;
	int xpos,ypos;
	int angle;
	fixed thrust;
	bool shield;
    bool fire;
    bool fire_delay;
	bool landed;
	bool rebound;
	BITMAP   *sprite_buffer;   // Image du vaisseau  
	BITMAP	 *sprite_buffer_rota;
	struct vaisseau_gfx * gfx;
    struct tir_data tir[MAX_TIR];
    struct tir_data backtir[MAX_TIR];

    struct debris_data debris[8];  // 8 debris
  };



int init_vaisseau_data(struct vaisseau_data* v, struct vaisseau_gfx* gfx,
                              float mass, float thrust_max, int anglestep,
                              int max_fuel, int speed_fuel_down, int speed_fuel_up,
                              int max_shield_force, int speed_shield_force_down, int speed_shield_force_up);

void clean_vaisseau_data(struct vaisseau_data* v);

void init_ship_pos_from_platforms(struct vaisseau_data* v, struct platform_data * plats);

// to be called at each vbl!
void fuel_shield_calcul(int nbvaisseau, struct vaisseau_data *v);


#endif
