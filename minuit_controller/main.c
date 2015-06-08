#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "metabot.h"
#include "minuit.h"


int main(int argc, char **argv)
{
	/* Initialiser SDL_net */
	UDPpacket *p;
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	SDLNet_SocketSet set;

	set=SDLNet_AllocSocketSet(16);
	if(!set) {
		printf("SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
		exit(1); //most of the time this is a major error, but do what you want.
	}


	/* Ouvrir un socket */
	UDPsocket sd;
	if (!(sd = SDLNet_UDP_Open(9998)))
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	int numused;

	numused=SDLNet_UDP_AddSocket(set,sd);
	if(numused==-1) {
		printf("SDLNet_AddSocket: %s\n", SDLNet_GetError());
		// perhaps you need to restart the set and make it bigger...
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

	int numready, numpkts;
	/* Boucle principale */
	int quit = 0;
	while (!quit)
	{

		numready = SDLNet_CheckSockets(set, -1); //wait for a packet to arrive
		if (numready == -1) {
			printf("SDLNet_CheckSockets: %s\n", SDLNet_GetError());
			// most of the time this is a system error, where perror might help you.
			perror("SDLNet_CheckSockets");
		}
		else if (numready) {
			// check all sockets with SDLNet_SocketReady and handle the active ones.
			if (SDLNet_SocketReady(sd)) {
				numpkts = SDLNet_UDP_Recv(sd, p);
				if (numpkts) {
					Str_array str = minuit_to_str_array(p);
					print_str_array(str);
					switch(protocol(str)){
					case OSC:
						execute(str_array_to_cmd(str), m);
						break;

					case minuit_query:
						;//empty statement
						Str_array answer = namespace_answer(get_device(m), get_string(str, 4));
						print_str_array(answer);
						send_answer(answer, 13579);
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
		}
	}

	/* Fermer la connection avec le Metabot */
	if(!DEBUG_MODE){
		stop(m);
	}

	/* Libérer la mémoire et quitter */
	free_metabot(m);
	SDLNet_FreePacket(p);
	// free a socket set
	//SDLNet_SocketSet set;
	SDLNet_FreeSocketSet(set);
	set=NULL; //this helps us remember that this set is not allocated

	SDLNet_Quit();
	return EXIT_SUCCESS;
}
