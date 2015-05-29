#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "com.h"
#include "metabot.h"


int main(int argc, char **argv)
{
	/* Initialiser SDL_net */
	UDPpacket *p;
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	/* Ouvrir un socket */
	UDPsocket sd;
	if (!(sd = SDLNet_UDP_Open(9998)))
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	/* Allouer de la place pour le paquet */
	if (!(p = SDLNet_AllocPacket(512)))
	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	/* Initialiser le metabot et s'y connecter */
	Metabot m = new_metabot("/dev/rfcomm0");
	if(!DEBUG_MODE)
		start(m);
	/* Permet de voir les nodes et les attributs du metabot */
	if(DEBUG_MODE)
		display_metabot(m);

	/* Boucle principale */
	int quit = 0;
	while (!quit)
	{
		/* Attendre un paquet. UDP_Recv retourne != 0 si un paquet arrive */
		if (SDLNet_UDP_Recv(sd, p))
		{
			switch(get_protocol(p)){
			case OSC:
				execute(OSC_to_Metabot(p) , m);
				break;

			case minuit_namespace:
				printf("minuit_namespace\n");
				char ** cmd = namespace_cmd_array(m);
				send_answer(cmd,9998);
				break;

			case minuit_node:
				printf("minuit_node\n");
				send_answer(namespace_node_cmd_array(m, get_node_namespace(p)),9998);
				break;

			case minuit_reply:
				printf("Reply\n");
				break;

			case unknown:
				printf("Unknown protocol\n");
				break;
			}
		}
	}

	/* Fermer la connection avec le Metabot */
	if(!DEBUG_MODE){
		stop(m);
	}

	/* Libérer la mémoire et quitter */
	free_metabot(m);
	SDLNet_FreePacket(p);
	SDLNet_Quit();
	return EXIT_SUCCESS;
}
