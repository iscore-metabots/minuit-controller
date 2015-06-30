#include <stdio.h>
#include "str_array.h"
#include <string.h>
#include <stdlib.h>
/***************************************************************/
/************ String array structure and fonctions *************/
/***************************************************************/

struct str_array{
  int size;
  char ** t;
  int max_size;
};

Str_array new_string_array(){
  Str_array str = malloc(sizeof(struct str_array));
  str->t = malloc(ARRAY_SIZE * sizeof(char *));
  str->size = 0;
  str->max_size = ARRAY_SIZE;
  return str;
}

void free_str_array(Str_array str){
  for(int i = 0 ; i < str->size ; i++){
    free(str->t[i]);
  }
  free(str->t);
  free(str);
}

Str_array str_array_append(Str_array str, char * s){
  if(str->size >= str->max_size){
    char ** t = malloc(str->max_size * 2 * sizeof(char*));
    str->max_size = str->max_size*2;
    for(int i = 0 ; i < str->size ; i++)
      t[i] = str->t[i];
    free(str->t);
    str->t = t;
  }
  int len = strlen(s);
  str->t[str->size] = malloc((len+1)*sizeof(char));
  str->t[str->size] = memcpy(str->t[str->size], s, len);
  str->t[str->size][len] = '\0';
  str->size += 1 ;
  return str;
}

void print_str_array(Str_array str){
  for(int i = 0 ; i < str->size ; i++){
    printf("%s  ", str->t[i]);
  }
  printf("\n");
}

char * get_string(Str_array str, int i){
  if(i < str->size)
    return str->t[i];
  else{
    printf("Erreur de segmentation\n");
    return "";
  }
}

int get_size(Str_array str){
  return str->size;
}
