#include <stdio.h>

#include "player_view.h"
#include "collision.h"
#include "utils.h"
#include "platform_data.h"
#include "physics.h"

PALETTE bar_palette;

int init_player_view(struct player_view *pv, int x, int y, int w, int h,struct player_info *player)
  {
	pv->bordersize=10;

	pv->x=x;
	pv->y=y;
	pv->w=w-2*pv->bordersize;
	pv->h=h-2*pv->bordersize;
	pv->player=player;
	pv->back_map_buffer = create_clear_bitmap(w,h);
	if (!pv->back_map_buffer)
		return -1;
	return 0;
  }

void clean_player_view(struct player_view *pv)
  {
  if (pv->back_map_buffer) destroy_bitmap(pv->back_map_buffer);
  }

void draw_basic_player_view(struct player_view *v,int nbviews, BITMAP *src_map,PALETTE commonpalette)
{

    for(int i=0; i<nbviews; i++)
    {

	int color=makecol(255,255,255);
	struct vaisseau_data *ship = v->player->ship;
	set_clip(v->back_map_buffer, 0, 0, v->w+2*v->bordersize,v->h+2*v->bordersize);
	clear_bitmap(v->back_map_buffer);
	hline(v->back_map_buffer,0,0,v->w+2*v->bordersize,color);
	hline(v->back_map_buffer,0,v->h+2*v->bordersize-1,v->w+2*v->bordersize,color);
	vline(v->back_map_buffer,0,0,v->h+2*v->bordersize,color);
	vline(v->back_map_buffer,v->w+2*v->bordersize-1,0,v->h+2*v->bordersize,color);
	// blit from the backmap into screen buffer for each player
	blit(src_map, v->back_map_buffer,
		 ship->xpos-(v->w/2), ship->ypos-(v->h/2),
		 v->bordersize, v->bordersize, v->w, v->h);

	char buffer[20];
    char bufferx[20];
    char buffery[20];
    char buffervx[20];
    char buffervy[20];

    if (player_gameover(v->player))
        sprintf(buffer,"Game over");
    else 
        sprintf(buffer,"Live(s): %d",v->player->nblives);
    
	sprintf(bufferx,"x=%d",ship->xpos);
	sprintf(buffery,"y=%d",ship->ypos);
	sprintf(buffervx,"vx=%d",fixtoi(ship->vx));
	sprintf(buffervy,"vy=%d",-fixtoi(ship->vy));

	textout(v->back_map_buffer, font, v->player->name, 3, 2, color);
	textout(v->back_map_buffer, font, buffer, v->bordersize+195 , 2, color);

    textout(v->back_map_buffer, font, bufferx, 3 , v->h+v->bordersize+1, color);
    textout(v->back_map_buffer, font, buffery, 55 , v->h+v->bordersize+1, color);
    textout(v->back_map_buffer, font, buffervx, 195 , v->h+v->bordersize+1, color);
    textout(v->back_map_buffer, font, buffervy, 250 , v->h+v->bordersize+1, color);

 	int barheight_fuel=v->h*ship->fuel/ship->max_fuel;
	int barheight_shield=v->h*ship->shield_force/ship->max_shield_force;
    int fuel_col;
    int shield_col;

    if(ship->fuel>=ship->max_fuel/2)
    {
    fuel_col=255-fixtoi(fixmul(itofix(255), fixsub(fixmul(itofix(2), fixdiv(itofix(ship->fuel), itofix(ship->max_fuel)) ), itofix(1)))) ;
    fuel_col=makecol(fuel_col,255,0);
    }
    else
    {
    fuel_col=255*2*ship->fuel/ship->max_fuel;
    fuel_col=makecol(255, fuel_col, 0);
    }

    if(ship->shield_force>=ship->max_shield_force/2)
    {
    shield_col=255-fixtoi(fixmul(itofix(255), fixsub(fixmul(itofix(2), fixdiv(itofix(ship->shield_force), itofix(ship->max_shield_force)) ), itofix(1)))) ;
    shield_col=makecol(shield_col, 255, 0);
    }
    else
    {
    shield_col=255*2*ship->shield_force/ship->max_shield_force;
    shield_col=makecol(255, shield_col, 0);
    }

    vline(v->back_map_buffer,(v->bordersize/2)+1,v->h+v->bordersize,v->bordersize+(v->h-barheight_fuel),fuel_col);
    vline(v->back_map_buffer,v->bordersize/2,v->h+v->bordersize,v->bordersize+(v->h-barheight_fuel),fuel_col);
    vline(v->back_map_buffer,v->w+v->bordersize+v->bordersize/2,v->h+v->bordersize,v->bordersize+(v->h-barheight_shield),shield_col);
    vline(v->back_map_buffer,v->w+v->bordersize+(v->bordersize/2)-1,v->h+v->bordersize,v->bordersize+(v->h-barheight_shield),shield_col);
	// reset clip after
	set_clip(v->back_map_buffer, v->bordersize, v->bordersize, v->w,v->h);
    v++;
   }
}


