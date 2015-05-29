#ifndef COM_H_
#define COM_H_

#include <SDL_net.h>

enum protocol { OSC, minuit_namespace, minuit_node, minuit_reply, unknown };
typedef enum protocol Protocol;

extern Protocol get_protocol(UDPpacket *p);
extern char * OSC_to_Metabot(UDPpacket *p);
extern char * answer_namespace_node(UDPpacket *p);
extern char * get_node_namespace(UDPpacket *p);
extern void send_answer(char ** cmd, int port);


#endif /* COM_H_ */
