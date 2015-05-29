#ifndef METABOT_H
#define METABOT_H

#include <SDL_net.h>


extern int size_bytes(char * s);
extern int write_data(UDPpacket *p, char * s, int it);
extern void send_answer(char ** cmd, int port);

#endif // METABOT_H_INCLUDED
