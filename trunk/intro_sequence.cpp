#include "intro_sequence.h"
#include "battle_sequence.h"

const int IntroSequence::mini=150;
const int IntroSequence::maxi=400;

IntroSequence::IntroSequence(GameSequence* previous,float zoom, float zoomspeed)
	: GameSequence(previous)
{
	iLogo=load_bitmap("intro_logo.bmp",iLogoPalette);
	iZoomMax=iZoom=ftofix(zoom);
	iZoomSpeed=ftofix(zoomspeed);
	iDoublebuffer=create_bitmap(GameManager::largeur,maxi-mini);
}


IntroSequence::~IntroSequence()
{
   if (iLogo)
	   destroy_bitmap(iLogo);
}

GameSequence* IntroSequence::doRun()
{
	bool isRunning=true;
	bool quickExit=false;
	int tempo=0;
	bool canQuickExit=false;

	set_palette(iLogoPalette);
	clear_bitmap(screen);
	InterruptTimer::start();
	do
	{
		while(InterruptTimer::wasTriggered()) {
		iZoom=fixsub(iZoom,iZoomSpeed);
		if (fixtof(iZoom)<1.0)
			{
			iZoom=itofix(1);
			isRunning=false;
			}
		clear_bitmap(iDoublebuffer);
		DrawZoomedLogoInCenter(mini,maxi);
		// draw 2 horizontal lines
		hline(iDoublebuffer, 0,0,GameManager::largeur,makecol(255,255,255));
		hline(iDoublebuffer, 0,IntroSequence::maxi-IntroSequence::mini-1,GameManager::largeur,makecol(255,255,255));
		// blit to the screen
		blit(iDoublebuffer,screen,0,0,0,mini,GameManager::largeur,maxi-mini);
		if (tempo>25)
			{
			tempo=0;
			canQuickExit=true;
			}
		else
			tempo++;

		if (key[KEY_ESC]&&canQuickExit)
			{
			quickExit=true;
			isRunning=false;
			}
		vsync();
		}
	} while(isRunning);
	InterruptTimer::reset();


	int choice=0;
	if (!quickExit)
	{
	tempo=0;
	int black=makecol(0,0,0);
	int red=makecol(255,0,0);
	int currentcolor=red;
	do
	{

		if(tempo++ == 50)
			{
			tempo=0;
			if(currentcolor==red)
				currentcolor=black;
			else
				currentcolor=red;
			}
		if (key[KEY_ESC])
			break;
		if (key[KEY_ENTER])
			{
			choice=1;
			break;
			}
		textout_centre(screen, font, "[ Press ENTER to play or ESC to leave ]", GameManager::largeur/2, maxi+5, currentcolor);

		vsync();
	} while (1);
	}
	clear_bitmap(screen);

	GameSequence * seq;
	if (choice)
		{
		iZoom=iZoomMax;
		seq=new BattleSequence(this,2,2,0);
		}
	else
		seq=ReturnScreen();

	return seq;
}

void IntroSequence::DrawZoomedLogoInCenter(int y1,int y2)
{
	int width=fixtoi(fixmul(itofix(iLogo->w),iZoom));
	int height=fixtoi(fixmul(itofix(iLogo->h),iZoom));
	int targetwidth=GameManager::largeur;
	int targetheight=y2-y1;

	int xs,ys,ws,hs;
	int xd,yd,wd,hd;

	if (width>targetwidth)
		{
		ws=fixtoi(fixdiv(itofix(targetwidth),iZoom));
		xs=(iLogo->w-ws)/2;
		xd=0;
		wd=targetwidth;
		}
	else
		{
		xs=0;
		ws=iLogo->w;
		xd=(targetwidth-width)/2;
		wd=width;
		}

	if (height>targetheight)
		{
		hs=fixtoi(fixdiv(itofix(targetheight),iZoom));
		ys=(iLogo->h-hs)/2;
		yd=0;
		hd=targetheight;
		}
	else
		{
		ys=0;
		hs=iLogo->h;
		yd=(targetheight-height)/2;
		hd=height;
		}
	stretch_blit(iLogo,iDoublebuffer,xs,ys,ws,hs,xd,yd,wd,hd);
}
