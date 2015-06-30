#include <string.h>
#include "UDPserver.h"
#include <stdlib.h>
#include <stdio.h>
#include "minuit.h"
#include "device.h"

/*********************************************************/
/**************** Minuit parsing fonctions ***************/
/*********************************************************/


int size_bytes(int i)
{
  if(i%4 == 0)
    return i;
  else
    return  i + 4 - i%4;
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
  while( i < fin - debut){
    tmp[i] = ds_string(p)[debut+i];
    i++;
  }
  tmp[i] = '\0';
  return tmp;
}

Str_array OSC_to_str_array(DATA p){
  int i = 0;
  while(i < ds_len(p) && ds_string(p)[i] != ',')
    i++;
  Str_array str = new_string_array();
  char * path = data_cpy(p, 0, i);
  str = str_array_append(str, path);
  free(path);
  char * type = data_cpy(p, i, i+2);
  str = str_array_append(str, type);
  free(type);
  char * cpy = data_cpy(p, i+4, ds_len(p));
  if(get_string(str, 1)[1] == 'i'){
    char * val = string_to_int_ASCII(cpy);
    str = str_array_append(str, val);
    free(val);
  }
  if(get_string(str, 1)[1] == 'f'){
    char * flo = string_to_float_ASCII(cpy);
    str = str_array_append(str, flo);
    free(flo);
  }
  free(cpy);
  return str;
}

Str_array minuit_to_str_array(DATA p){
  if(ds_string(p)[0] == '/')
    return OSC_to_str_array(p);
  int i = 0;
  while(i < ds_len(p) && ds_string(p)[i] != '?' && ds_string(p)[i] != ':')
    i++;
  Str_array str = new_string_array();
  char * device = data_cpy(p, 0, i);
  str = str_array_append(str, device);
  free(device);
  char * op = data_cpy(p, i, i+1);
  str = str_array_append(str, op);
  free(op);
  i++;
  int j;
  char * tmp = NULL;
  while(i <ds_len(p)){
    j = 0;
    while(j < ds_len(p) - i && ds_string(p)[j+i] != '\0')
      j++;
    tmp = data_cpy(p, i, j + i);
    str = str_array_append(str, tmp);
    free(tmp);
    i += size_bytes(j+1);
  }
  return str;
}

DATA write_minuit_packet(Str_array str){
  if(str != NULL){
    int len = 0;
    for(int i = 0; i < 3 ; i++){
      len += strlen(get_string(str, i));
    }
    len = size_bytes(len + 1);
    for(int i = 3 ; i < get_size(str) ; i++){
      len += size_bytes(strlen(get_string(str, i)) + 1);
    }
    char * buffer = malloc(len * sizeof(char));
    int it = 0;
    for(int i = 0 ; i < 3 ; i++){
      for(int j = 0 ; j < strlen(get_string(str, i)) ; j++){
	buffer[it] = get_string(str,i)[j];
	it++;
      }
    }
    while(it < size_bytes(strlen(get_string(str, 0)) + strlen(get_string(str, 1)) + strlen(get_string(str, 2)) + 1)){
      buffer[it] = '\0';
      it++;
    }
    for(int i = 3 ; i < get_size(str) ; i++){
      for(int j = 0 ; j < strlen(get_string(str, i)) ; j++){
	buffer[it] = get_string(str, i)[j];
	it++;
      }
      for(int j = strlen(get_string(str, i)) ; j < size_bytes(strlen(get_string(str, i)) +1 ) ; j++){
	buffer[it] = '\0';
	it++;
      }
    }
    DATA d = init_data(buffer, len);
    free(buffer);
    return d;
  }
  else
    return NULL;
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
  if(strcmp(get_string(str, 2), "namespace") == 0)
    return discover;
  if(strcmp(get_string(str, 2), "get" )== 0)
    return get;
  if(strcmp(get_string(str, 2), "set") == 0)
    return set;
  if(strcmp(get_string(str, 2), "listen") == 0)
    return listen;
  else
    return unknown_query;
}

Protocol protocol(Str_array str){
  if(get_string(str, 0)[0] == '/')
    return OSC;
  if(get_string(str, 1)[0] == ':')
    return minuit_reply;
  if(get_string(str, 1)[0] == '?')
    return minuit_query;
  else
    return unknown_protocol;
}

