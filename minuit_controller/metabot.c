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

static char t_moves[][10] = {"h", "dx", "dy", "turn"};
static char t_sys[][10] = {"start", "stop"};
static char t_modes[][10] = {"alt", "crab", "backleg", "back", "freq", "gait"};

struct node{
	char * name;
	char * description;
	char * * attributes;
	int size;
};

struct metabot{
	Node * nodes;
	int fd;
};

/**
 * \brief Initialise une node
 * \param name Nom de la node
 * \param description Description de la node
 * \param attributes Tableau de chaines de caractères contenant les attributs de la node
 * \param size Taille du tableau attributes
 * \return Node initalisée
 */
Node new_node(char * name, char * description, char attributes[][10], int size){
	Node n = malloc(sizeof(struct node));
	n->name = malloc(strlen(name)+1);
	strcpy(n->name, name);
	n->description = malloc(strlen(description)+1);
	strcpy(n->description, description);
	n->size=size;
	n->attributes = malloc(size*sizeof(attributes));

	for(int i = 0 ; i < size ; i++){
		n->attributes[i] = malloc(strlen(attributes[i])+1);
		strcpy(n->attributes[i], attributes[i]);
	}
	return n;

}

Metabot new_metabot(char * path){
	Metabot m = malloc(sizeof(struct metabot));
	m->nodes = malloc(NB_NODES*sizeof(Node));
	m->nodes[0]=new_node("moves", "Fonctions de mouvement", t_moves, 4);
	m->nodes[1]=new_node("sys", "Fonctions système", t_sys, 2);
	m->nodes[2]=new_node("modes", "Fonctions modes", t_modes, 6);
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

/**
 * \brief Libère une node
 * \param n Node
 */
void free_node(Node n){
	free(n->name);
	free(n->description);

	for(int i = 0 ; i < sizeof(n->attributes) ; i++){
		free(n->attributes[i]);
	}
	free(n->attributes);
}

void free_metabot(Metabot m){
	for(int i = 0; i < NB_NODES ; i++)
		free_node(m->nodes[i]);
	free(m);
}

void display_node(Node n){
	printf("%s : %s\n{ ", n->name, n->description);
	for(int j = 0 ; j < n->size ; j++){
		printf("%s " , n->attributes[j]);
	}
	printf("}\n");
}

void display_metabot(Metabot m){
	printf("Metabot :\n");
	for(int i = 0 ; i < NB_NODES ; i++){
		display_node(m->nodes[i]);
	}
}

char * node_name(Node n){
	return n->name;
}

char * node_desc(Node n){
	return n->description;
}

char ** node_attr(Node n){
	return n->attributes;
}

int node_size(Node n){
	return n->size;
}

void execute(char * cmd, Metabot m){
	if(!DEBUG_MODE){
		if(write(m->fd, cmd, strlen(cmd)) == -1)
			printf("Couldn't write \"%s\"", cmd);
	}
	else
		printf("%s", cmd);
}

char ** namespace_cmd_array(Metabot m){
	char array2[][20] = {"Metabot:namespace", ",s", "/", "nodes={", "moves", "sys", "modes", "}"};
	char ** array = malloc(sizeof(array2));
	for(int i = 0; i < 8 ; i++){
		array[i] = malloc(strlen(array2[i]));
		array[i] = strcpy(array[i], array2[i]);
	}
	return array;
}

char ** namespace_node_cmd_array(Metabot m, char * node){
	int i = 0;
	while(i<NB_NODES && !strcmp(m->nodes[i]->name, node))
		i++;
	char ** array = malloc(sizeof(array)); //Ici, il faut créer le message à envoyer.
	return array;
}
