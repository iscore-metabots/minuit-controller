#ifndef MINUIT_H_
#define MINUIT_H_

#define ARRAY_SIZE 20

#include "UDPserver.h"

enum protocol { OSC, minuit_reply, minuit_query, unknown_protocol };
enum query{ discover, get, set, listen, unknown_query};
enum attribute{ value, type, service, priority, rangeBounds, rangeClipmode, tags, description, unknown_attribute };
enum object_type{Application, Container, Data, none };

typedef enum protocol Protocol;
typedef enum query Query;
typedef enum attribute Attribute;
typedef enum object_type ObjectType;

typedef struct str_array * Str_array;
typedef struct node * Node;
typedef struct device * Device;

Device new_device(char * name, Node root);
Device add_node(Device d, Node n);
void add_subnode(Node n, Node subn);

void free_device(Device d);
Node get_root(Device d);
Node find_node(Node n, char * name);
Str_array minuit_answer(Device d, Str_array str);
Query get_query(Str_array str);
char * path_to_name(char * path);
/**
 * \brief Envoie le message contenu dans "cmd" en UDP sur le port "port"
 * Chaque case de cmd sera écrite dans un bloc d'une taille multiple de 32 bits dans le message final.
 * \param cmd Tableau de chaines de caractères.
 * \param port Port
 */
extern void send_answer(Str_array str, int port);
Protocol protocol(Str_array str);

Str_array OSC_to_str_array(DATA p);

char * str_array_to_cmd(Str_array str);

Str_array minuit_to_str_array(DATA p);

/**
 * \brief Initialise une node
 * \param name Nom de la node
 * \param description Description de la node
 * \param attributes Tableau de chaines de caractères contenant les attributs de la node
 * \param size Taille du tableau attributes
 * \return Node initalisée
 */
Node new_node(char * name, char * description);
/**
 * \brief Libère une node
 * \param n Node
 */
void free_node(Node n);

/**
 * \brief renvoie le nom de la node passée en paramètre
 * \param n Node
 * \return nom de n
 */
char * node_name(Node n);

/**
 * \brief renvoie la description de la node passée en paramètre
 * \param n Node
 * \return description de n
 */
char * node_desc(Node n);

/**
 * \brief renvoie le tableau d'attributs de la node passée en paramètre
 * \param n Node
 * \return tableau d'attributs de n
 */
char ** node_attr(Node n);

/**
 * \brief renvoie le nombre d'attributs de la node passée en paramètre
 * \param n Node
 * \return nombre d'attributs de n
 */
int node_size(Node n);

void display_node(Node n);

Str_array namespace_answer(Device d, Str_array src);

char * get_string(Str_array str, int i);
Str_array new_string_array();
Str_array str_array_append(Str_array str, char * s);
void free_str_array(Str_array str);
void print_str_array(Str_array str);
DATA write_minuit_packet(Str_array str);

#endif /* MINUIT_H_ */