void rotate_sprite(struct player_view * v)
	{
	struct vaisseau_data *ship = v->player->ship;

	clear_bitmap(ship->sprite_buffer_rota);
	rotate_sprite(ship->sprite_buffer_rota, ship->sprite_buffer, 0,0, itofix(ship->angle));

	}

void display_rotate_sprite_in_all_view(struct player_view * v, struct player_view allviews[], int nbviews)
{
	struct vaisseau_data *ship = v->player->ship;
	int j=0;
	for(j=0;j<nbviews;j++)
	{
		struct player_view* oppview = &allviews[j];
		struct vaisseau_data *oppship = oppview->player->ship;
        if(!v->player->ship->explode)
			{
			draw_sprite(oppview->back_map_buffer, ship->sprite_buffer_rota,
			        oppview->bordersize+oppview->w/2+ship->xpos-oppship->xpos,
					oppview->bordersize+oppview->h/2+ship->ypos-oppship->ypos);
			}
	}
  }

void init_tir(struct vaisseau_data *v)
{

     struct tir_data *shoot;

     if(v->fire && (test_place_tir(v)!=-1) && v->fire_delay)
     {
	 int place_free_tir=test_place_tir(v);
	 if (place_free_tir == -1)
		 return; // only do something IF THERE IS A FREE SHOOT !
     shoot = &v->tir[test_place_tir(v)];
     int cx;
     int cy;
     cx = v->xpos+15;
     cy = v->ypos+16;
     shoot->x = cx+fixtoi(fixmul(itofix(18),fixsin(itofix(v->angle))));
     shoot->y = cy+fixtoi(fixmul(itofix(18),-fixcos(itofix(v->angle))));
     shoot->xposprecise = itofix(shoot->x);
     shoot->yposprecise = itofix(shoot->y);
     shoot->dx = fixmul(ftofix(5.1),fixsin(itofix(v->angle)));
     shoot->dy = fixmul(ftofix(5.1),-fixcos(itofix(v->angle)));

     shoot->dx = fixadd(shoot->dx, fixdiv(v->vx, ftofix(3.5)));
     shoot->dy = fixadd(shoot->dy, fixdiv(v->vy, ftofix(3.5)));

     shoot->free=false;


     if(v->option_type==5)
     {
     struct tir_data *backshoot;
     place_free_tir=test_place_backtir(v);
	 if (place_free_tir == -1)
	 return; // only do something IF THERE IS A FREE BACKSHOOT !

     backshoot = &v->backtir[test_place_backtir(v)];

     backshoot->x = 2*(v->xpos+16) - shoot->x;
     backshoot->y = 2*(v->ypos+16) - shoot->y;
     backshoot->xposprecise = itofix(backshoot->x);
     backshoot->yposprecise = itofix(backshoot->y);
     backshoot->dx = -fixmul(ftofix(5.1),fixsin(itofix(v->angle)));
     backshoot->dy = -fixmul(ftofix(5.1),-fixcos(itofix(v->angle)));
     backshoot->free=false;
     }


     }
}

