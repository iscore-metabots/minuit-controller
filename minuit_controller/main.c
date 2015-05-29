#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include "controller.h"
#include "metabot.h"
#include "parse.h"


int main(int argc, char **argv)
{
	UDPsocket sd;       /* Socket descriptor */
	UDPpacket *p;       /* Pointer to packet memory */

	//Initialisation des nodes et de leurs attributs
	//Plus tard, ces données seront modifiables via un fichier ou un menu
	char t_moves[][10] = {"h", "dx", "dy", "turn"};
	char t_sys[][10] = {"start", "stop"};
	char t_modes[][10] = {"alt", "crab", "backleg", "back", "freq", "gait"};

	Metabot m = new_metabot(t_moves, t_sys, t_modes);

	if(DEBUG_MODE)
		display_metabot(m);

	int fd = 0;
	if(!DEBUG_MODE){
		fd = open("/dev/rfcomm0", O_RDWR);
		if(write(fd, "start\n", strlen("start\n"))==-1)
			printf("Couldn't write \"start\"\n");
	}

	/* Initialize SDL_net */
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	/* Open a socket */
	if (!(sd = SDLNet_UDP_Open(9998)))
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	/* Make space for the packet */
	if (!(p = SDLNet_AllocPacket(512)))
	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	/* Main loop */
	int quit = 0;
	while (!quit)
	{
		/* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
		if (SDLNet_UDP_Recv(sd, p))
		{
			switch(get_protocol(p)){
			case OSC:
				printf("OSC\n");
				receive_OSC_command(p, fd);
				break;
			case minuit_namespace:
				printf("minuit_namespace\n");
				answer_namespace(9998);
				break;
			case minuit_node:
				printf("minuit_node\n");
				int i = 0;
				while(i < p->len && p->data[i] != '/')
					i++;
				//copie de la commande
				char * name = malloc(32*sizeof(char));
				name = strcpy(name, (char *)p->data+i);
				Node n;
				for(int x = 0 ; x < NB_NODES ; x++)
				{
					n=m[x];
					if(strcmp(name, node_name(n)))
					{
						answer_namespace_node(x,9998);
					}
				}
				free(name);
				break;

			case unknown:
				printf("Unknown protocol\n");
				break;

			case minuit_reply:
				printf("Reply\n");
				break;
			}
		}
	}
	/* Clean and exit */
	free_metabot(m);
	SDLNet_FreePacket(p);
	SDLNet_Quit();
	if(!DEBUG_MODE){
		if(write(fd, "stop\n", strlen("stop\n")) == -1)
			printf("Couldn't write \"stop\"\n");
		close(fd);
	}


	return EXIT_SUCCESS;
}
