#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "device.h"
#include "str_array.h"

/********************************************************/
/******* Minuit device and node related fonctions *******/
/********************************************************/

struct node{
  char * name;
  Node * subnodes;
  Node parent;
  int size;
  char * objectType;
  char * type;
  char * description;
  int priority;
  char * service;
  int * rangeBounds;
};

struct device{
  char * name;
  Node root;
};

Node new_node(char * name, char * description, char * type, int * rangeBounds, char * objectType){
  Node n = malloc(sizeof(struct node));
  n->name = malloc(strlen(name)+1);
  strcpy(n->name, name);
  if(description != NULL){
    n->description = malloc(strlen(description)+1);
    strcpy(n->description, description);
  }
  else
    n->description = NULL;
  if(type != NULL){
    n->type = malloc(strlen(type)+1);
    strcpy(n->type, type);
  }
  else
    n->type = NULL;
  if(objectType != NULL){
    n->objectType = malloc(strlen(objectType)+1);
    strcpy(n->objectType, objectType);
  }
  else
    n->objectType = NULL;
  if(rangeBounds != NULL){
    n->rangeBounds = malloc(sizeof(int)*2);
    n->rangeBounds[0] = rangeBounds[0];
    n->rangeBounds[1] = rangeBounds[1];
  }
  else
    n->rangeBounds = NULL;
  n->subnodes = NULL;
  n->size = 0;
  n->parent = NULL;
  n->priority = 0;
  n->service = "message";
  return n;
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

Node * node_subnodes(Node n){
  return n->subnodes;
}

char * node_type(Node n){
  return n->type;
}

int node_priority(Node n){
  return n->priority;
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

void display_node(Node n){
  if(n != NULL){
    printf("%s : \ndesc: %s\n{ ", node_name(n), node_desc(n));
    if(n->type != NULL)
      printf("type : %s\n{ ", n->type);
    if(n->rangeBounds!=NULL)
      printf("rangeBounds : [ %d , %d ]\n", n->rangeBounds[0], n->rangeBounds[1]);
    if(n->objectType!=NULL)
      printf("objectType: %s\n", n->objectType);
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

char * node_name(Node n){
  return n->name;
}

char * node_desc(Node n){
  return n->description;
}

int node_size(Node n){
  return n->size;
}

char * node_service(Node n){
  return n->service;
}

char * node_objectType(Node n){
  return n->objectType;
}

void free_node(Node n){
  free(n->name);
  free(n->description);
  if(n->subnodes != NULL){
    for(int i = 0; i < n->size ; i++)
      free_node(n->subnodes[i]);
    free(n->subnodes);
  }
  if(n->type != NULL)
    free(n->type);
  if(n->rangeBounds != NULL)
    free(n->rangeBounds);
  if(n->objectType != NULL)
    free(n->objectType);
  free(n);
}

Device new_device(char * name, Node root){
  Device d = malloc(sizeof(struct device));
  d->root = root;
  d->name = malloc(strlen(name)+1);
  d->name = strcpy(d->name, name);
  return d;
}

Node get_root(Device d){
  return d->root;
}

char * device_name(Device d){
  return d->name;
}


void free_device(Device d){
  free_node(d->root);
  free(d->name);
  free(d);
}

/********************************************************/
/************ Reading from the config file **************/
/************   to create minuit devices   **************/
/********************************************************/

char * node_string_get_type(char * string){
  char * sub = strstr(string, "type");
  int it = 0;
  while(it < strlen(sub) && sub[it] != '{'){
    it++;
  }
  int cpt = 0;
  int i = it +1;
  while(it < strlen(sub) && sub[it] != '}'){
    if(sub[it] != ' ')
      cpt++;
    it++;
  }
  char * type = malloc(cpt*sizeof(char)+1);
  it = i;
  cpt=0;
  while(it < strlen(sub) && sub[it] != '}'){
    if(sub[i] != ' '){
      type[cpt] = sub[it];
      cpt++;
    }
    it++;
  }
  type[cpt] = '\0';
  return type;
}

char * node_string_get_objectType(char * string){
  char * sub = strstr(string, "objectType");
  int it = 0;
  while(it < strlen(sub) && sub[it] != '{'){
    it++;
  }
  int cpt = 0;
  int i = it+1;
  while(it < strlen(sub) && sub[it] != '}'){
    cpt++;
    it++;
  }
  char * objectType = malloc((cpt +1)*sizeof(char));
  it = i;
  cpt=0;
  while(it < strlen(sub) && sub[it] != '}'){
    objectType[cpt] = sub[it];
    cpt++;
    it++;
  }
  objectType[cpt] = '\0';
  return objectType;
}

char * node_string_get_description(char * string){
  char * sub = strstr(string, "description");
  int it = 0;
  while(it < strlen(sub) && sub[it] != '{'){
    it++;
  }
  int cpt = 0;
  int i = it+1;
  while(it < strlen(sub) && sub[it] != '}'){
    cpt++;
    it++;
  }
  char * description = malloc((cpt +1)*sizeof(char));
  it = i;
  cpt=0;
  while(it < strlen(sub) && sub[it] != '}'){
    description[cpt] = sub[it];
    cpt++;
    it++;
  }
  description[cpt] = '\0';
  return description;
}

int * node_string_get_rangeBounds(char * string){
  char * sub = strstr(string, "rangeBounds");
  int it = 0;
  while(it < strlen(sub) && sub[it] != '{'){
    it++;
  }
  int cpt = 0;
  int tmp = it + 1;
  int * tab = malloc(sizeof(int)*2);
  while(it < strlen(sub) && sub[it] != ',')
    it ++;
  char * nb = malloc(it * sizeof(char));
  it = tmp;
  cpt = 0;
  while(it < strlen(sub) && sub[it] != ','){
    nb[cpt] = sub[it];
    cpt++;
    it++;
  }
  nb[cpt] = '\0';
  tab[0] = atoi(nb);
  free(nb);
  cpt = 0;
  tmp = it + 1;
  while(it < strlen(sub) && sub[it] != '}')
    it ++;
  nb = malloc(it * sizeof(char));
  it = tmp;
  cpt = 0;
  while(it < strlen(sub) && sub[it] != '}'){
    nb[cpt] = sub[it];
    cpt++;
    it++;
  }
  nb[cpt] = '\0';
  tab[1] = atoi(nb);
  free(nb);
  return tab;
}

Str_array node_string_get_subnodes(char * string){
  char * sub = strstr(string, "subnodes");
  int it = 0;
  while(it < strlen(sub) && sub[it] != '{'){
    it++;
  }
  int cpt = 0;
  int tmp = it + 1;
  while(it < strlen(sub) && sub[it] != '}'){
    if(sub[it] == ',')
      cpt++;
    it++;
  }
  it = tmp;
  Str_array subnodes = new_string_array();
  for(int i = 0 ; i < cpt + 1 ; i++){
    tmp = it;
    int n = 0;
    while(sub[it] != ',' && sub[it] != '}'){
      if(sub[it] != ' ')
	n++;
      it++;
    }
    char * str = malloc((n+1)*sizeof(char));
    it = tmp;
    int j = 0;
    while(j < n){
      if(sub[it] != ' '){
	str[j] = sub[it];
	j++;
      }
      it++;
    }
    str[j] = '\0';
    it++;
    subnodes = str_array_append(subnodes, str);
    free(str);
  } 
  return subnodes;
}

int has_subnodes(char * chaine){
  return (strstr(chaine, "subnodes") != NULL);
}

int has_type(char * chaine){
  return (strstr(chaine, "type") != NULL);
}

int has_description(char * chaine){
  return (strstr(chaine, "description") != NULL);
}

int has_rangeBounds(char * chaine){
  return (strstr(chaine, "rangeBounds") != NULL);
}

int has_objectType(char * chaine){
  return (strstr(chaine, "objectType") != NULL);
}

Node new_node_file(char * name, FILE * f){
  char chaine[TAILLE_MAX];
  //char attribute[TAILLE_MAX];
  Node n = NULL;
  char * desc = NULL;
  char * type = NULL;
  char * objectType = NULL;
  int * rangeBounds = NULL;
  rewind(f);
  while(fgets(chaine, TAILLE_MAX, f) != NULL){
    int it = 0;
    while(it < strlen(chaine) && chaine[it] != ' ' && chaine[it] != '\0' && chaine[it] != '=')
      it++;
    char * head = malloc(sizeof(char) * it +1);
    for(int i = 0 ; i < it ; i ++)
      head[i] = chaine[i];
    head[it] = '\0';
    if(strcmp(head, name) == 0){
      if(has_description(chaine))
	desc = node_string_get_description(chaine);
      if(has_type(chaine))
	type = node_string_get_type(chaine);
      if(has_rangeBounds(chaine))
	rangeBounds = node_string_get_rangeBounds(chaine);
      if(has_objectType(chaine)){
	objectType = node_string_get_objectType(chaine);
      }
      n = new_node(name, desc, type, rangeBounds, objectType);
      if(desc != NULL)
	free(desc);
      if(type != NULL)
	free(type);
      if(rangeBounds != NULL)
	free(rangeBounds);
      if(objectType != NULL)
	free(objectType);
      if(has_subnodes(chaine)){
	Str_array tab = node_string_get_subnodes(chaine);
	for(int i = 0 ; i < get_size(tab) ; i++){
	  Node sub = new_node_file(get_string(tab, i), f);
	  add_subnode(n, sub);
	}
	free_str_array(tab);
      }
      free(head);
      break;
    }
    free(head);
  }
  return n;
}

Device file_device(char * file){
  char chaine[TAILLE_MAX];
  FILE * f = NULL;
  f = fopen(file, "r");
  fgets(chaine, TAILLE_MAX, f);
  chaine[strlen(chaine) - 1] = '\0';
  Device d = new_device(chaine, new_node_file(chaine, f));
  fclose(f);
  return d;
}
