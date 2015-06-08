#include <string.h>

#include "minuit.h"


struct str_array{
	int size;
	char ** t;
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

int write_data(UDPpacket *p, char * s, int it)
{
    for(int j = 0 ; j < size_bytes(strlen(s)+1) ; j++)
    {
        if(j < strlen(s))
            p->data[it] = s[j];
        else
            p->data[it] = 0;
        it++;
    }
    return it;
}


Str_array new_string_array(char * s){
	Str_array str = malloc(sizeof(struct str_array));
	str->t = malloc(sizeof(char *));
	str->t[0] = malloc(strlen(s) +1);
	str->t[0] = strcpy(str->t[0], s);
	str->size = 1;
	return str;
}

Str_array str_array_append(Str_array str, char * s){
	str->t = realloc(str->t, (str->size +1)*sizeof(char *));
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
	return str->t[i];
}

char * data_to_string(Uint8 * data, int n){
	char * s = malloc(sizeof(char)*(n));
	for(int i = 0 ; i < n ; i++){
		s[i] = (char)data[i];
	}
	s[n] = '\0';
	return s;
}

char * data_to_string_int(Uint8 * data){
	char * s = malloc(sizeof(char)*5);
	long int d = 0;
	for(int j = 0; j < 4 ; j++)
	{
		d <<= 8;
		d += data[j];
	}
	sprintf( s, "%ld", d);
	return s;
}

char * data_to_string_float(Uint8 * data){
	char * s = malloc(sizeof(char)*5);
	long int * d = malloc(sizeof(long int));
	for(int j = 0; j < 4 ; j++)
	{
		*d <<= 8;
		*d += data[j];
	}
	float * f = (float * )d;
	sprintf( s, "%f", *f);
	free(d);
	return s;

}

//adresse mail du developpeur de minuit : theod@gmea.net
char * get_node_namespace(UDPpacket *p){
	int i = 0;
	while(i < p->len && p->data[i] != '/')
		i++;
	//copie de la commande
	char * name = malloc(32*sizeof(char));
	name = strcpy(name, (char *)p->data+i+1);
	return name;
}

Str_array OSC_to_str_array(UDPpacket *p){
	int i = 0;
	while(i < p->len && p->data[i] != ',')
		i++;
	char * tmp = data_to_string(p->data, i);
	Str_array str = new_string_array(tmp);
	str = str_array_append(str, data_to_string(p->data + i, 4));

	if(str->t[1][1] == 'i')
		str = str_array_append(str, data_to_string_int(p->data + i+4));
	if(str->t[1][1] == 'f')
		str = str_array_append(str, data_to_string_float(p->data + i+4));
	return str;
}

Str_array minuit_to_str_array(UDPpacket *p){
	if(p->data[0] == '/')
		return OSC_to_str_array(p);
	int i = 0;
	while(i < p->len && p->data[i] != '?' && p->data[i] != ':')
		i++;
	Str_array str = new_string_array(data_to_string(p->data, i));
	str = str_array_append(str, data_to_string(p->data+i, 1));
	i++;
	while(i <p->len){
		int j = 0;
		while(j < p->len - i && p->data[j+i] != '\0')
			j++;
		str = str_array_append(str, data_to_string(p->data+i, j));
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

void write_minuit_packet(Str_array str, UDPpacket *p){
	char * head = malloc(strlen(str->t[0]) + strlen(str->t[1]) + strlen(str->t[2]));
	sprintf(head, "%s%s%s", str->t[0], str->t[1], str->t[2]);
	int it = 0;
	p->len += size_bytes(strlen(head));
	it = write_data(p, head, it);
	for(int i_str = 3 ; i_str < str->size ; i_str++)
	{
		p->len += size_bytes(strlen(str->t[i_str])+1);
		it = write_data(p, str->t[i_str], it);
	}
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
		char * type = malloc(sizeof(char)*(n->size + 6));
		type[0] = ',';
		for(int i = 1 ; i < n->size+6 ; i++)
			type[i] = 's';
		type[n->size+6] = '\0';
		str = str_array_append(str, type);
		free(type);
		str = str_array_append(str, path);
		str = str_array_append(str, "attributes={");
		for(int i = 0 ; i < n->size ; i++)
			str = str_array_append(str, n->attributes[i]);
		str = str_array_append(str, "}");
	}
	return str;
}

void send_answer(Str_array str, int port)
{
    UDPsocket sd;
    IPaddress srvadd;
    UDPpacket *p;

    /* Open a socket on random port */
    if (!(sd = SDLNet_UDP_Open(0)))
    {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    /* Resolve server name  */
    if (SDLNet_ResolveHost(&srvadd, "localhost", port) == -1)
    {
        fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n", "localhost", port, SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    /* Allocate memory for the packet */
    if (!(p = SDLNet_AllocPacket(512)))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    write_minuit_packet(str, p);
    p->address.host = srvadd.host;	/* Set the destination host */
    p->address.port = srvadd.port;	/* And destination port */

    SDLNet_UDP_Send(sd, -1, p); /* This sets the p->channel */
    SDLNet_FreePacket(p);
    SDLNet_Quit();
}