//Réponse à une minuit_query
Str_array namespace_answer(Device d, Str_array src){
  char * path = get_string(src, 4);
  Str_array str = new_string_array();
  str = str_array_append(str, node_name(get_root(d)));
  str = str_array_append(str, ":");
  str = str_array_append(str, "namespace");
  if(!strcmp(path, "/")){
    //fabriquer namespace général
    char * type = malloc(sizeof(char)*(node_size(get_root(d)) + 6));
    type[0] = ',';
    for(int i = 1 ; i < node_size(get_root(d))+5 ; i++)
      type[i] = 's';
    type[node_size(get_root(d)) + 5] = '\0';
    str = str_array_append(str, type);
    free(type);
    str = str_array_append(str, "/");
    str = str_array_append(str, "Application");
    str = str_array_append(str, "nodes={");
    for(int i = 0 ; i < node_size(get_root(d)) ; i++)
      str = str_array_append(str, node_name(node_subnodes(get_root(d))[i]));
    str = str_array_append(str, "}");
  }
  else{
    //fabriquer namespace pour la node située à l'adresse path
    char * name = path_to_name(path);
    Node n = find_node(get_root(d), name);
    free(name);
    if(n == NULL){
      printf("Couldn't find node\n");
      return NULL;
    }
    char * type = NULL;
    if(strcmp(node_objectType(n), "Container")==0){
      type = malloc(sizeof(char)*(node_size(n) + 6));
      type[0] = ',';
      for(int i = 1 ; i < node_size(n)+5 ; i++)
	type[i] = 's';
      type[node_size(n)+5] = '\0';
    }
    else if (strcmp(node_objectType(n), "Data")==0){
      type = malloc(sizeof(char)*5);
      type[0] = ',';
      for(int i = 1 ; i < 3 ; i++)
	type[i] = 's';
      type[3] = '\0';
    }
    if(type != NULL){
      str = str_array_append(str, type);
      free(type);
    }
    str = str_array_append(str, path);

    str = str_array_append(str, node_objectType(n));
    if(strcmp(node_objectType(n), "Container")==0){
      str = str_array_append(str, "nodes={");
      for(int i = 0 ; i < node_size(n) ; i++)
	str = str_array_append(str, node_name(node_subnodes(n)[i]));
      str = str_array_append(str, "}");
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

Str_array get_answer(Device d, Str_array str){
  char * name = path_to_name(get_string(str, 4));
  Node n = find_node(get_root(d), name);
  Str_array answer = new_string_array();
  if( n == NULL )
    printf("Node %s doesn't exist\n", name);
  else{
    answer = str_array_append(answer, device_name(d));
    answer = str_array_append(answer, ":");
    answer = str_array_append(answer, "get");
    switch(get_attribute(get_string(str, 4))){
    case value:
      free_str_array(answer);
      answer = NULL;
      break;
    case type:
      answer = str_array_append(answer, ",ss");
      answer = str_array_append(answer, get_string(str, 4));
      answer = str_array_append(answer, node_type(n));
      break;
    case tags:
      free_str_array(answer);
      answer = NULL;
      break;
    case rangeClipmode:
      free_str_array(answer);
      answer = NULL;
      break;
    case service:
      answer = str_array_append(answer, ",ss");
      answer = str_array_append(answer, get_string(str, 4));
      answer = str_array_append(answer, node_service(n));
      break;
    case priority:
      ;
      char * p = malloc(sizeof(char)*5);
      sprintf(p,"%d", node_priority(n));
      answer = str_array_append(answer, ",si");
      answer = str_array_append(answer, get_string(str, 4));
      answer = str_array_append(answer, p);
      free(p);
      break;
    case rangeBounds:
      free_str_array(answer);
      answer = NULL;
      break;
    case description:
      answer = str_array_append(answer, ",ss");
      answer = str_array_append(answer, get_string(str, 4));
      answer = str_array_append(answer, node_desc(n));
      break;
    case unknown_attribute:
      free_str_array(answer);
      answer = NULL;
      break;
    }
  }
  free(name);
  return answer;
}

Str_array minuit_answer(Device d, Str_array str){
  Str_array answer = NULL;
  switch(get_query(str)){
  case discover:
    answer = namespace_answer(d, str);
    break;
  case get:
    answer = get_answer(d, str);
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
