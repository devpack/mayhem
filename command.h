#ifndef __COMMAND_H_
#define __COMMAND_H_

struct vaisseau_data;

struct command {
 bool left;
 bool right;
 bool fire;
 bool shield;
 bool thrust;
 struct vaisseau_data* controlled_ship;
};

void handle_command(struct command *cmd);

#endif