#include <string.h>
#include "com.h"

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

char * OSC_to_Metabot(UDPpacket *p){
	char * cmd = malloc(sizeof(*cmd)*32);
	int i = 0;
	//comptage du nombre de caractères de la commande
	while(p->data[i]!=',')
	{
		i++;
	}
	//copie de la commande
	cmd = strncpy(cmd, (char *)p->data+1, i);

	//la séquence est elle terminée ?
	i++;
	//type de l'argument
	char type = p->data[i];
	i+=3;

	//copie de l'argument
	float val=0;
	if(type == 'f')
	{
		long int * d = malloc(sizeof(long int));
		for(int j = 0; j < 4 ; j++)
		{
			*d <<= 8;
			*d += p->data[i+j];
		}
		float * f = (float * )d;
		val = *f;
		free(d);
	}
	if(type == 'i')
	{
		long int data = 0;
		for(int j = 0; j < 4 ; j++)
		{
			data <<= 8;
			data += p->data[i+j];
		}
		val = (float) data;
	}

	//concaténation de la commande
	sprintf( cmd, "%s %f\n", cmd, val );

	return cmd;
}

char * get_node_namespace(UDPpacket *p){
	int i = 0;
	while(i < p->len && p->data[i] != '/')
		i++;
	//copie de la commande
	char * name = malloc(32*sizeof(char));
	name = strcpy(name, (char *)p->data+i);
	return name;
}
