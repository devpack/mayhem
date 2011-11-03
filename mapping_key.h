#ifndef __MAPPING_KEY_H_
#define __MAPPING_KEY_H_

#include "vaisseau_data.h"

struct mapping_key {
	int left,right,thrust,shield,fire;
	struct command *cmd;
};

void init_mapping_key(struct mapping_key* keymap, struct command *cmd,
	                    int left,int right,int thrust, int shield, int fire);

#endif
