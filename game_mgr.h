#ifndef __GAMEMANAGER_H_
#define __GAMEMANAGER_H_

#include <allegro.h>

#define  DEFAULT_WIDTH  1024
#define  DEFAULT_HEIGHT 768

class GameSequence
{
public:
   GameSequence(GameSequence *returnScreen)
                      : iReturnScreen(returnScreen) {}
   virtual ~GameSequence(){};
   GameSequence* run() { GameSequence* s=doRun(); if (s!=iReturnScreen && iReturnScreen) delete iReturnScreen; return s; };


protected:
   virtual GameSequence* doRun() = 0 ;
   GameSequence *ReturnScreen() const { return iReturnScreen; };
private:
   GameSequence *iReturnScreen;
};


class GameManager
{
public:
   static void Init();
   static void Shutdown();
   static void Run(GameSequence *aSeq);
};


class InterruptTimer
{
public:
	static void init() { reset(); install_timer(); install_int(&InterruptTimer::irq, 25); };	// install interrupt timer 1/40s (25 ms)
    static void shutdown() { remove_timer(); };
	static volatile int timing_counter;
	inline static void start() { timing_counter = 0; };
	inline static void reset() { timing_counter = -1; };
	static void irq() { if (timing_counter>=0) ++timing_counter;	};
	//static bool wasTriggered() { if (timing_counter>0) { timing_counter--; return true; } return false; };
    //Something wierd is going on here... Under windows 10 you dont get 40 fps, and with the above code you end up with a LOT more..  Something isn't pegging the interupt properly.
    static bool wasTriggered() { if (timing_counter>0) { timing_counter=0; return true; } return false; };
};


#endif
