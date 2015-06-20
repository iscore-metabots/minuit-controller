/*
 * musicPlayer.c*/
#include "minuit.h"
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>



int main(int argc, char **argv){
	int continuer = 1;

	SDL_Init(SDL_INIT_VIDEO);
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
	{
		printf("%s", Mix_GetError());
	}
	Mix_Music *musique; //Création du pointeur de type Mix_Music
	musique = Mix_LoadMUS("musique.wav"); //Chargement de la musique
	int quit = 0;
	SOCKET s = init_socket(8888);
	while (!quit)
	{
		DATA p = receive_socket(s);
		Str_array str = minuit_to_str_array(p);
		print_str_array(str);
		switch(protocol(str)){
		case OSC:
			printf("OSC\n");
			if(get_string(str, 0)[1] == 'p'){
				Mix_PlayMusic(musique, 1);
			}else if(get_string(str, 0)[1] == 's'){
				Mix_HaltMusic();
			}
			break;

		case minuit_query:
			printf("Minuit\n");
			break;

		case minuit_reply:
			printf("Reply\n");
			break;

		case unknown_protocol:
			printf("Unknown protocol\n");
			break;
		}
		free_str_array(str);
		free_data(p);
	}
	Mix_FreeMusic(musique); //Libération de la musique

	Mix_CloseAudio(); //Fermeture de l'API

	SDL_Quit();
	close_socket(s);
	return EXIT_SUCCESS;
}
