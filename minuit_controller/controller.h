#ifndef METABOT_H
#define METABOT_H

#define DEBUG_MODE 1

#include <SDL_net.h>


extern int size_bytes(char * s);
extern int write_data(UDPpacket *p, char * s, int it);
extern void receive_OSC_command(UDPpacket *p, int fd);
extern void send_answer(char ** cmd, int port);
extern void answer_namespace(int port);
extern void answer_namespace_node(int x, int port);



#endif // METABOT_H_INCLUDED
