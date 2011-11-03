#ifndef __GAMEMANAGER_H_
#define __GAMEMANAGER_H_

#include <allegro.h>

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
public:
   static const int largeur; 
   static const int hauteur;  // Taille fenetre video affichable

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
	static bool wasTriggered() { if (timing_counter>0) { timing_counter--; return true; } return false; };
};


#endif
