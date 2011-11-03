#ifndef __SOUNDFX_H__
#define __SOUNDFX_H__

#include <allegro.h>
#include "vaisseau_data.h"

struct loopedsfx
{
  SAMPLE *sfx;
  bool playingFlag;
  int freqStep;
  int startFreq;
};

struct soundfx
{
 struct loopedsfx thrust;  // loop
 struct loopedsfx shield;  // loop
 struct loopedsfx refuel;  // loop with a freq twist ;)
 SAMPLE *shoot;  // one shot
 SAMPLE *boom; // one shot
 SAMPLE *rebound; // one shot
};

int init_soundfx_from_wavfile(struct soundfx* sfx,
							  char * thrustwav,
							  char *shieldwav,
							  char *refuelwav,
							  char *shootwav,
							  char *boomwav,
							  char * rebound);

void cleanup_soundfx(struct soundfx* sfx);
void play_soundfx_from_shipdata(struct soundfx *sfx,struct vaisseau_data* v);

#endif

