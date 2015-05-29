/*
 * metabot.h
 *
 *  Created on: 29 mai 2015
 *      Author: nicho
 */

#ifndef METABOT_H_
#define METABOT_H_

#define NB_NODES 3

typedef struct node * Node;
typedef Node * Metabot;

extern Metabot new_metabot(char t_moves [][10], char t_sys[][10], char t_modes[][10]);
extern void display_metabot(Metabot m);
extern void free_metabot();

char * node_name(Node n);
char * node_desc(Node n);
char ** node_attr(Node n);
int node_size(Node n);

#endif /* METABOT_H_ */
