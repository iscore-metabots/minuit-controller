#ifndef METABOT_H
#define METABOT_H

#define NB_NODES 3
#define DEBUG_MODE 1

#include <SDL_net.h>
#include <stdbool.h>

typedef struct node * Node;
typedef Node * Metabot;

extern Metabot new_metabot(char ** t_moves, char ** t_sys, char ** t_modes);
extern Node new_node(char * name, char * description, char ** attributs );
extern void display_metabot(Metabot m);
extern void free_metabot();
extern int size_bytes(char * s);
extern int write_data(UDPpacket *p, char * s, int it);
extern void receive_OSC_command(UDPpacket *p, int fd);
extern void send_answer(char ** cmd, int port);
extern void answer_namespace(int port);
extern void answer_namespace_node(int x, int port);
bool cmp_name_node(char * name, Node n);

#endif // METABOT_H_INCLUDED
