#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "metabot.h"
#include "minuit.h"


int main(int argc, char **argv)
{

  /* Initialiser le metabot et s'y connecter */
  Metabot m = new_metabot(argv[1]);
  if(!DEBUG_MODE)
    start(m);
  /* Permet de voir les nodes et les attributs du metabot */
  if(DEBUG_MODE)
    display_metabot(m);
  /* Boucle principale */
  int quit = 0;
  SOCKET s = init_socket(atoi(argv[2]));
  while (!quit)
    {
      DATA p = receive_socket(s);
      Str_array str = minuit_to_str_array(p);
      printf("\nReception : ");
      print_str_array(str);
      switch(protocol(str)){
      case OSC:
	execute(str_array_to_cmd(str), m);
	break;

      case minuit_query:
	;
	Str_array answer = minuit_answer(get_device(m), str);
	if(answer == NULL)
	  break;
	printf("\nEnvoi : ");
	print_str_array(answer);
	DATA d = write_minuit_packet(answer);
	send_socket(s, d, "127.0.0.1", 13579);
	free_str_array(answer);
	free_data(d);
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
  close_socket(s);
  /* Fermer la connection avec le Metabot */
  if(!DEBUG_MODE){
    stop(m);
  }

  /* Libérer la mémoire et quitter */
  free_metabot(m);
  // free a socket set
  //SDLNet_SocketSet set;
  return EXIT_SUCCESS;
}
