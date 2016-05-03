#include "game_mgr.h"


#include "collision.h"
#include "physics.h"
#include "platform_data.h"
#include "player_info.h"
#include "player_view.h"
#include "utils.h"
#include "vaisseau_data.h"
#include "vaisseau_gfx.h"
#include "mapping_key.h"
#include "keyboard_input.h"
#include "soundfx.h"
#include "command.h"
#include "option.h"


#ifdef __NETSUPPORT__
#define _TYPEDEFSOCKET_ 1
#define FD_SETSIZE 2
#include "netgame.h"
#endif


#define CHECKFPS 1

#define NB_MAX_VAISSEAU 4
#define NB_MAX_PLAYERS (NB_MAX_VAISSEAU)
#define NB_MAX_TYPE_VAISSEAU 4
#define NB_LEVELS 6

#define NB_OPTIONS 1

class BattleSequence : public GameSequence
{
public:
       BattleSequence(GameSequence *previous,int nbviews, int nbplayers, int nblives, int level, bool usedca, bool wall_collision, int s_width, int s_height);
       ~BattleSequence();
private:
       void InitLevelData();
       void InitMappingAndControls();
       void InitAllSpriteGfx();
       void InitPlayerInfo();
       void InitPlayerViews();
       void InitSoundFx();
       bool Gameover();
private:
       GameSequence* doRun();

private:
#ifdef __NET_SUPPORT__
        netgameclient gameclient;
        netgameserver gameserver;
#endif
        int screen_width;
        int screen_height;

        int    nb_views;
        int    nb_players;
        int    nb_lives;
        bool use_dca;
        bool wall_collision;
        struct vaisseau_data vaisseaux[NB_MAX_VAISSEAU];
        struct mapping_key keyvaisseau[NB_MAX_VAISSEAU];
        struct command commands[NB_MAX_VAISSEAU];
  // for each player we will need one of this structure initialization is made just after
  // allegro is actually started so that the bitmap can get created.

       struct level_data levels[NB_LEVELS];
       struct level_data * currentlevel;
       struct player_info players[NB_MAX_PLAYERS];
       struct player_view views[NB_MAX_PLAYERS];
       struct vaisseau_gfx gfx_vaisseaux[NB_MAX_TYPE_VAISSEAU];
       struct soundfx sounds[NB_MAX_PLAYERS];
       struct option_data opt[NB_OPTIONS];
       physics_constants moon_physics;
};
