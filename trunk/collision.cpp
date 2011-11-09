#include "collision.h"
#include "utils.h"
#include "player_view.h"
#include "platform_data.h"

bool collision_testonepixel_separate(int x1,int y1, BITMAP * bmp1, int x2, int y2, BITMAP * bmp2,PALETTEPTR commonpalette)
{
	bool collided = FALSE;
	unsigned long address_bmp;              // pour le sprite
	unsigned char pixelcolor;

	bmp_select(bmp2);
    address_bmp = bmp_read_line(bmp2, y2);      // address ligne y du fond
    pixelcolor = bmp_read8(address_bmp+x2);      // lit l'octet x dans ligne y
    if ( (commonpalette[pixelcolor].r !=0) ||
		 (commonpalette[pixelcolor].g !=0) ||
		 (commonpalette[pixelcolor].b !=0))
		{
		bmp_select(bmp1);
		address_bmp = bmp_read_line(bmp1, y1);      // address ligne y du fond
		pixelcolor = bmp_read8(address_bmp+x1);      // lit l'octet x dans ligne y
	    if ( (commonpalette[pixelcolor].r !=0) ||
			 (commonpalette[pixelcolor].g !=0) ||
			 (commonpalette[pixelcolor].b !=0))
			 collided=TRUE;
		}

	return collided;
}


bool collision_testonepixel(int x,int y, BITMAP * bmp1, BITMAP * bmp2,PALETTEPTR commonpalette)
{
	return collision_testonepixel_separate(x,y,bmp1,x,y,bmp2,commonpalette);
}



bool test_collision(struct player_view * pv, struct level_data *currentlevel)
{
    struct vaisseau_data *vaisseau= pv->player->ship;

    struct platform_data *plt;
    bool test_it=true;

    for(int i=0; i<currentlevel->nbplatforms; i++)
    {
    plt= &currentlevel->platformdata[i];
    int xmin = plt->xmin - 9;                                               // centrage
    int xmax = plt->xmax - 23;                                               // centrage
    int yflat = plt->yflat - 29;

    if ((xmin<=vaisseau->xpos) && (vaisseau->xpos<=xmax) && ((vaisseau->ypos==yflat) || ((vaisseau->ypos-1)==yflat) || ((vaisseau->ypos-2)==yflat) || ((vaisseau->ypos-3)==yflat))  &&  ((vaisseau->angle&0xff)<=12 || (vaisseau->angle&0xff)>=243) )
    test_it=false;
    if (vaisseau->shield && (xmin<=vaisseau->xpos) && (vaisseau->xpos<=xmax) && ((vaisseau->ypos==yflat) || ((vaisseau->ypos-1)==yflat) || ((vaisseau->ypos-2)==yflat) || ((vaisseau->ypos-3)==yflat) || ((vaisseau->ypos+1)==yflat)) &&  ((vaisseau->angle&0xff)<=12 || (vaisseau->angle&0xff)>=243) )
    test_it=false;
    if ((vaisseau->thrust) && (xmin<=vaisseau->xpos) && (vaisseau->xpos<=xmax) && ((vaisseau->ypos==yflat) || ((vaisseau->ypos-1)==yflat) || ((vaisseau->ypos+1)==yflat) ))
    test_it=false;

   }

    if (!test_it) return test_it;

	BITMAP * little_screen = sprite_buffer_screen();
    // 32*32 de back_map_buffer2 ou sera blitté le sprite -> little_screen
    blit(pv->back_map_buffer, little_screen, pv->bordersize+pv->w/2, pv->bordersize+pv->h/2, 0, 0, 32, 32);

    int y;                                                   // loop pour toute les lignes
    int x;                                                   // loop pour ts les octet ds ligne
	int xorig,yorig;

	xorig=0;
	yorig=0;
	int size=31;

	while(size>=1)
	{
	for(x=xorig;x<xorig+size;x++)
		{
		if (collision_testonepixel(x,yorig,vaisseau->sprite_buffer_rota,little_screen,vaisseau->gfx->sprite_colors))
			return TRUE;
		}
	for(y=yorig;y<yorig+size;y++)
		{
		if (collision_testonepixel(xorig+size,y,vaisseau->sprite_buffer_rota,little_screen,vaisseau->gfx->sprite_colors))
			return TRUE;
		}
	for(x=xorig+size;x>=xorig+1;x--)
		{
		if (collision_testonepixel(x,yorig+size,vaisseau->sprite_buffer_rota,little_screen,vaisseau->gfx->sprite_colors))
			return TRUE;
		}
	for(y=yorig+size;y>=yorig+1;y--)
		{
		if (collision_testonepixel(xorig,y,vaisseau->sprite_buffer_rota,little_screen,vaisseau->gfx->sprite_colors))
			return TRUE;
		}
	xorig+=1;
	yorig+=1;
	size-=2;
	}

	return FALSE;

}

