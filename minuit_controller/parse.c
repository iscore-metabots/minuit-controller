#include <string.h>
#include "com.h"

struct str_array{
	int size;
	char ** t;
};


int size_bytes2(int i)
{
	if(i%4 == 0)
		return i;
	else
		return  i + 4 - i%4;
}

Str_array new_string_array(char * s){
	Str_array str = malloc(sizeof(struct str_array));
	str->t = malloc(sizeof(char *));
	str->t[0] = malloc(size_bytes2(strlen(s)));
	str->t[0] = memcpy(str->t[0], s, size_bytes2(strlen(s)));
	str->size = 1;
	return str;
}

Str_array str_array_append(Str_array str, char * s){
	str->t = realloc(str->t, (str->size +1)*sizeof(char *));
	int l = size_bytes2(strlen(s));
	if(l < 4)
		l=4;
	str->t[str->size] = malloc(l+1);
	str->t[str->size] = memcpy(str->t[str->size], s, l);
	str->size += 1 ;
	return str;
}

void free_str_array(Str_array str){
	for(int i = 0 ; i < str->size ; i++){
		free(str->t[i]);
	}
	free(str);
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
Protocol get_protocol(UDPpacket *p){
	if(p->data[0] == '/')
		return OSC;
	int i = 0;
	while(i < p->len){
		i++;
		if(p->data[i] == '?')
		{
			if(strcmp((char*)p->data+i, "namespace")){
				while(i < p->len){
					i++;
					if(p->data[i] == '/'){
						if(p->data[i+1] == 0)
							return minuit_namespace;
						else
							return minuit_node;
					}

				}
			}
		}
		if(p->data[i] == ':')
			return minuit_reply;
	}
	return unknown;
}

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
	for(int i = 0 ; i < str->size ; i++){
		printf("%s  ", str->t[i]);
	}
	printf("\n");
	return str;
}

char * str_array_to_cmd(Str_array str){
	char * cmd = malloc((strlen(str->t[0]) + 6)*sizeof(char));
	sprintf( cmd, "%s %s\n", str->t[0]+1, str->t[2]);
	return cmd;
}

