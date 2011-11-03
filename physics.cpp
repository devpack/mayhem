#include "physics.h"


physics_constants::physics_constants(float g, float xfrott, float yfrott,
							float coeffax,float coeffvx,float coeffay,float coeffvy,float coeffimpact)
{
	iG=ftofix(g);
	iXfrott=ftofix(xfrott);
	iYfrott=ftofix(yfrott);
	iCoeffax=ftofix(coeffax);
	iCoeffay=ftofix(coeffay);
	iCoeffvx=ftofix(coeffvx);
	iCoeffvy=ftofix(coeffvy);
	iCoeffimpact=ftofix(coeffimpact);
}


void calcul_pos(const physics_constants& physics,int nbvaisseau,struct vaisseau_data *vaisseau,struct platform_data platforms[],int nbplatforms)
{
	while(nbvaisseau--)
	{
	  vaisseau->rebound = FALSE;
      if (!vaisseau->explode)
      if (!vaisseau->landed)
      {
      vaisseau->ax = fixmul(vaisseau->thrust, fixsin(itofix(vaisseau->angle)));            // ax = thrust * sinµ
      vaisseau->ay = fixadd(physics.iG, fixmul(vaisseau->thrust, -fixcos(itofix(vaisseau->angle))));// ay = g + thrust * (-cosµ)
	  // gestion impact
	  if (vaisseau->impactx || vaisseau->impacty)
		{
		vaisseau->ax = fixadd(vaisseau->ax, fixmul(physics.iCoeffimpact,itofix(vaisseau->impactx)));
		vaisseau->ay = fixadd(vaisseau->ay, fixmul(physics.iCoeffimpact,itofix(vaisseau->impacty)));
		vaisseau->impactx=vaisseau->impacty=0;
		}

	  vaisseau->vx = fixadd(vaisseau->vx, fixmul(physics.iCoeffax, vaisseau->ax));                   // vx += coeffa * ax
      vaisseau->vy = fixadd(vaisseau->vy, fixmul(physics.iCoeffay, vaisseau->ay));                   // vy += coeffa * ay


      vaisseau->vx = fixmul(vaisseau->vx, physics.iXfrott);                               // on freine de xfrott
	  vaisseau->vy = fixmul(vaisseau->vy, physics.iYfrott);                               // on freine de yfrott

      vaisseau->xposprecise = fixadd(vaisseau->xposprecise,fixmul(physics.iCoeffvx, vaisseau->vx));   // xpos += coeffv * vx
      vaisseau->yposprecise = fixadd(vaisseau->yposprecise,fixmul(physics.iCoeffvy, vaisseau->vy));   // ypos += coeffv * vy
      }                                                      
      else
      {
      vaisseau->ax = itofix(0);                                        // on
      vaisseau->ay = itofix(0);                                        //    est
      vaisseau->vx = itofix(0);                                        //        pose
      vaisseau->vy = itofix(0);                                        //             !!!
      }


	  // transfer to screen coordinates
	  vaisseau->xpos=fixtoi(vaisseau->xposprecise);
	  vaisseau->ypos=fixtoi(vaisseau->yposprecise);

	  int i;
	  for(i=0;i<nbplatforms;i++)
		{
		  vaisseau->rebound |= test_landed (vaisseau,&platforms[i]);
		} 
	  vaisseau++;
	}
}

//----------------------------------------------------------------------------//
//                    TEST SI POSE SUR PLATEFORME                             //
//                updates ship speed if posed 
//				  returns TRUE if rebound (or landed)
//----------------------------------------------------------------------------//


bool test_landed(struct vaisseau_data* vaisseau, struct platform_data *plt)
{
    int xmin = plt->xmin - 9;                                               // centrage
    int xmax = plt->xmax - 23;                                               // centrage
    int yflat = plt->yflat - 29;                                             // centrage

	if ((xmin<=vaisseau->xpos) && (vaisseau->xpos<=xmax) &&
	   ((vaisseau->ypos==yflat) || ((vaisseau->ypos-1)==yflat) || ((vaisseau->ypos-2)==yflat) || ((vaisseau->ypos-3)==yflat) ) &&
		(fixtoi(vaisseau->vy)>0) &&  ((vaisseau->angle&0xff)<=12 || (vaisseau->angle&0xff)>=243) )
		{
		vaisseau->vy = -fixdiv( vaisseau->vy, ftofix(1.2)); 
		vaisseau->vx = fixdiv( vaisseau->vx, ftofix(1.1)); 
		vaisseau->angle = 0;
        vaisseau->ypos=yflat;
        vaisseau->yposprecise=itofix(yflat);

		float vvx=fixtof(vaisseau->vx);
		float vvy=fixtof(vaisseau->vy);
		if ( (-1.0<=vvx) && (vvx<1.0) && (-1.0<vvy) && (vvy<1.0) )
			{
			vaisseau->landed = TRUE; 
			}
		return TRUE;	
		}
	return FALSE;
}		   

