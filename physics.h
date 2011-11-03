#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include <allegro.h>

#include "vaisseau_data.h"
#include "platform_data.h"

class physics_constants {
public:
  physics_constants( float g, float xfrott, float yfrott,
					float coeffax,float coeffvx,float coeffay,float coeffvy,float coeffimpact);
public:
  fixed    iG;                              // cte gravi
  fixed    iXfrott;                         // frottement en x
  fixed    iYfrott;                         // frottement en y
  fixed    iCoeffax;                        // pour dV = coeffa A
  fixed    iCoeffvx;                        // pour dX = coeffv V
  fixed    iCoeffay;                        // pour dV = coeffa A
  fixed    iCoeffvy;                        // pour dX = coeffv V
  fixed	   iCoeffimpact;					// when shooted on
};


void calcul_pos(const physics_constants& physics, int nbvaisseau,struct vaisseau_data *vaisseau,struct platform_data platforms[],int nbplatforms);
bool test_landed(struct vaisseau_data* vaisseau, struct platform_data *plt);

#endif