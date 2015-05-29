#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>

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

	/* Initialiser le metabot */
	Metabot m = new_metabot();

	/* Permet de voir les nodes et les attributs du metabot */
	if(DEBUG_MODE)
		display_metabot(m);

	/* Etablir la connection avec le metabot */
	int fd = 0;
	if(!DEBUG_MODE){
		fd = open("/dev/rfcomm0", O_RDWR);
		if(write(fd, "start\n", strlen("start\n"))==-1)
			printf("Couldn't write \"start\"\n");
	}
	/* Boucle principale */
	int quit = 0;
	while (!quit)
	{
		/* Attendre un paquet. UDP_Recv retourne != 0 si un paquet arrive */
		if (SDLNet_UDP_Recv(sd, p))
		{
			switch(get_protocol(p)){
			case OSC:
				printf("OSC\n");
				execute(OSC_to_Metabot(p) , fd);
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
		if(write(fd, "stop\n", strlen("stop\n")) == -1)
			printf("Couldn't write \"stop\"\n");
		close(fd);
	}

	/* Libérer la mémoire et quitter */
	free_metabot(m);
	SDLNet_FreePacket(p);
	SDLNet_Quit();
	return EXIT_SUCCESS;
}