void plot_tir(struct vaisseau_data *v, struct player_view *views, int nbviews, BITMAP *src_map)
{
     struct tir_data *shoot;

     for (int i=0; i<MAX_TIR; i++)
     {
     shoot = &v->tir[i];
	 if (shoot->free)
		 continue; // only iterate through the non free shoot
	 if (testcollision_bullet4pix(src_map,shoot->x,shoot->y))
		{
		 // if there is collision we free the shoot
		 // and we go on!
		 shoot->free=true;
		 continue;
		}

	 // display in all views!
	 int nbsaveviews=nbviews;
	 struct player_view *view=views;
	 while(nbsaveviews--)
		{
		put_big_pixel(view->back_map_buffer,
					view->bordersize+view->w/2+shoot->x-view->player->ship->xpos,
					view->bordersize+view->h/2+shoot->y-view->player->ship->ypos, makecol(255,255,255));
		view++;
		}

     shoot->xposprecise = fixadd(shoot->xposprecise, shoot->dx);
     shoot->yposprecise = fixadd(shoot->yposprecise, shoot->dy);
     shoot->x = fixtoi(shoot->xposprecise);
     shoot->y = fixtoi(shoot->yposprecise);
     }


     if(v->option_type==5)
     {
     struct tir_data *backshoot;

     for (int i=0; i<MAX_TIR; i++)
     {
     backshoot = &v->backtir[i];
	 if (backshoot->free)
		 continue; // only iterate through the non free shoot
	 if (testcollision_bullet4pix(src_map,backshoot->x,backshoot->y))
		{
		 // if there is collision we free the shoot
		 // and we go on!
		 backshoot->free=true;
		 continue;
		}

	 // display in all views!
	 int nbsaveviews=nbviews;
	 struct player_view *view=views;
	 while(nbsaveviews--)
		{
		put_big_pixel(view->back_map_buffer,
		view->bordersize+view->w/2+backshoot->x-view->player->ship->xpos,
		view->bordersize+view->h/2+backshoot->y-view->player->ship->ypos, makecol(255,255,255));

		view++;
		}

     backshoot->xposprecise = fixadd(backshoot->xposprecise, backshoot->dx);
     backshoot->yposprecise = fixadd(backshoot->yposprecise, backshoot->dy);
     backshoot->x = fixtoi(backshoot->xposprecise);
     backshoot->y = fixtoi(backshoot->yposprecise);
     }
     }

}


// src_map necessary for collision detection
void gestion_tir(struct vaisseau_data *v,  struct player_view *views, int nbviews,BITMAP *src_map)
{
     init_tir(v);
     plot_tir(v,views,nbviews,src_map);
}


int test_place_tir(struct vaisseau_data *v)
{
     struct tir_data *shoot;

     for(int i=0; i<MAX_TIR; i++)
     {
     shoot = &v->tir[i];
     if(shoot->free)
		 return(i);
     }
     return(-1);
}

int test_place_backtir(struct vaisseau_data *v)
{
     struct tir_data *backshoot;

     for(int i=0; i<MAX_TIR; i++)
     {
     backshoot = &v->backtir[i];
     if(backshoot->free)
		 return(i);
     }
     return(-1);
}

void put_big_pixel(BITMAP *bmp, int x, int y, int color)
{
     putpixel(bmp, x, y, color);
     putpixel(bmp, x+1, y, color);
     putpixel(bmp, x, y-1, color);
     putpixel(bmp, x+1, y-1, color);
}

void draw_explosion(struct player_info *allpi, struct player_view *views, struct platform_data * plats, int nombre_vaisseau, int nbviews)
{
	int i;
	int j;
	for(i=0;i<nombre_vaisseau;i++)
		{
		if(allpi[i].ship->explode)
			if (allpi[i].ship->explode_count<48)
				{
				for(j=0;j<nbviews;j++)
					draw_sprite(views[j].back_map_buffer, get_sprite_explosion_frame(allpi[i].ship->explode_count),views[j].bordersize + views[j].w/2 + allpi[i].ship->xpos - allpi[j].ship->xpos, views[j].bordersize + views[j].h/2 + allpi[i].ship->ypos - allpi[j].ship->ypos);
				allpi[i].ship->explode_count++;
				}
			else
				{
                   if(allpi[i].ship->explode_count<200)
                      allpi[i].ship->explode_count++;
                   else
                   {
				   allpi[i].ship->explode_count=0;
				   allpi[i].ship->explode=false;
				   init_ship_pos_from_platforms(allpi[i].ship,&plats[i]);
                   allpi[i].nblives--;
                   }
				}
		}
}



