#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>

#include "metabot.h"
#include "net.h"
#include "parse.h"


int main(int argc, char **argv)
{
	/* Initialize SDL_net */
	UDPpacket *p;
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	/* Open a socket */
	UDPsocket sd;
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


	Metabot m = new_metabot();

	if(DEBUG_MODE)
		display_metabot(m);

	int fd = 0;
	if(!DEBUG_MODE){
		fd = open("/dev/rfcomm0", O_RDWR);
		if(write(fd, "start\n", strlen("start\n"))==-1)
			printf("Couldn't write \"start\"\n");
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
