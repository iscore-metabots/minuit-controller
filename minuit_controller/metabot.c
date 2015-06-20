/*
 * metabot.c
 *
 *  Created on: 29 mai 2015
 *      Author: nicho
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "metabot.h"

static char * t_moves[] = {"h", "dx", "dy", "turn"};
static char  * t_sys[] = {"start", "stop"};
static char * t_modes[] = {"alt", "crab", "backleg", "back", "freq", "gait"};

struct metabot{
  Device d;
  int fd;
};

Metabot new_metabot(char * path){
  Metabot m = malloc(sizeof(struct metabot));
  Node n = new_node("Metabot", "root");
  m->d = new_device("Metabot", n);
  Node moves = new_node("moves", "Fonctions de mouvements");
  Node sys = new_node("sys", "Fonctions systeme");
  Node modes = new_node("modes", "Fonctions de mode");
  add_subnode(n, moves);
  add_subnode(n, sys);
  add_subnode(n, modes);
  for(int i = 0 ; i < 4 ; i++)
    add_subnode(moves, new_node(t_moves[i], "subnode"));
  for(int i = 0 ; i < 2 ; i++)
    add_subnode(sys, new_node(t_sys[i], "subnode"));
  for(int i = 0 ; i < 6 ; i++)
    add_subnode(modes, new_node(t_modes[i], "subnode"));
	
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
