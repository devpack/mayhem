#include <stdio.h>
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
	iDoublebuffer=create_bitmap(INTRO_SCREEN_WIDTH,maxi-mini);

	width = DEFAULT_WIDTH;
	height = DEFAULT_HEIGHT;
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
    int levelchoice=0;
    char strlevelchoice[50];

	set_palette(iLogoPalette);
	clear_bitmap(screen);
	InterruptTimer::start();

	do
	{
//		while(InterruptTimer::wasTriggered()) {
            iZoom=fixsub(iZoom,iZoomSpeed);
            if (fixtof(iZoom)<1.0)
                {
                iZoom=itofix(1);
                isRunning=false;
                }
            clear_bitmap(iDoublebuffer);
            DrawZoomedLogoInCenter(mini,maxi);
            // draw 2 horizontal lines
            hline(iDoublebuffer, 0,0,INTRO_SCREEN_WIDTH,makecol(255,255,255));
            hline(iDoublebuffer, 0,IntroSequence::maxi-IntroSequence::mini-1,INTRO_SCREEN_WIDTH,makecol(255,255,255));
            // blit to the screen
            blit(iDoublebuffer,screen,0,0,0,mini,INTRO_SCREEN_WIDTH,maxi-mini);
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
//		}
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
                //if(currentcolor==red)
                    //currentcolor=black;
                //else
                    currentcolor=red;
                }
            if (key[KEY_ESC])
                break;
            if (key[KEY_ENTER])
                {
                choice=2;
                break;
                }
            if (key[KEY_F2])
                {
                choice=2;
                break;
                }
            if (key[KEY_F3])
                {
                choice=3;
                break;
                }
            if (key[KEY_F4])
                {
                choice=4;
                break;
                }
            if (key[KEY_F5])
                {
                levelchoice += 1;
                if(levelchoice == NB_LEVELS) levelchoice = 0;
                //short rest to stop multiple key presses
                rest(100);
                }
            if (key[KEY_MINUS_PAD] || key[KEY_MINUS_PAD])
                {
                width = 800;
                height = 600;
                set_gfx_mode( GFX_AUTODETECT, width, height, 0, 0 );
                }
            if (key[KEY_PLUS_PAD] || key[KEY_PLUS_PAD])
                {
                width = 1280;
                height = 800;
                set_gfx_mode( GFX_AUTODETECT, width, height, 0, 0 );
                }
            if (key[KEY_ASTERISK])
                {
                width = 1024;
                height = 768;
                set_gfx_mode( GFX_AUTODETECT, width, height, 0, 0 );
                }

            textout_centre(screen, font, "[ Press ENTER to play or (F2/F3/F4 for 2/3/4 players) or ESC to leave ]", INTRO_SCREEN_WIDTH/2, maxi+5, currentcolor);
            snprintf(strlevelchoice, sizeof(strlevelchoice), "[ Press F5 to change level ] - %d", levelchoice + 1);
            textout_centre(screen, font, strlevelchoice, INTRO_SCREEN_WIDTH/2, maxi+15, currentcolor);
            
            vsync();
        } while (1);
	}
	clear_bitmap(screen);

	GameSequence * seq;
	if (choice)
		{
		iZoom=iZoomMax;
		seq=new BattleSequence(this,choice,choice,20,levelchoice, width, height);
		}
	else
		seq=ReturnScreen();

	return seq;
}

void IntroSequence::DrawZoomedLogoInCenter(int y1,int y2)
{
	int width=fixtoi(fixmul(itofix(iLogo->w),iZoom));
	int height=fixtoi(fixmul(itofix(iLogo->h),iZoom));
	int targetwidth=INTRO_SCREEN_WIDTH;
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
	//void stretch_blit(BITMAP *source, BITMAP *dest, int source_x, source_y, source_width, source_height, int dest_x, dest_y, dest_width, dest_height);
}
