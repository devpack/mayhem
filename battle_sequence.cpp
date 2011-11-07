#include <string.h>
#include <stdio.h>

#include "battle_sequence.h"
#include "platform_data.h"




                                //------------------//
                                //     VARIABLES    //
                                //------------------//

  struct platform_data level1[] =
  { { 504, 568, 985 }, { 464, 513, 333 },	{ 428, 497, 531 },	{ 178, 241, 875 },
    { 8, 37, 187 },
	{ 302, 351, 271 },
	{ 434, 521, 835 },
	{ 60, 127, 1045 },
	{ 499, 586, 1165 } };

  struct platform_data level2[] =
  {
    { 552, 615, 513 },
    { 201, 259, 175 },
	{ 546, 599, 327 },
	{ 660, 697, 447 },
	{ 350, 435, 621 },
	{ 468, 525, 915 },
	{ 596, 697, 1141 },
	{ 21, 92, 1087 } };

  struct platform_data level3[] =
  { { 565, 616, 459 },
    { 14, 65, 111 },
	{ 343, 398, 207 },
	{ 713, 760, 231 },
	{ 473, 540, 617 },
	{ 316, 385, 805 },
	{ 492, 548, 987 },
	{ 66, 145, 1180 },
	{ 38, 93, 1121 } };






// init currentlevel with level
BattleSequence::BattleSequence(GameSequence *previous,int nbviews, int nbplayers, int level, int s_width, int s_height)
  : GameSequence(previous),moon_physics(0.07,0.984,0.99,0.6,0.6,0.6,0.6,0.2)
#ifdef __NET_SUPPORT__
     , gameclient(3000,"localhost"), gameserver(3000)
#endif
{
  screen_width = s_width;
  screen_height = s_height;

  nb_views = nbviews;
  nb_players = nbplayers;
  currentlevel=&levels[level];
  //
  InitLevelData();
  InitMappingAndControls();
  InitAllSpriteGfx();
  InitSoundFx();
  InitPlayerInfo();
  InitPlayerViews();
  // TODO ASSERT?
  load_level(currentlevel, screen_width, screen_height);
}

BattleSequence::~BattleSequence()
{
  clean_sprite_buffer_screen();
  // stop sound
  cleanup_soundfx(&sounds[0]);
  cleanup_soundfx(&sounds[1]);
  if(nb_views>=3)
    cleanup_soundfx(&sounds[2]);
  if(nb_views>=4)
    cleanup_soundfx(&sounds[3]);

  int i;
  for(i=0;i<nb_views;i++)		// cleanup buffers for each player ship
	clean_player_view(&views[i]);

  for(i=0;i<nb_players;i++)		// cleanup buffers for each player ship
	clean_vaisseau_data(&vaisseaux[i]);

  for(i=0;i<NB_MAX_TYPE_VAISSEAU;i++)	// cleanup the buffers for each ship type
	cleanup_vaisseau_gfx(&gfx_vaisseaux[i]);

  cleanup_sprite_explosion();
  unload_option(opt);
  unload_level(currentlevel);
}

void BattleSequence::InitLevelData()
{
  init_level_data(&levels[0],"Mayhem_Level1_Map_256c.bmp", "Mini_map1.bmp", level1,10);
                                       // x    y  area  delay
  init_level_dca(&(&levels[0])->alldca[0], 766, 85, 150, 25);
  init_level_dca(&(&levels[0])->alldca[1], 170, 481, 90, 25);
  init_level_data(&levels[1],"Mayhem_Level2_Map_256c.bmp", "Mini_map2.bmp", level2,8);
  init_level_dca(&(&levels[1])->alldca[0], 647, 273, 150, 25);
  init_level_dca(&(&levels[1])->alldca[1], 267, 947, 90, 25);
  init_level_data(&levels[2],"Mayhem_Level3_Map_256c.bmp", "Mini_map3.bmp", level3,9);
  init_level_dca(&(&levels[2])->alldca[0], 180, 555, 90, 25);
  init_level_dca(&(&levels[2])->alldca[1], 152, 1012, 90, 25);
}