bool pixel_collision_detect_inbox(BITMAP *bmp1,int xl1, int yt1,BITMAP *bmp2,int xl2, int yt2,int w, int h,PALETTEPTR commonpalette)
{
	int x,y;
	for (x=0;x<w;x++)
		for(y=0;y<h;y++)
			if (collision_testonepixel_separate(xl1+x,yt1+y,bmp1,xl2+x,yt2+y,bmp2,commonpalette))
				return TRUE;
	return FALSE;
}

//
// Implementation de la detection des collisions entre vaisseaux
//@First we look for a bounding box, if none, sure, there are no collision
//  If there is, we find the limit of the bounding box and
//  we pixel iterate through it...
//

bool test_collision_ship2ship(struct vaisseau_data * vaisseau1,struct vaisseau_data *vaisseau2,PALETTEPTR commonpalette)
{
	// first find the bouding box
	int xl1,xl2,yt1,yt2;
	int w,h;

	if (vaisseau1->xpos+32>=vaisseau2->xpos && vaisseau1->xpos<=vaisseau2->xpos)
	{
		xl2=0;
		xl1=vaisseau2->xpos - vaisseau1->xpos;
		w=vaisseau1->xpos+32-vaisseau2->xpos;
	}
	else if (vaisseau2->xpos+32>=vaisseau1->xpos && vaisseau2->xpos<=vaisseau1->xpos)
	{
		xl1=0;
		xl2=vaisseau1->xpos - vaisseau2->xpos;
		w=vaisseau2->xpos+32-vaisseau1->xpos;
	}
	else return FALSE;

	if (vaisseau1->ypos+32>=vaisseau2->ypos &&  vaisseau1->ypos<=vaisseau2->ypos)
	{
		yt2=0;
		yt1=vaisseau2->ypos - vaisseau1->ypos;
		h=vaisseau1->ypos+32-vaisseau2->ypos;
	}
	else if (vaisseau2->ypos+32>=vaisseau1->ypos && vaisseau2->ypos<=vaisseau1->ypos)
	{
		yt1=0;
		yt2=vaisseau1->ypos - vaisseau2->ypos;
		h=vaisseau2->ypos+32-vaisseau1->ypos;
	}
	else return FALSE;

	// if we arrive here we might have a bounding box
	// with collision

	return 	pixel_collision_detect_inbox(vaisseau1->sprite_buffer_rota,xl1,yt1,
										 vaisseau2->sprite_buffer_rota,xl2,yt2,
										 w,h,commonpalette);
}

bool testcollision_bullet4pix(BITMAP *bmp,int x,int y)
{

    if(x<0 || x>=bmp->w || y<0 || y>=bmp->h) return TRUE;
    else
    {
	unsigned long address_bmp;              // pour le sprite
	unsigned char pixelcolor;

	bmp_select(bmp);
	int j;
	for(j=y-1;j<=y;j++)
		{
		address_bmp = bmp_read_line(bmp, j);      // address ligne j {y-1,y} du fond
		pixelcolor = bmp_read8(address_bmp+x);      // lit l'octet x dans ligne y
		if (pixelcolor != 0) // i pray that black == 0!
			return true;
		pixelcolor = bmp_read8(address_bmp+x+1);      // lit l'octet x dans ligne y
		if (pixelcolor != 0)
			return true;
		}
   }
	return false;
}

