#ifndef UDPSERVER_H_
#define UDPSERVER_H_
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;
typedef int SOCKET;

/* Data structure :
 * A data structure is a string that can contain '\0' symbols.
 * Every UDP message received will be transformed into a data structure
 */
typedef struct data_string * DATA;

/**
 * \brief returns a pointer to the string contained by a data structure
 */
char * ds_string(DATA d);

/**
 * \brief returns the length of a data structure's string
 */
int ds_len(DATA d);

/**
 * \brief initialises a data structure
 * This operation allocates memory : use free_data to free the DATA
 */
DATA init_data();

/**
 *\brief frees the data structure
 */
void free_data(DATA d);

/**
 * \brief initialises a socket on port "port"
 */
SOCKET init_socket(int port);

/**
 * \brief receives a socket on the socket sock. This function is blocking.
 */
DATA receive_socket(SOCKET sock);

/**
 * \brief sends a socket containing DATA d to the address "ip" on the port "port"
 */
void send_socket(SOCKET sock, DATA d, char * ip, int port);

/**
 * \brief frees and closes a socket
 */
void close_socket(SOCKET sock);

/**
 * \brief sets a data length (useful when you want to modify its string)
 */
void ds_len_set(DATA d, int len);

#endif /* UDPSERVER_H_ */
