/*
 * UDPserver.h
 *
 *  Created on: 9 juin 2015
 *      Author: nicho
 */

#ifndef UDPSERVER_H_
#define UDPSERVER_H_
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;
typedef struct data_string * DATA;
typedef int SOCKET;

char * ds_string(DATA d);
int ds_len(DATA d);
void free_data(DATA d);
SOCKET init_socket(int port);
DATA receive_socket(SOCKET sock);
void send_socket(SOCKET sock, DATA d,char * ip, int port);
void close_socket(SOCKET sock);
void ds_len_set(DATA d, int len);

#endif /* UDPSERVER_H_ */
