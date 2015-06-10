/*
 * UDPserver.c
 *
 *  Created on: 9 juin 2015
 *      Author: nicho
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "UDPserver.h"
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1


struct data_string{
  char * data;
  int len;
};

void free_data(DATA d){
  free(d->data);
  free(d);
}

char * ds_string(DATA d){
	return d->data;
}

int ds_len(DATA d){
	return d->len;
}

void ds_len_set(DATA d, int len){
	d->len = len;
}

SOCKET init_socket(int port){
  SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock == INVALID_SOCKET)
    {
      perror("socket()");
      exit(errno);
    }

  SOCKADDR_IN sin = { 0 };

  sin.sin_addr.s_addr = htonl(INADDR_ANY);

  sin.sin_family = AF_INET;

  sin.sin_port = htons(port);

  if(bind (sock, (SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR)
    {
      perror("bind()");
      exit(errno);
    }
  return sock;
}

DATA receive_socket(SOCKET sock){
  DATA p = malloc(sizeof(struct data_string));
  p->data = malloc(1024);
  //struct hostent *hostinfo = NULL;
  SOCKADDR_IN from = { 0 };
  unsigned int fromsize = sizeof(from);
  if((p->len = recvfrom(sock, p->data, 1024 - 1, 0, (SOCKADDR *)&from, &fromsize)) < 0)
    {
      perror("recvfrom()");
      exit(errno);
    }

  p->data[p->len] = '\0';
  return p;
}

void send_socket(SOCKET sock, DATA d,char * ip, int port){
  struct sockaddr_in * to = malloc(sizeof(struct sockaddr_in));
  to->sin_addr.s_addr = inet_addr(ip);     // we specifed the address as a string, inet_addr translates to a number in the correct byte order
  to->sin_family = AF_INET;                         // ipv4
  to->sin_port = htons(port);                // set port address (is this the sender's port or the receiver's port

  if(sendto(sock, d->data, d->len, 0, (SOCKADDR *)to, sizeof(*to)) < 0)
    {
      perror("sendto()");
      exit(errno);
    }
}

void close_socket(SOCKET sock){
  close(sock);
}
