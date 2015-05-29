/*
 * metabot.h
 *
 *  Created on: 29 mai 2015
 *      Author: nicho
 */

#ifndef METABOT_H_
#define METABOT_H_

#define DEBUG_MODE 1

#define NB_NODES 3

typedef struct node * Node;
typedef Node * Metabot;

extern Metabot new_metabot();
extern void display_metabot(Metabot m);
extern void free_metabot();

char * node_name(Node n);
char * node_desc(Node n);
char ** node_attr(Node n);
int node_size(Node n);

char ** namespace_cmd_array(Metabot m);
char ** namespace_node_cmd_array(Metabot m, char * node);

void execute(char * cmd, int fd);

#endif /* METABOT_H_ */
