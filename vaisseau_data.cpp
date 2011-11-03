#include "vaisseau_data.h"
#include "platform_data.h"

int init_vaisseau_data(struct vaisseau_data* v, struct vaisseau_gfx* gfx,
                              float mass, float thrust_max, int anglestep,
                              int max_fuel, int speed_fuel_down, int speed_fuel_up,
                              int max_shield_force, int speed_shield_force_down, int speed_shield_force_up)
  {
	v->thrust_max=ftofix(thrust_max);
	v->mass=ftofix(mass);
	v->anglestep=anglestep;

    v->max_fuel=max_fuel;
    v->speed_fuel_down=speed_fuel_down;
    v->speed_fuel_up=speed_fuel_up;
    v->max_shield_force=max_shield_force;
    v->speed_shield_force_down=speed_shield_force_down;
    v->speed_shield_force_up=speed_shield_force_down;
	int i;
    for (i=0; i<MAX_TIR; i++)
		{
		v->tir[i].x=0;
		v->tir[i].y=0;
        v->tir[i].xposprecise=itofix(0);
        v->tir[i].yposprecise=itofix(0);
		v->tir[i].dx=itofix(0);
		v->tir[i].dy=itofix(0);
		v->tir[i].free=true;
		}

    for (i=0; i<MAX_TIR; i++)
		{
		v->backtir[i].x=0;
		v->backtir[i].y=0;
        v->backtir[i].xposprecise=itofix(0);
        v->backtir[i].yposprecise=itofix(0);
		v->backtir[i].dx=itofix(0);
		v->backtir[i].dy=itofix(0);
		v->backtir[i].free=true;
		}

   for (i=0; i<8; i++)
       {
       v->debris[i].x=0;
       v->debris[i].y=0;
       v->debris[i].xposprecise = itofix(0);
       v->debris[i].yposprecise = itofix(0);
       v->debris[i].ax=itofix(0);
       v->debris[i].ay=itofix(0);
       v->debris[i].vx=itofix(0);
       v->debris[i].vy=itofix(0);
       v->debris[i].impultion=itofix(0);
       v->debris[i].angle=0;
       v->debris[i].active=FALSE;
       }


	v->gfx=gfx;
	v->sprite_buffer = create_bitmap(32, 32);                     // create bitmap pour le sprite
	if (! v->sprite_buffer)
		return -1;
	clear_bitmap(v->sprite_buffer);                               // On nettoye
	blit(gfx->sprite, v->sprite_buffer, 0, 0, 0, 0, 32, 32); // blit le sprite dans son buffer

	v->sprite_buffer_rota = create_bitmap(32, 32);                     // create bitmap pour le sprite
	if (! v->sprite_buffer_rota)
		return -1;
	clear_bitmap(v->sprite_buffer_rota);                               // On nettoye
	return 0;
  }

void clean_vaisseau_data(struct vaisseau_data* v)
  {
	if (v->sprite_buffer) destroy_bitmap(v->sprite_buffer);                               // On nettoye
	if (v->sprite_buffer_rota) destroy_bitmap(v->sprite_buffer_rota);                          // On nettoye
  }

void init_ship_pos_from_platforms(struct vaisseau_data* v, struct platform_data * plats)
{	
	int xpos,ypos;
	xpos=(plats->xmin+plats->xmax)/2-16;
	ypos=plats->yflat-29;

	v->xpos=xpos;
	v->ypos=ypos;
	v->xposprecise=itofix(xpos);
	v->yposprecise=itofix(ypos);
    v->fuel=v->max_fuel;
    v->shield_force=v->max_shield_force;

    v->explode_count=0;
    v->explode=FALSE;

    v->option_type=0;

	v->impactx=0;
	v->impacty=0;
	v->ax=itofix(0);
	v->ay=itofix(0);
	v->vx=itofix(0);
	v->vy=itofix(0);
	v->thrust=itofix(0);
	v->shield=FALSE;
    v->fire=FALSE;
    v->fire_delay=FALSE;
	v->landed=TRUE;
	v->rebound=FALSE;
	v->angle=0;
	v->refueling=false;
}


void fuel_shield_calcul(int nbvaisseau, struct vaisseau_data *v)
{
 while(nbvaisseau--)
 {
  v->refueling=false;
  if (v->fire_delay) v->fuel-=3;
  if (!(v->landed||v->rebound) && (v->thrust!=itofix(0))&&(v->fuel>0))
        v->fuel-= v->speed_fuel_down; 
  else if( (v->landed||v->rebound) && (v->thrust==itofix(0)) && (v->fuel<v->max_fuel))
  {
        v->fuel+= v->speed_fuel_up;
		v->refueling=true;
  }
  if (v->fuel<0) v->fuel=0;
  if (v->fuel>v->max_fuel) v->fuel=v->max_fuel;
  

   if (v->shield && v->shield_force>0)
      v->shield_force-= v->speed_shield_force_down;
   else if(!v->shield && v->shield_force<v->max_shield_force)
      v->shield_force+= v->speed_shield_force_up;

  if (v->shield_force<0) v->shield_force=0;
  if (v->shield_force>v->max_shield_force) v->shield_force=v->max_shield_force;
  
   v++;
 }
}


