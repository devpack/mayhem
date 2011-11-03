#include "keyboard_input.h"


void get_input_clavier(int nbmapping,struct mapping_key* keymap)
	{
	while(nbmapping--)
		{
		struct command *cmd=keymap->cmd;
		cmd->left=key[keymap->left];
		cmd->right=key[keymap->right];
		cmd->shield=key[keymap->shield];
		cmd->fire=key[keymap->fire];
		cmd->thrust=key[keymap->thrust];

		keymap++;
		}
	}

