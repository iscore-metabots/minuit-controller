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
  Node * subnodes;
  Node parent;
  int size;
  ObjectType objectType;
};

struct device{
  Node root;
};

Node new_node(char * name, char * description){
  Node n = malloc(sizeof(struct node));
  n->name = malloc(strlen(name)+1);
  strcpy(n->name, name);
  n->description = malloc(strlen(description)+1);
  strcpy(n->description, description);
  n->subnodes = NULL;
  n->size = 0;
  n->parent = NULL;
  return n;
}

Device new_device(char * name, Node root){
  Device d = malloc(sizeof(struct device));
  d->root = root;
  return d;
}

Node get_root(Device d){
  return d->root;
}

void add_subnode(Node n, Node subn){
  if(n->subnodes == NULL)
    n->subnodes = malloc(sizeof(struct node));
  else
    n->subnodes = realloc(n->subnodes, (n->size + 1) * sizeof(struct node));
  n->subnodes[n->size] = subn;
  n->size++;
  subn->parent = n;
}

void free_device(Device d){
  free_node(d->root);
  free(d);
}

void delete_subnode(Node n, Node subnode){
  int flag = 0;
  for(int i = 0 ; i < n->size ; i++){
    if(flag && i != n->size-1)
      n->subnodes[i] = n->subnodes[i+1];
    if(n->subnodes[i] == subnode){
      flag = 1;
    }
  }
  if(flag == 1){
    n->subnodes[n->size - 1] = NULL;
    n->size --;
  }
  else
    printf("Can't find node");
}

void free_node(Node n){
  free(n->name);
  free(n->description);
  if(n->subnodes != NULL)
    for(int i = 0; i < n->size ; i++)
      free_node(n->subnodes[i]);
  if(n->parent != NULL)
    delete_subnode(n->parent, n);
  free(n);
}

char * node_name(Node n){
    return n->name;
}

char * node_desc(Node n){
  return n->description;
}

int node_size(Node n){
  return n->size;
}

Node find_node(Node n, char * name){
  if(strcmp(n->name, name)==0)
    return n;
  if(n->subnodes != NULL){
    for(int i = 0 ; i < n->size ; i++){
      if(strcmp(n->subnodes[i]->name, name) == 0)
	return n->subnodes[i];
      else
	{
	  Node l = find_node(n->subnodes[i], name);
	  if(l != NULL)
	    return l;
	}
    }
  }
  return NULL;
}

void display_node(Node n){
  if(n != NULL){
    printf("%s : %s\n{ ", node_name(n), node_desc(n));
    if(n->subnodes != NULL){
      for(int j = 0 ; j < node_size(n) ; j++){
	display_node(n->subnodes[j]);
      }
    }
    printf("}\n");
  }
  else
    printf("Node n == NULL");
}



int size_bytes(int i)
{
  if(i%4 == 0)
    return i;
  else
    return  i + 4 - i%4;
}


Str_array new_string_array(){
  Str_array str = malloc(sizeof(struct str_array));
  str->t = malloc(ARRAY_SIZE * sizeof(char *));
  str->size = 0;
  str->max_size = ARRAY_SIZE;
  return str;
}

Str_array str_array_append(Str_array str, char * s){
  Str_array tmp = str;
  if(str->size >= str->max_size){
    tmp = malloc(sizeof(struct str_array));
    tmp->t = malloc(str->max_size * 2 * sizeof(char*));
    tmp->max_size = str->max_size*2;
    tmp->size = str->size;
    for(int i = 0 ; i < str->size ; i++)
      tmp->t[i] = str->t[i];
    free(str);
  }
  tmp->t[tmp->size] = malloc(strlen(s)+1);
  tmp->t[tmp->size] = strcpy(tmp->t[tmp->size], s);
  tmp->t[tmp->size][strlen(s)] = '\0';
  tmp->size += 1 ;
  return tmp;
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
  tmp[fin-debut] = '\0';
  return tmp;
}

Str_array OSC_to_str_array(DATA p){
  int i = 0;
  while(i < ds_len(p) && ds_string(p)[i] != ',')
    i++;
  Str_array str = new_string_array();
  str = str_array_append(str, data_cpy(p, 0, i));
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
  Str_array str = new_string_array();
  str = str_array_append(str, data_cpy(p, 0, i));
  str = str_array_append(str, data_cpy(p, i, i+1));
  i++;
  int j;
  while(i <ds_len(p)){
    j = 0;
    while(j < ds_len(p) - i && ds_string(p)[j+i] != '\0')
      j++;
    str = str_array_append(str, data_cpy(p, i, j + i));
    i += size_bytes(j+1);
  }
  return str;
}

void print_str_array(Str_array str){
  for(int i = 0 ; i < str->size ; i++){
    printf("%s  ", str->t[i]);
  }
  printf("\n");
}

DATA write_minuit_packet(Str_array str){
  int len = 0;
  for(int i = 0; i < 3 ; i++){
      len += strlen(str->t[i]);
  }
  len = size_bytes(len + 3);
  for(int i = 3 ; i < str->size ; i++){
    len += size_bytes(strlen(str->t[i]) + 1);
  }
  char * buffer = malloc(len * sizeof(char));
  int it = 0;
  for(int i = 0 ; i < 3 ; i++){
    for(int j = 0 ; j < strlen(str->t[i]) ; j++){
      buffer[it] = str->t[i][j];
      it++;
    }
  }
  while(it < size_bytes(strlen(str->t[0]) + strlen(str->t[1]) + strlen(str->t[2]) + 3)){
    buffer[it] = '\0';
    it++;
  }
  for(int i = 3 ; i < str->size ; i++){
    for(int j = 0 ; j < strlen(str->t[i]) ; j++){
      buffer[it] = str->t[i][j];
      it++;
    }
    for(int j = strlen(str->t[i]) ; j < size_bytes(strlen(str->t[i]) +1 ) ; j++){
      buffer[it] = '\0';
      it++;
    }
  }
  return init_data(buffer, len);
}

