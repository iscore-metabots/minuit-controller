/*
 * metabot.c
 *
 *  Created on: 29 mai 2015
 *      Author: nicho
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "metabot.h"

struct node{
	char * name;
	char * description;
	char * * attributes;
	int size;
};

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

Metabot new_metabot(char t_moves [][10], char t_sys[][10], char t_modes[][10]){
	Metabot m = malloc(NB_NODES*sizeof(Node));
	m[0]=new_node("moves", "Fonctions de mouvement", t_moves, 4);
	m[1]=new_node("sys", "Fonctions système", t_sys, 2);
	m[2]=new_node("modes", "Fonctions modes", t_modes, 6);
	return m;
}

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
		free_node(m[i]);
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
		display_node(m[i]);
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