bool testcollision_bullet1pix(BITMAP *bmp,int x,int y,PALETTEPTR palette)
{
	bmp_select(bmp);
	int pixel=bmp_read8(bmp_read_line(bmp, y)+x);      // lit l'octet x dans ligne y
	return palette[pixel].r!=0 || palette[pixel].g!=0 || palette[pixel].b!=0 ;
}

bool collision_tir_ship(struct vaisseau_data * v,struct vaisseau_data *allv, int nombre_vaisseau)
{
	 struct tir_data * shoot;

	 while (nombre_vaisseau--)
		{
         for (int i=0; i<MAX_TIR; i++)
		    {
			shoot = &allv->tir[i];
			if (shoot->free)
				 continue; // only iterate through the non free shoot

			int xtrans=shoot->x-v->xpos;
			int ytrans=shoot->y-v->ypos;

			// if outside of the bounding box
			if (xtrans<0 || xtrans>=32 || ytrans<0 || ytrans>=32)
				continue;

			bool b=testcollision_bullet1pix(v->sprite_buffer_rota,xtrans,ytrans,v->gfx->sprite_colors);
			if (xtrans+1 < 32)
			{
				if (ytrans-1>=0)
					{
					b |= testcollision_bullet1pix(v->sprite_buffer_rota,xtrans+1,ytrans-1,v->gfx->sprite_colors);
					b |= testcollision_bullet1pix(v->sprite_buffer_rota,xtrans,ytrans-1,v->gfx->sprite_colors);
					}
				b |= testcollision_bullet1pix(v->sprite_buffer_rota,xtrans+1,ytrans,v->gfx->sprite_colors);
			}
			else if (ytrans-1>=0)
				{
				b |= testcollision_bullet1pix(v->sprite_buffer_rota,xtrans,ytrans-1,v->gfx->sprite_colors);
				}
			if (b)
				{
				shoot->free=true;
				bool isProtecting=(v->shield && v->shield_force>0);
				if (isProtecting && !v->landed)
					{
					v->impactx=fixtoi(shoot->dx);
					v->impacty=fixtoi(shoot->dy);
					}
				return !isProtecting;
				}
		 }
		 allv++;
	 }
	return false;
}

bool collision_backtir_ship(struct vaisseau_data * v,struct vaisseau_data *allv, int nombre_vaisseau)
{
	 struct tir_data * backshoot;

	 while (nombre_vaisseau--)
		{
         for (int i=0; i<MAX_TIR; i++)
		    {
			backshoot = &allv->backtir[i];
			if (backshoot->free)
				 continue; // only iterate through the non free shoot

			int xtrans=backshoot->x-v->xpos;
			int ytrans=backshoot->y-v->ypos;

			// if outside of the bounding box
			if (xtrans<0 || xtrans>=32 || ytrans<0 || ytrans>=32)
				continue;

			bool b=testcollision_bullet1pix(v->sprite_buffer_rota,xtrans,ytrans,v->gfx->sprite_colors);
			if (xtrans+1 < 32)
			{
				if (ytrans-1>=0)
					{
					b |= testcollision_bullet1pix(v->sprite_buffer_rota,xtrans+1,ytrans-1,v->gfx->sprite_colors);
					b |= testcollision_bullet1pix(v->sprite_buffer_rota,xtrans,ytrans-1,v->gfx->sprite_colors);
					}
				b |= testcollision_bullet1pix(v->sprite_buffer_rota,xtrans+1,ytrans,v->gfx->sprite_colors);
			}
			else if (ytrans-1>=0)
				{
				b |= testcollision_bullet1pix(v->sprite_buffer_rota,xtrans,ytrans-1,v->gfx->sprite_colors);
				}
			if (b)
				{
				backshoot->free=true;
				bool isProtecting=(v->shield && v->shield_force>0);
				if (isProtecting && !v->landed)
					{
					v->impactx=fixtoi(backshoot->dx);
					v->impacty=fixtoi(backshoot->dy);
					}
				return !isProtecting;
				}

		 }
		 allv++;
	 }
	return false;
}