Attribute get_attribute(char * s){
   int i = 0;
  while(i < strlen(s) && s[i] != ':'){
    i++;
  }
  if(i >= strlen(s))
    return unknown_attribute;
  else
    {
      char * attr = s + i + 1;
      if(strcmp(attr, "value") == 0)
	return value;
      if(strcmp(attr, "tags") == 0)
	return tags;
      if(strcmp(attr, "type") == 0)
	return type;
      if(strcmp(attr, "service") == 0)
	return service;
      if(strcmp(attr, "priority") == 0)
	return priority;
      if(strcmp(attr, "rangeBounds") == 0)
	return rangeBounds;
      if(strcmp(attr, "rangeClipmode") == 0)
	return rangeClipmode;
      if(strcmp(attr, "description") == 0)
	return description;
      else
	return unknown_attribute;
    }
}

Query get_query(Str_array str){
  if(strcmp(str->t[2], "namespace") == 0)
    return discover;
  if(strcmp(str->t[2], "get" )== 0)
    return get;
  if(strcmp(str->t[2], "set") == 0)
    return set;
  if(strcmp(str->t[2], "listen") == 0)
    return listen;
  else
    return unknown_query;
}

Protocol protocol(Str_array str){
  if(str->t[0][0] == '/')
    return OSC;
  if(str->t[1][0] == ':')
    return minuit_reply;
  if(str->t[1][0] == '?')
    return minuit_query;
  else
    return unknown_protocol;
}

//Réponse à une minuit_query
Str_array namespace_answer(Device d, Str_array src){
  char * path = src->t[4];
  Str_array str = new_string_array();
  str = str_array_append(str, d->root->name);
  str = str_array_append(str, ":");
  str = str_array_append(str, "namespace");
  if(!strcmp(path, "/")){
    //fabriquer namespace général
    char * type = malloc(sizeof(char)*(d->root->size + 6));
    type[0] = ',';
    for(int i = 1 ; i < d->root->size+5 ; i++)
      type[i] = 's';
    type[d->root->size + 5] = '\0';
    str = str_array_append(str, type);
    free(type);
    str = str_array_append(str, "/");
    str = str_array_append(str, "Application");
    str = str_array_append(str, "nodes={");
    for(int i = 0 ; i < d->root->size ; i++)
      str = str_array_append(str, d->root->subnodes[i]->name);
    str = str_array_append(str, "}");
  }
  else{
    //fabriquer namespace pour la node située à l'adresse path
    int cpt = 0;
    for(int i = 0 ; i < strlen(path) ; i++){
      if(path[i] == '/')
	cpt++;
    }
    if(cpt == 1){
      Node n = NULL;
      for(int i = 0 ; i < d->root->size ; i++){
	if(strcmp(d->root->subnodes[i]->name, path+1) == 0)
	  n = d->root->subnodes[i];
      }
      if(n == NULL){
	printf("Couldn't find node\n");
	return str;
      }
      char * type = malloc(sizeof(char)*(n->size + 6));
      type[0] = ',';
      for(int i = 1 ; i < n->size+5 ; i++)
	type[i] = 's';
      type[n->size+5] = '\0';
      str = str_array_append(str, type);
      free(type);
      str = str_array_append(str, path);

      str = str_array_append(str, "Container");
      str = str_array_append(str, "nodes={");
      for(int i = 0 ; i < n->size ; i++)
	str = str_array_append(str, n->subnodes[i]->name);
      str = str_array_append(str, "}");
    }
    else if (cpt == 2){
      char * type = malloc(sizeof(char)*(4));
    type[0] = ',';
    for(int i = 1 ; i < 3 ; i++)
      type[i] = 's';
    type[4] = '\0';
    str = str_array_append(str, type);
    free(type);
    str = str_array_append(str, path);
      str = str_array_append(str, "Data");
    }
  }
  return str;
}

char * path_to_name(char * path){
  int last_slash = 0;
  int i;
  for(i = 0 ; path[i] != '\0' && path[i] != ':' ; i++){
    if(path[i] == '/')
      last_slash = i;
  }
  char * name = malloc((i - last_slash + 1 ) * sizeof(char));
  name = strncpy(name, last_slash + 1 + path, i - last_slash - 1);
  name[i - last_slash -1] = '\0';
  return name;
}

/*
Str_array = get_answer(Device d, Str_array str){
  
  return answer;
  }*/

Str_array minuit_answer(Device d, Str_array str){
  Str_array answer = NULL;
  switch(get_query(str)){
  case discover:
    answer = namespace_answer(d, str);
    break;
  case get:
    ;
    char * name = path_to_name(str->t[4]);
    Node n = find_node(d->root, name);
    if( n == NULL )
      printf("Node %s doesn't exist\n", name);
    else{
      switch(get_attribute(str->t[4])){
      case value:
	printf("value");
	break;
      case type:
	printf("type");
	break;
      case tags:
	printf("tags");
	break;
      case rangeClipmode:
	printf("rangeClipmode");
	break;
      case service:
	printf("service");
	break;
      case priority:
	printf("priority");
	break;
      case rangeBounds:
	printf("rangeBounds");
	break;
      case description:
	printf("description");
	break;
      case unknown_attribute:
	printf("unknown_attribute");
	break;
      }
    }
    break;
  case set:
    break;
  case listen:
    break;
  case unknown_query:
    break;
  }
  return answer;
}
