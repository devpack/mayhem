#include <allegro.h>

#include "game_mgr.h"

#define FULLSCREEN 1

#ifdef FULLSCREEN
#define GFXOPENARG GFX_AUTODETECT
#else
#define GFXOPENARG GFX_AUTODETECT_WINDOWED
#endif

volatile int InterruptTimer::timing_counter;

void GameManager::Init()
{
  allegro_init();                                            // Initialise Allegro
  InterruptTimer::init();
  set_color_depth(8);                                        // Combien de bitplan
  install_keyboard();                                        // Installe le clavier
  // GP TODO? THROW EXCEPTION?
  install_sound(DIGI_AUTODETECT , MIDI_AUTODETECT , NULL);
  set_gfx_mode( GFXOPENARG, DEFAULT_WIDTH, DEFAULT_HEIGHT, 0, 0 );          // test ecran
  //set_gfx_mode( GFX_AUTODETECT_WINDOWED, DEFAULT_WIDTH, DEFAULT_HEIGHT, 0, 0 );          // windowed
}

void GameManager::Shutdown()
{
  remove_sound();
  InterruptTimer::shutdown();
  allegro_exit();
}

void GameManager::Run(GameSequence *aSeq)
{
  while(aSeq)
  {
   aSeq=aSeq->run();
  }
}

