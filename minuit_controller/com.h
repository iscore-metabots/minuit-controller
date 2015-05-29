#ifndef COM_H_
#define COM_H_

#include <SDL_net.h>

enum protocol { OSC, minuit_namespace, minuit_node, minuit_reply, unknown };
typedef enum protocol Protocol;

/**\brief Renvoie le protocole correspondant au message contenu dans p->data
 * \param UDPpacket *p le paquet à analyser
 * \return Protocole de p
 */
extern Protocol get_protocol(UDPpacket *p);

/**
 * \brief Convertit un message OSC en une ligne de commande executable par un Metabot
 * \param *p le paquet contenant le message
 * \return Chaine de caractère contenant une instruction
 */
extern char * OSC_to_Metabot(UDPpacket *p);

/**
 * \brief Renvoie le nom de la node indiquée dans le message
 * \param *p paquet contenant le message
 * \return nom de la node
 */
extern char * get_node_namespace(UDPpacket *p);

/**
 * \brief Envoie le message contenu dans "cmd" en UDP sur le port "port"
 * Chaque case de cmd sera écrite dans un bloc d'une taille multiple de 32 bits dans le message final.
 * \param cmd Tableau de chaines de caractères.
 * \param port Port
 */
extern void send_answer(char ** cmd, int port);


#endif /* COM_H_ */
