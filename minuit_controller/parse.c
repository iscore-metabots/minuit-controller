/*
 * parse.c
 *
 *  Created on: 29 mai 2015
 *      Author: nicho
 */

#include "parse.h"
#include <string.h>

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