void mega_collision_test(struct player_info *allpi, struct player_view *views, struct vaisseau_data *vaisseaux, struct level_data *currentlevel, int nbplayers, int nombre_vaisseau)
{
    struct dca_data *dca;
    dca=&currentlevel->alldca[0];

	int i;
    for(i=0;i<nbplayers;i++)
		{
        if (!allpi[i].ship->explode&&collision_debris_ship(allpi[i].ship,vaisseaux,nombre_vaisseau))
        player_exploded(&allpi[i]);

        if (!allpi[i].ship->explode&&collision_dca_ship(allpi[i].ship,dca, NB_DCA))
        player_exploded(&allpi[i]);

		if (!allpi[i].ship->explode&&collision_backtir_ship(allpi[i].ship,vaisseaux,nombre_vaisseau))
		player_exploded(&allpi[i]);

		if (!allpi[i].ship->explode&&collision_tir_ship(allpi[i].ship,vaisseaux,nombre_vaisseau))
			{
			player_exploded(&allpi[i]);
			//textout(screen, font, "SHOT   ", 50*(i+1), 50, makecol(255*(i+1)/(nombre_vaisseau),128,128));
			}
       else
			{
			if (!allpi[i].ship->explode&& !(vaisseaux[i].landed) && test_collision(&views[i],currentlevel))
				{
				player_exploded(&allpi[i]);
				//textout(screen, font, "BOOM  ", 50*(i+1), 50, makecol(255*(i+1)/(nombre_vaisseau),128,128));
				}
			//else
				//textout(screen, font, "OK    ", 50*(i+1), 50, makecol(255,255*(i+1)/(nombre_vaisseau),255));
			}
		}

	int j;
	for (i=0;i<nombre_vaisseau;i++)
		for(j=nombre_vaisseau-1;j>i;j--)
			if (!allpi[i].ship->explode&&!allpi[j].ship->explode&&test_collision_ship2ship(&vaisseaux[i],&vaisseaux[j],currentlevel->colormap))
				{
				player_exploded(&allpi[i]);
				player_exploded(&allpi[j]);
				//textout(screen, font, "DOUBLEBOOM", 350, 50, makecol(255,255,255));
				}
}


//----------------------------------------------------------------------------//
//                        Gestion des Debris
//----------------------------------------------------------------------------//

void init_debris(struct vaisseau_data *v)
{
   int angle = 16;

   for (int i=0; i<8; i++)
   {

   v->debris[i].angle = angle;
   v->debris[i].x = ((v->xpos+15) + fixtoi(fixmul(itofix(20),fixsin(itofix(v->debris[i].angle)))));
   v->debris[i].y = ((v->ypos+16) + fixtoi(fixmul(itofix(20),-fixcos(itofix(v->debris[i].angle)))));
   v->debris[i].xposprecise = itofix(v->debris[i].x);
   v->debris[i].yposprecise = itofix(v->debris[i].y);
   v->debris[i].ax = itofix(0);
   v->debris[i].ay = itofix(0);
   v->debris[i].impultion = ftofix(7.5);
   v->debris[i].vx = itofix(0);
   v->debris[i].vy = itofix(0);
   v->debris[i].active = TRUE;

   angle += 32;
   }

}

void test_collision_debris(struct vaisseau_data *v, BITMAP *src_map)
{
   for(int i=0; i<8; i++)
   {
   if(testcollision_bullet4pix(src_map, v->debris[i].x, v->debris[i].y))
   v->debris[i].active=FALSE;
   }
}


