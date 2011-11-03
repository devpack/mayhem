#ifndef __INTROSEQUENCE_H__
#define __INTROSEQUENCE_H__

#include "game_mgr.h"

class IntroSequence : public GameSequence
{
public:
   IntroSequence(GameSequence* previous,float zoom=5.0,float zoomspeed=0.1);
   ~IntroSequence();
   void DrawZoomedLogoInCenter(int y1,int y2);
private:
   static const int mini, maxi;
   GameSequence* doRun();
   fixed iZoom;
   fixed iZoomMax;
   fixed iZoomSpeed;
   BITMAP* iLogo;
   PALETTE iLogoPalette;
   BITMAP* iDoublebuffer;
};

#endif