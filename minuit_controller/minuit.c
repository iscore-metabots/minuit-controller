#include <string.h>
#include "UDPserver.h"
#include <stdlib.h>
#include <stdio.h>
#include "minuit.h"


struct str_array{
	int size;
	char ** t;
	int max_size;
};

struct node{
	char * name;
	char * description;
	char * * attributes;
	int size;
};

struct device{
	char * name;
	Node * nodes;
	int nodes_len;
};

Node new_node(char * name, char * description, char * attributes[], int size){
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

Device new_device(char * name, Node n){
	Device d = malloc(sizeof(struct device));
	d->name = malloc(strlen(name)+1);
	d->name = strcpy(d->name, name);
	d->nodes_len = 1;
	d->nodes = malloc(sizeof(Node));
	d->nodes[0] = new_node(n->name, n->description, n->attributes, n->size);
	return d;
}

Device add_node(Device d, Node n){
	d->nodes = realloc(d->nodes, (d->nodes_len+1)*sizeof(Node));
	d->nodes[d->nodes_len] = new_node(n->name, n->description, n->attributes, n->size);
	d->nodes_len ++;
	return d;
}

Node get_node(Device d, int i){
	return d->nodes[i];
}

void free_device(Device d){
	free(d->name);
	for(int i = 0 ; i < d->nodes_len ; i++)
		free_node(d->nodes[i]);
	free(d);
}

void free_node(Node n){
	free(n->name);
	free(n->description);

	for(int i = 0 ; i < sizeof(n->attributes) ; i++){
		free(n->attributes[i]);
	}
	free(n->attributes);
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

void display_node(Node n){
	printf("%s : %s\n{ ", node_name(n), node_desc(n));
	for(int j = 0 ; j < node_size(n) ; j++){
		printf("%s " , node_attr(n)[j]);
	}
	printf("}\n");
}



int size_bytes(int i)
{
	if(i%4 == 0)
		return i;
	else
		return  i + 4 - i%4;
}

int write_data(DATA p, char * s, int it)
{
    for(int j = 0 ; j < size_bytes(strlen(s)+1) ; j++)
    {
        if(j < strlen(s))
            ds_string(p)[it] = s[j];
        else
            ds_string(p)[it] = '\0';
        it++;
    }
    return it;
}


Str_array new_string_array(char * s){
	Str_array str = malloc(sizeof(struct str_array));
	str->t = malloc(ARRAY_SIZE * sizeof(char *));
	str->t[0] = malloc(strlen(s) +1);
	str->t[0] = strcpy(str->t[0], s);
	str->size = 1;
	str->max_size = ARRAY_SIZE;
	return str;
}

Str_array str_array_append(Str_array str, char * s){
	if(str->size >= str->max_size){
		str->t = realloc(str, ARRAY_SIZE * 2 * sizeof(char *));
		str->max_size *=2;
	}
	str->t[str->size] = malloc(strlen(s)+1);
	str->t[str->size] = strcpy(str->t[str->size], s);
	str->size += 1 ;
	return str;
}

void free_str_array(Str_array str){
	for(int i = 0 ; i < str->size ; i++){
		free(str->t[i]);
	}
	free(str);
}

char * get_string(Str_array str, int i){
	if(i < str->size)
		return str->t[i];
	else{
		printf("Erreur de segmentation\n");
		return "";
	}
}

char * string_to_float_ASCII(char * str){
	char * s = malloc(sizeof(char)*5);
	int d = 0;
	for(int j = 0; j < 4 ; j++)
	{
		d <<= 8;
		d += str[j] & 0xFF;
	}

	float * f = (float * )&d;
	sprintf( s, "%f", *f);
	return s;
}


char * string_to_int_ASCII(char * str){
	int d = 0;
	char * s = malloc(sizeof(char)*5);
	for(int i = 0 ; i < 4 ; i++){
		d <<= 8;
		d += str[i] & 0xFF;
	}
	sprintf( s, "%d", d);
	return s;
}

char * data_cpy(DATA p, int debut, int fin){
	char * tmp = malloc((fin - debut)*sizeof(char) + 1);
	int i = 0;
	while(i < ds_len(p) - debut && i < fin - debut){
		tmp[i] = ds_string(p)[debut+i];
		i++;
	}
	tmp[fin] = '\0';
	return tmp;
}

Str_array OSC_to_str_array(DATA p){
	int i = 0;
	while(i < ds_len(p) && ds_string(p)[i] != ',')
		i++;
	char * tmp = data_cpy(p, 0, i);
	Str_array str = new_string_array(tmp);
	str = str_array_append(str, data_cpy(p, i, i+2));
	if(str->t[1][1] == 'i'){
		str = str_array_append(str, string_to_int_ASCII(data_cpy(p, i+4, ds_len(p))));
	}
	if(str->t[1][1] == 'f')
		str = str_array_append(str, string_to_float_ASCII(data_cpy(p, i+4, ds_len(p))));
	return str;
}

Str_array minuit_to_str_array(DATA p){
	if(ds_string(p)[0] == '/')
		return OSC_to_str_array(p);
	int i = 0;
	while(i < ds_len(p) && ds_string(p)[i] != '?' && ds_string(p)[i] != ':')
		i++;
	Str_array str = new_string_array(data_cpy(p, 0, i));
	str = str_array_append(str, data_cpy(p, i, i+1));
	i++;
	while(i <ds_len(p)){
		int j = 0;
		while(j < ds_len(p) - i && ds_string(p)[j+i] != '\0')
			j++;
		str = str_array_append(str, data_cpy(p, i, j + i));
		i += size_bytes(j);
	}
	return str;
}

void print_str_array(Str_array str){
	for(int i = 0 ; i < str->size ; i++){
		printf("%s  ", str->t[i]);
	}
	printf("\n");
}

void write_minuit_packet(Str_array str, DATA p){
	char * head = malloc(strlen(str->t[0]) + strlen(str->t[1]) + strlen(str->t[2]) + 3);
	sprintf(head, "%s%s%s", str->t[0], str->t[1], str->t[2]);
	int it = 0;
	ds_len_set(p, 0);
	ds_len_set(p, ds_len(p) + size_bytes(strlen(head)) );
	it = write_data(p, head, it);
	for(int i_str = 3 ; i_str < str->size ; i_str++)
	{
		ds_len_set(p, ds_len(p) + size_bytes(strlen(str->t[i_str])+1));
		it = write_data(p, str->t[i_str], it);
	}
	printf("p->data = %s\n", ds_string(p));
}

Protocol protocol(Str_array str){
	if(str->t[0][0] == '/')
		return OSC;
	else if(str->t[1][0] == ':')
		return minuit_reply;
	else if(str->t[1][0] == '?')
		return minuit_query;
	else
		return unknown;
}

//Réponse à une minuit_query
Str_array namespace_answer(Device d, char * path){
	Str_array str = new_string_array(d->name);
	str = str_array_append(str, ":");
	str = str_array_append(str, "namespace");
	if(!strcmp(path, "/")){
		//fabriquer namespace général
		char * type = malloc(sizeof(char)*(d->nodes_len + 5));
		type[0] = ',';
		for(int i = 1 ; i < d->nodes_len+5 ; i++)
			type[i] = 's';
		type[d->nodes_len + 5] = '\0';
		str = str_array_append(str, type);
		free(type);
		str = str_array_append(str, "/");
		str = str_array_append(str, "Application");
		str = str_array_append(str, "nodes={");
		for(int i = 0 ; i < d->nodes_len ; i++)
			str = str_array_append(str, d->nodes[i]->name);
		str = str_array_append(str, "}");
	}
	else{
		//fabriquer namespace pour la node située à l'adresse path
		Node n = NULL;
		for(int i = 0 ; i < d->nodes_len ; i++){
			if(strcmp(d->nodes[i]->name, path+1))
				n = d->nodes[i];
		}
		if(n == NULL){
			printf("Couldn't find node\n");
			return str;
		}
		char * type = malloc(sizeof(char)*(n->size + 5));
		type[0] = ',';
		for(int i = 1 ; i < n->size+5 ; i++)
			type[i] = 's';
		type[n->size+5] = '\0';
		str = str_array_append(str, type);
		free(type);
		str = str_array_append(str, path);
		str = str_array_append(str, "none");
		str = str_array_append(str, "attributes={");
		for(int i = 0 ; i < n->size ; i++)
			str = str_array_append(str, n->attributes[i]);
		str = str_array_append(str, "}");
	}
	return str;
}