void BattleSequence::InitMappingAndControls()
{
  // init command
  // Here we have to pad the right keys
  init_mapping_key(&keyvaisseau[1],&commands[1],KEY_LEFT,KEY_RIGHT,KEY_DEL_PAD,KEY_0_PAD,KEY_ENTER_PAD);
  init_mapping_key(&keyvaisseau[0],&commands[0],KEY_Z,KEY_X,KEY_V,KEY_C,KEY_G);

  if(nb_views>=3)
    init_mapping_key(&keyvaisseau[2],&commands[2],KEY_B,KEY_N,KEY_COMMA,KEY_M,KEY_L);

  if(nb_views>=4)
    init_mapping_key(&keyvaisseau[3],&commands[3],KEY_Y,KEY_U,KEY_O,KEY_I,KEY_0);


  commands[0].controlled_ship=&vaisseaux[0];
  commands[1].controlled_ship=&vaisseaux[1];
  if(nb_views>=3)
    commands[2].controlled_ship=&vaisseaux[2];
  if(nb_views>=4)
    commands[3].controlled_ship=&vaisseaux[3];
}

void BattleSequence::InitAllSpriteGfx()
{
  // init the gfx for the first vaisseau type
  init_vaisseau_gfx_from_file(&gfx_vaisseaux[0],"ship1_256c.bmp",
	                                           "ship1_thrust_256c.bmp",
												 "ship1_shield_256c.bmp");
  init_vaisseau_gfx_from_file(&gfx_vaisseaux[1],"ship2_256c.bmp",
	                                            "ship2_thrust_256c.bmp",
												"ship2_shield_256c.bmp");
  init_vaisseau_gfx_from_file(&gfx_vaisseaux[2],"ship3_256c.bmp",
	                                            "ship3_thrust_256c.bmp",
                                                "ship3_shield_256c.bmp");
  init_vaisseau_gfx_from_file(&gfx_vaisseaux[3],"ship4_256c.bmp",
	                                            "ship4_thrust_256c.bmp",
												"ship4_shield_256c.bmp");
  init_sprite_explosion("sprite_explosion.bmp");

  for(int i=0;i<nb_players;i++)
    init_vaisseau_data(&vaisseaux[i],&gfx_vaisseaux[i],0.9,0.32,5,1284,1,8,214,2,2);
 //  time active, inactive
  init_option_data(opt, "Option.bmp", 500, 50);
}

void BattleSequence::InitPlayerInfo()
{
  // init player info
  char *defplayername[] = { "Player 1" , "Player 2" , "Player 3" , "Player 4" };
  for(int i=0;i<nb_players;i++)
    {
    init_player_info(&players[i],defplayername[i],20,&vaisseaux[i]);
	init_ship_pos_from_platforms(&vaisseaux[i],&(currentlevel->platformdata[i]));
    }
}


void BattleSequence::InitPlayerViews()
{

  if (nb_views == 1)
  {
  init_player_view(&views[0],90,100,300,260,&players[0]);
  }
  else if (nb_views == 2)
  {
  init_player_view(&views[0], screen_width*(90.0/800.0) , screen_height*(100.0/600.0), screen_width*(300.0/800.0), screen_height*(260.0/600.0),&players[0]);
  init_player_view(&views[1], screen_width*(410.0/800.0) , screen_height*(100.0/600.0) ,screen_width*(300.0/800.0), screen_height*(260.0/600.0),&players[1]);
  }
  else if (nb_views == 3)
  {
  init_player_view(&views[0], screen_width*(90/800.0), screen_height*(40/600.0),screen_width*(300.0/800.0),screen_height*(260.0/600.0),&players[0]);
  init_player_view(&views[1], screen_width*(410/800.0) ,screen_height*(40/600.0),screen_width*(300.0/800.0),screen_height*(260.0/600.0),&players[1]);
  init_player_view(&views[2], screen_width*(40/800.0) , screen_height*(310/600.0),screen_width*(300.0/800.0),screen_height*(260.0/600.0),&players[2]);
  }
  else if (nb_views == 4)
  {
  init_player_view(&views[0], screen_width*(90/800.0), screen_height*(40/600.0),screen_width*(300.0/800.0),screen_height*(260.0/600.0),&players[0]);
  init_player_view(&views[1], screen_width*(410/800.0) ,screen_height*(40/600.0),screen_width*(300.0/800.0),screen_height*(260.0/600.0),&players[1]);
  init_player_view(&views[2], screen_width*(40/800.0), screen_height*(310/600.0),screen_width*(300.0/800.0),screen_height*(260.0/600.0),&players[2]);
  init_player_view(&views[3], screen_width*(460/800.0), screen_height*(310/600.0),screen_width*(300.0/800.0),screen_height*(260.0/600.0),&players[3]);
  }
}

