#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "metabot.h"

struct metabot{
  Device d;
  int fd;
};

Metabot new_metabot(char * path, char * configfile){
  Metabot m = malloc(sizeof(struct metabot));
  m->d = file_device(configfile);
  if(!DEBUG_MODE)
    m->fd = open(path, O_RDWR);
  else
    m->fd = 0;
  return m;
}

void start(Metabot m){
  if(write(m->fd, "start\n", strlen("start\n"))==-1)
    printf("Couldn't write \"start\"\n");
}

void stop(Metabot m){
  if(write(m->fd, "stop\n", strlen("stop\n"))==-1)
    printf("Couldn't write \"start\"\n");
}

void free_metabot(Metabot m){
  free_device(m->d);
  close(m->fd);
  free(m);
}

void display_metabot(Metabot m){
  printf("Metabot :\n");
  display_node(get_root(m->d));
}

char * str_array_to_cmd(Str_array str){
  char * cmd = malloc((strlen(get_string(str,0)) + 6)*sizeof(char));
  sprintf( cmd, "%s %s\n", path_to_name(get_string(str,0)), get_string(str,2));
  return cmd;
}

Device get_device(Metabot m){
  return m->d;
}

void execute(char * cmd, Metabot m){
  if(!DEBUG_MODE){
    if(write(m->fd, cmd, strlen(cmd)) == -1)
      printf("Couldn't write \"%s\"", cmd);
  }
  else
    printf("%s\n", cmd);
}

void metabot_controller(char * serialport, char * port, char * configfile){
  Metabot m = new_metabot(serialport, configfile);
  if(!DEBUG_MODE)
    start(m);
  // Permet de voir les nodes et les attributs du metabot
  
  if(DEBUG_MODE)
    display_metabot(m);
  
  //Boucle principale
  int quit = 0;
  SOCKET s = init_socket(atoi(port));
  while (!quit)
    {
      DATA p = receive_socket(s);
      Str_array str = minuit_to_str_array(p);
      switch(protocol(str)){
      case OSC:
	;
	char * name = path_to_name(get_string(str,0));
	if(strcmp(name, "quit")==0)
	  quit = 1;
	else
	  execute(str_array_to_cmd(str), m);
	free(name);
	break;
      
      case minuit_query:
      ;
	Str_array answer = minuit_answer(get_device(m), str);
	if(answer == NULL)
	  break;
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
  // Fermer la connection avec le Metabot
  if(!DEBUG_MODE){
    stop(m);
  }

  // Libérer la mémoire et quitter
  // free a socket set
  //SDLNet_SocketSet set;
  
  free_metabot(m);
}