bool collision_debris_ship(struct vaisseau_data * v,struct vaisseau_data *allv, int nombre_vaisseau)
{
	 struct debris_data * debris;

	 while (nombre_vaisseau--)
		{
         for (int i=0; i<8; i++)
		    {
			debris = &allv->debris[i];

            if(!debris->active)
            continue;

			int xtrans=debris->x-v->xpos;
			int ytrans=debris->y-v->ypos;

			// if outside of the bounding box
			if (xtrans<0 || xtrans>=32 || ytrans<0 || ytrans>=32)
				continue;

			bool b=testcollision_bullet1pix(v->sprite_buffer_rota,xtrans,ytrans,v->gfx->sprite_colors);
			if (xtrans+1 < 32)
			{
				if (ytrans-1>=0)
					{
					b |= testcollision_bullet1pix(v->sprite_buffer_rota,xtrans+1,ytrans-1,v->gfx->sprite_colors);
					b |= testcollision_bullet1pix(v->sprite_buffer_rota,xtrans,ytrans-1,v->gfx->sprite_colors);
					}
				b |= testcollision_bullet1pix(v->sprite_buffer_rota,xtrans+1,ytrans,v->gfx->sprite_colors);
			}
			else if (ytrans-1>=0)
				{
				b |= testcollision_bullet1pix(v->sprite_buffer_rota,xtrans,ytrans-1,v->gfx->sprite_colors);
				}
			if (b)
				{
				debris->active=false;
				bool isProtecting=(v->shield && v->shield_force>0);
				if (isProtecting && !v->landed)
					{
					v->impactx=fixtoi(debris->vx);
					v->impacty=fixtoi(debris->vy);
					}
				return !isProtecting;
				}
		 }
		 allv++;
	 }
	return false;
}


bool collision_dca_ship(struct vaisseau_data * v,struct dca_data *alldca, int nb_dca)
{
	 struct tir_data * dca_tir;

	 while (nb_dca--)
		{
         for (int i=0; i<MAX_DCA_TIR; i++)
		    {
			dca_tir = &alldca->dca_tir[i];

            if(dca_tir->free)
            continue;

			int xtrans=dca_tir->x-v->xpos;
			int ytrans=dca_tir->y-v->ypos;

			// if outside of the bounding box
			if (xtrans<0 || xtrans>=32 || ytrans<0 || ytrans>=32)
				continue;

			bool b=testcollision_bullet1pix(v->sprite_buffer_rota,xtrans,ytrans,v->gfx->sprite_colors);
			if (xtrans+1 < 32)
			{
				if (ytrans-1>=0)
					{
					b |= testcollision_bullet1pix(v->sprite_buffer_rota,xtrans+1,ytrans-1,v->gfx->sprite_colors);
					b |= testcollision_bullet1pix(v->sprite_buffer_rota,xtrans,ytrans-1,v->gfx->sprite_colors);
					}
				b |= testcollision_bullet1pix(v->sprite_buffer_rota,xtrans+1,ytrans,v->gfx->sprite_colors);
			}
			else if (ytrans-1>=0)
				{
				b |= testcollision_bullet1pix(v->sprite_buffer_rota,xtrans,ytrans-1,v->gfx->sprite_colors);
				}
			if (b)
				{
				dca_tir->free=true;
				bool isProtecting=(v->shield && v->shield_force>0);
				if (isProtecting && !v->landed)
					{
					v->impactx=fixtoi(dca_tir->dx);
					v->impacty=fixtoi(dca_tir->dy);
					}
				return !isProtecting;
				}
		 }
		 alldca++;
	 }
	return false;
}



