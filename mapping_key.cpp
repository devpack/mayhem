#include "mapping_key.h"

void init_mapping_key(struct mapping_key* keymap, struct command* cmd,
	                    int left,int right,int thrust, int shield, int fire)
  {
	keymap->cmd=cmd;
	keymap->left = left;
	keymap->right = right;
	keymap->shield = shield;
	keymap->thrust = thrust;
    keymap->fire = fire;
  }

