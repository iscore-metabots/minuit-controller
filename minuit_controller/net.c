#include "com.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * \brief Renvoie le plus petit nombre n d'octets supérieur à la taille de s tel que n soit un multiple de 4.
 * \param s chaine à analyser
 * \return nombre d'octets multiple de 4
 */
int size_bytes(char * s)
{
    if(strlen(s)%4 == 0)
        return strlen(s);
    else
        return  strlen(s) + 4 - strlen(s)%4;
}

/**
 * \brief Ecrit dans le champ p->data la chaine de caractères s à partir de l'itérateur it
 * Cette fonction doit être utilisée de la façon suivante si on veut l'utiliser plusieurs fois :
 * it = write_data(p,s,it);
 * \param *p paquet à éditer
 * \param s chaine de caracteres à écrire
 * \param it itérateur
 * \return valeur de l'itérateur après passage sur p->data
 */
int write_data(UDPpacket *p, char * s, int it)
{
    for(int j = 0 ; j < size_bytes(s) ; j++)
    {
        if(j < strlen(s))
            p->data[it] = s[j];
        else
            p->data[it] = 0;
        it++;
    }
    return it;
}

void send_answer(char ** cmd, int port)
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

    p->len = 0;
    int it = 0;
    for(int i_cmd = 0 ; i_cmd < sizeof(cmd) ; i_cmd ++)
    {
        p->len += size_bytes(cmd[i_cmd]);
        it = write_data(p, cmd[i_cmd], it);
    }

    p->address.host = srvadd.host;	/* Set the destination host */
    p->address.port = srvadd.port;	/* And destination port */

    SDLNet_UDP_Send(sd, -1, p); /* This sets the p->channel */
    SDLNet_FreePacket(p);
    SDLNet_Quit();
}
