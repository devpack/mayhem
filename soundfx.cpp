#include "soundfx.h"

void init_loopedsfx(struct loopedsfx* l,char *wav,int freqStep)
{
 l->sfx=load_sample(wav);
 l->playingFlag=false;
 l->freqStep=freqStep;
 l->startFreq=1000;
}

int init_soundfx_from_wavfile(struct soundfx* sfx,char * thrustwav,char *shieldwav,char *refuelwav, char *shootwav, char *boomwav,char * rebound)
{
 init_loopedsfx(&(sfx->thrust),thrustwav,0);
 init_loopedsfx(&(sfx->shield),shieldwav,0);
 init_loopedsfx(&(sfx->refuel),refuelwav,5);
 sfx->shoot = load_sample(shootwav);
 sfx->boom = load_sample(boomwav);
 sfx->rebound = load_sample(rebound);
 return !(sfx->thrust.sfx && sfx->shield.sfx && sfx->refuel.sfx && sfx->shoot && sfx->boom && sfx->rebound);
}

void cleanup_soundfx(struct soundfx *sfx)
{
 if (sfx->thrust.sfx)
    destroy_sample(sfx->thrust.sfx);
 if (sfx->shield.sfx)
    destroy_sample(sfx->shield.sfx);
 if (sfx->refuel.sfx)
    destroy_sample(sfx->refuel.sfx);
 if (sfx->shoot)
    destroy_sample(sfx->shoot);
 if (sfx->boom)
    destroy_sample(sfx->boom);
 if (sfx->rebound)
    destroy_sample(sfx->rebound);
}

void play_looped_sample(struct loopedsfx *l, bool active)
{
   if ( !active && !l->playingFlag)
   {
	return;
   }
   if (active && l->playingFlag)
	   {
		if (l->freqStep) 
			{
			l->startFreq+=l->freqStep;
			adjust_sample(l->sfx, 128, 128, l->startFreq,1);
			}
		return;
		}
	
   if (active)
      {
       play_sample(l->sfx,128,128, l->startFreq, 1);
      }
   else
      {
       stop_sample(l->sfx);
	   l->startFreq=1000;
      }
   l->playingFlag=active;
}

void play_soundfx_from_shipdata(struct soundfx *sfx,struct vaisseau_data* v)
{
  play_looped_sample(&sfx->thrust,(v->thrust>0));
  play_looped_sample(&sfx->shield,(v->shield && v->shield_force>0));
  play_looped_sample(&sfx->refuel,(v->refueling));
  if (v->rebound)
	play_sample(sfx->rebound,128,128,1000,0);
  if (v->fire_delay)
	play_sample(sfx->shoot,128,128,1000,0);
  if (v->explode && v->explode_count == 1) // TODO, fix this is disgusting...
    play_sample(sfx->boom,128,128,1000,0);
}