void plot_debris(struct vaisseau_data *v, struct player_view *views,const physics_constants& physics,int nbviews)
{

   for (int j=0; j<8; j++)
   {
      if(v->debris[j].active)
      {
         struct player_view *view = views;
         int nbsaveviews=nbviews;
         while(nbsaveviews--)
         {
         put_big_pixel(view->back_map_buffer, view->bordersize+view->w/2+v->debris[j].x-view->player->ship->xpos, view->bordersize+view->h/2+v->debris[j].y-view->player->ship->ypos, makecol(255,255,255));
         view++;
         }
      }
   }

   for (int i=0; i<8; i++)
   {
   if(v->debris[i].active)
   {
   v->debris[i].ax = fixmul(v->debris[i].impultion, fixsin(itofix(v->debris[i].angle)));
   v->debris[i].ay = fixadd(physics.iG, fixmul(v->debris[i].impultion, -fixcos(itofix(v->debris[i].angle))));

   v->debris[i].vx = fixadd(v->debris[i].vx, fixmul(physics.iCoeffax, v->debris[i].ax));
   v->debris[i].vy = fixadd(v->debris[i].vy, fixmul(physics.iCoeffay, v->debris[i].ay));

   v->debris[i].vx = fixmul(v->debris[i].vx, physics.iXfrott);
   v->debris[i].vy = fixmul(v->debris[i].vy, physics.iYfrott);

   v->debris[i].vx = fixadd(v->debris[i].vx, fixdiv(v->vx, ftofix(2.5)));
   v->debris[i].vy = fixadd(v->debris[i].vy, fixdiv(v->vy, ftofix(8.5)));

   v->debris[i].xposprecise = fixadd(v->debris[i].xposprecise, fixmul(physics.iCoeffvx, v->debris[i].vx));
   v->debris[i].yposprecise = fixadd(v->debris[i].yposprecise, fixmul(physics.iCoeffvy, v->debris[i].vy));

   v->debris[i].x = fixtoi(v->debris[i].xposprecise);
   v->debris[i].y = fixtoi(v->debris[i].yposprecise);

   v->debris[i].impultion = itofix(0);
   }
   }
   v->vx=itofix(0);
   v->vy=itofix(0);
}


void draw_debris(struct player_info *allpi, struct player_view *views,const physics_constants& physics, int nbviews, int nombre_vaisseau, BITMAP *src_map)
{
   for(int i=0;i<nombre_vaisseau;i++)
   {
      if(allpi[i].ship->explode_count == 1)
      init_debris(allpi[i].ship);

      if((allpi[i].ship->explode) && (allpi[i].ship->explode_count>1))
      {
         test_collision_debris(allpi[i].ship, src_map);
         plot_debris(allpi[i].ship, views, physics, nbviews);
      }

      if(!allpi[i].ship->explode)
      for(int j=0; j<8; j++)
      {
      allpi[i].ship->debris[j].active=FALSE;
      }

   }
}

//----------------------------------------------------------------------------//
//                        Gestion MiniMap
//----------------------------------------------------------------------------//


void gestion_minimap(struct vaisseau_data *vaisseaux, struct level_data *currentlevel, int nbplayers, int largeur, int hauteur)
{
    //blit(currentlevel->mini_bitmap, currentlevel->mini_bitmap_buffer, 0, 0, 0, 0, 99, 150);
    stretch_blit(currentlevel->mini_bitmap, currentlevel->mini_bitmap_buffer, 0, 0, 99, 150, 0, 0, largeur*(99/800.0), hauteur*(150/600.0));

    int x,y;
    struct vaisseau_data *v;
    for(int i=0; i<nbplayers; i++)
    {
        v=&vaisseaux[i];
        x=(v->xpos>>3)+1;
        y=(v->ypos>>3)+3;
        put_big_pixel(currentlevel->mini_bitmap_buffer, x*(largeur*(99/800.0)/99.0), y*(hauteur*(150/600.0)/150.0), makecol(255, 255, 255));
    }

    blit(currentlevel->mini_bitmap_buffer, screen, 0, 0, largeur*(350.0/800.0), hauteur*(370.0/600.0), largeur*(99/800.0), hauteur*(150/600.0));
    //stretch_blit(currentlevel->mini_bitmap_buffer, screen, 0, 0, 99, 150,largeur*(350.0/800.0), hauteur*(370.0/600.0), largeur*(99/800.0), hauteur*(150/600.0));

    clear_bitmap(currentlevel->mini_bitmap_buffer);
}

//----------------------------------------------------------------------------//
//                        Gestion DCA
//----------------------------------------------------------------------------//