void BattleSequence::InitSoundFx()
{
init_soundfx_from_wavfile(&sounds[0],"sfx_loop_thrust.WAV","sfx_loop_shield.WAV","sfx_loop_refuel.WAV","sfx_shoot.WAV","sfx_boom.WAV","sfx_rebound.WAV");
init_soundfx_from_wavfile(&sounds[1],"sfx_loop_thrust.WAV","sfx_loop_shield.WAV","sfx_loop_refuel.WAV","sfx_shoot.WAV","sfx_boom.WAV","sfx_rebound.WAV");
if (nb_views >= 3)
    init_soundfx_from_wavfile(&sounds[2],"sfx_loop_thrust.WAV","sfx_loop_shield.WAV","sfx_loop_refuel.WAV","sfx_shoot.WAV","sfx_boom.WAV","sfx_rebound.WAV");
if (nb_views >= 4)
    init_soundfx_from_wavfile(&sounds[3],"sfx_loop_thrust.WAV","sfx_loop_shield.WAV","sfx_loop_refuel.WAV","sfx_shoot.WAV","sfx_boom.WAV","sfx_rebound.WAV");

create_sprite_buffer_screen();
}

GameSequence* BattleSequence::doRun()
{
  int i; // for everythign counter

#ifdef CHECKFPS
  int check_fps=0;
  int retrace_count_init=retrace_count;
#endif

  bool isRunning=true;
  InterruptTimer::start();
  while(isRunning)
  {
  while(InterruptTimer::wasTriggered()) {
	  // quick hack for level change
		if (key[KEY_1])
		{
		unload_level(currentlevel);
		currentlevel=&levels[0];
		load_level(currentlevel, screen_width, screen_height);
		for(i=0;i<nb_players;i++)
			init_ship_pos_from_platforms(&vaisseaux[i],&(currentlevel->platformdata[i]));

		}
		else if (key[KEY_2])
		{
		unload_level(currentlevel);
		currentlevel=&levels[1];
		load_level(currentlevel, screen_width, screen_height);
		for(i=0;i<nb_players;i++)
			init_ship_pos_from_platforms(&vaisseaux[i],&(currentlevel->platformdata[i]));
		}
		else if (key[KEY_3])
		{
		unload_level(currentlevel);
		currentlevel=&levels[2];
		load_level(currentlevel, screen_width, screen_height);
		for(i=0;i<nb_players;i++)
			init_ship_pos_from_platforms(&vaisseaux[i],&(currentlevel->platformdata[i]));
		}
        else if(key[KEY_ESC])
        {
         isRunning=false;
         break;
        }

  get_input_clavier(nb_views,keyvaisseau);   // Clavier (only one... the other comes from .net)

#ifdef __NETSUPPORT__
  struct command  *cmdptr=keyvaisseau[0].cmd;
  if (memcmp(&netpadcmd,cmdptr,sizeof(struct command)))
	{
	static int count=0;
	static int miss=0;
	char bf[20];
	sprintf(bf,"net send:%d",count++);
	textout(screen,font,bf,700,50,makecol(255,255,255));
	if (!gameclient.send(keyvaisseau[0].cmd))
		{
		sprintf(bf,"send miss:%d",miss++);
		textout(screen,font,bf,700,60,makecol(255,255,255));
		}
	}
  if (gameserver.recv(netpadcmd))
	{
	static int count=0;
	char bf[20];
	sprintf(bf,"net recv:%d",count++);
	textout(screen,font,bf,700,80,makecol(255,255,255));
	netpadcmd.controlled_ship=&vaisseaux[0];
	}

  handle_command(&netpadcmd);
#else
  if(!vaisseaux[0].explode)
	  handle_command(keyvaisseau[0].cmd);
#endif
  if(!vaisseaux[1].explode)
	handle_command(keyvaisseau[1].cmd);

  if(nb_views>=3)
    if(!vaisseaux[2].explode)
        handle_command(keyvaisseau[2].cmd);

  if(nb_views>=4)
    if(!vaisseaux[3].explode)
        handle_command(keyvaisseau[3].cmd);

  calcul_pos(moon_physics,nb_players,vaisseaux,currentlevel->platformdata,currentlevel->nbplatforms);  // Position
  fuel_shield_calcul(nb_players,vaisseaux);
  // sound both player
  play_soundfx_from_shipdata(&sounds[0],&vaisseaux[0]);
  play_soundfx_from_shipdata(&sounds[1],&vaisseaux[1]);
  if(nb_views>=3)
    play_soundfx_from_shipdata(&sounds[2],&vaisseaux[2]);
  if(nb_views>=4)
    play_soundfx_from_shipdata(&sounds[3],&vaisseaux[3]);


   draw_basic_player_view(views, nb_views, currentlevel->bitmap, currentlevel->colormap);

   // first we rotate the sprite then display the shooting
   for(i=0;i<nb_players;i++)
		rotate_sprite(&views[i]);

  mega_collision_test(players, views, vaisseaux, currentlevel, nb_views, nb_players);

  gestion_option(opt, currentlevel,vaisseaux, views, nb_players,nb_views);


   for(i=0;i<nb_players;i++)
      gestion_tir(&vaisseaux[i], views, nb_views,currentlevel->bitmap);

    if(USE_DCA!=0) {
        for(i=0;i<nb_players;i++)
            gestion_dca(&currentlevel->alldca[0], &vaisseaux[i], views, nb_views, currentlevel->bitmap);
    }

    draw_explosion(players, views, currentlevel->platformdata, nb_players, nb_views);
    draw_debris(players, views, moon_physics, nb_players, nb_views, currentlevel->bitmap);
    gestion_minimap(vaisseaux, currentlevel, nb_players, screen_width, screen_height);

    if(currentlevel==&levels[0])
       warp_zone(vaisseaux, nb_players);

	for(i=0;i<nb_players;i++)
		display_rotate_sprite_in_all_view(&views[i],views,nb_views);
   // back_map_buffer dans screen
	for(i=0;i<nb_views;i++)
		{
		struct player_view* v = &views[i];
		blit(v->back_map_buffer, screen, 0, 0, v->x, v->y, v->w+2*v->bordersize, v->h+2*v->bordersize);
		}

#ifdef CHECKFPS
    check_fps++;
	if (check_fps == 100)
	{
		char fps[10];
		sprintf(fps,"fps=%.1f",check_fps*70.0/(retrace_count-retrace_count_init));
		textout(screen,font,fps,5,5,makecol(200,200,200));

		char reso[10];
		sprintf(reso, "%ix%i", screen_width, screen_height);
        textout(screen,font, reso ,5,17,makecol(200,200,200));

		check_fps=0;
		retrace_count_init=retrace_count;
	}
#endif
  vsync();                                                   // wait the raster
  } // eof while(InterruptTimer())
  } // eof while (isRunning)
  InterruptTimer::reset();

  return  ReturnScreen();
}
