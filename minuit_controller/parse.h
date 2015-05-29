/*
 * parse.h
 *
 *  Created on: 29 mai 2015
 *      Author: nicho
 */

#ifndef PARSE_H_
#define PARSE_H_

#include <SDL_net.h>

enum protocol { OSC, minuit_namespace, minuit_node, minuit_reply, unknown };
typedef enum protocol Protocol;

extern Protocol get_protocol(UDPpacket *p);

#endif /* PARSE_H_ */