void init_dca_tir(struct dca_data *dca, struct vaisseau_data *v)
{
      fixed distance;
      fixed deltax, deltay;
      fixed angle;

      deltax = fixsub(v->xposprecise+itofix(16), itofix(dca->xsrc));
      deltay = fixsub(v->yposprecise+itofix(16), itofix(dca->ysrc));
      distance = fixhypot(deltax, deltay);

      struct tir_data *dca_tir;
      if( distance<itofix(dca->area) && (test_place_dca_tir(dca)!=-1) && dca->delayed==0)
      {
         int place_free_dca_tir=test_place_dca_tir(dca);
             if(place_free_dca_tir == -1)
		     return;


         dca_tir = &(dca->dca_tir[test_place_dca_tir(dca)]);

         angle = fixacos(fixdiv(deltay, distance));

         dca_tir->x = dca->xsrc;
         dca_tir->y = dca->ysrc;

         dca_tir->xposprecise = itofix(dca_tir->x);
         dca_tir->yposprecise = itofix(dca_tir->y);

         if(deltax<itofix(0))
         dca_tir->dx = fixmul(ftofix(1.1), -fixsin(angle));
         else
         dca_tir->dx = fixmul(ftofix(1.1), fixsin(angle));

         dca_tir->dy = fixmul(ftofix(1.1), fixcos(angle));

         dca_tir->free=false;
         dca->shoot=true;
      }
}

void plot_dca_tir(struct dca_data *dca, struct vaisseau_data *v, struct player_view *views, int nbviews, BITMAP *src_map)
{
     struct tir_data *dca_tir;

     for (int i=0; i<MAX_DCA_TIR; i++)
     {
        dca_tir = &(dca->dca_tir[i]);
	    if (dca_tir->free)
        continue;
	       if (testcollision_bullet4pix(src_map,dca_tir->x,dca_tir->y))
           {
	       dca_tir->free=true;
		   continue;
		   }

        int nbsaveviews=nbviews;
        struct player_view *view=views;
        while(nbsaveviews--)
        {
        put_big_pixel(view->back_map_buffer,
					view->bordersize+view->w/2+dca_tir->x-view->player->ship->xpos,
					view->bordersize+view->h/2+dca_tir->y-view->player->ship->ypos, makecol(255,255,255));
        view++;
        }

        dca_tir->xposprecise = fixadd(dca_tir->xposprecise, dca_tir->dx);
        dca_tir->yposprecise = fixadd(dca_tir->yposprecise, dca_tir->dy);
        dca_tir->x = fixtoi(dca_tir->xposprecise);
        dca_tir->y = fixtoi(dca_tir->yposprecise);

     }
}


void gestion_dca(struct dca_data *dca, struct vaisseau_data *v,  struct player_view *views, int nbviews, BITMAP *src_map)
{

      for(int i=0; i<NB_DCA; i++)
      {
      init_dca_tir(dca, v);
      plot_dca_tir(dca, v, views, nbviews, src_map);
         if(dca->shoot)
         {
         dca->delayed++;
            if(dca->delayed > dca->delay)
            {
            dca->delayed=0;
            dca->shoot=false;
            }
         }
      dca++;
      }
}



int test_place_dca_tir(struct dca_data *dca)
{
     struct tir_data *dca_tir;

     for(int i=0; i<MAX_DCA_TIR; i++)
     {
     dca_tir = &(dca->dca_tir[i]);
     if(dca_tir->free)
		 return(i);
     }
     return(-1);
}


//----------------------------------------------------------------------------//
//                        Gestion Warp Zone
//----------------------------------------------------------------------------//


void warp_zone(struct vaisseau_data *v, int nbplayers)
{
    while(nbplayers--)
    {
    if(v->ypos<=160 && v->xpos>=174 && v->xpos<=184)
       {
       v->xpos=344;
       v->ypos=1052;
       v->xposprecise=itofix(344);
       v->yposprecise=itofix(1052);
       }
    if(1053<=v->ypos && v->xpos>=339 && v->xpos<=349)
       {
       v->xpos=179;
       v->ypos=165;
       v->xposprecise=itofix(179);
       v->yposprecise=itofix(165);
       }
    v++;
    }
}














































