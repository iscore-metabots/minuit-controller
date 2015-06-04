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
