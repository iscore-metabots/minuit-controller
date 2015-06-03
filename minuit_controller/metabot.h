/*
 * metabot.h
 *
 *  Created on: 29 mai 2015
 *      Author: nicho
 */

#ifndef METABOT_H_
#define METABOT_H_


#define DEBUG_MODE 1 /* 0 active le robot, 1 ne l'active pas */

#define NB_NODES 3

typedef struct node * Node;
typedef struct metabot * Metabot;

/**
 * \brief Initialise un métabot et se connecte au périphérique
 * \param path Chemin du périphérique metabot
 * \return metabot initialisé
 */
extern Metabot new_metabot(char * path);

/**
 * \brief Envoie la commande "start" sur le metabot
 * \param m Metabot
 */
extern void start(Metabot m);

/**
 * \brief Envoie la commande "stop" sur le metabot
 * \param m Metabot
 */
extern void stop(Metabot m);

/**
 * \brief Affiche les nodes et leurs attributs du métabot m
 * \param m Metabot à afficher
 */
extern void display_metabot(Metabot m);

/**
 * \brief libère la mémoire allouée à un metabot
 * \param m Metabot
 */
extern void free_metabot(Metabot m);

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

/**
 * \brief Renvoie un tableau contenant les blocs qui composent le message réponse à la requête "namespace" pour le chemin "/"
 * \param m metabot
 * \return Tableau de chaines de caractères
 */
char ** namespace_cmd_array(Metabot m);

/**
 * \brief Renvoie un tableau contenant les blocs qui composent le message réponse à la requête "namespace" pour le chemin "/node"
 * \param m metabot
 * \param node chaine de caractères contenant le nom de la node
 * \return Tableau de chaines de caractères
 */
char ** namespace_node_cmd_array(Node n);

/**
 * \brief Execute la commande cmd sur le metabot m
 * \param cmd Commande
 * \param m Metabot
 */
void execute(char * cmd, Metabot m);

int node_search(Metabot m, char * name);

Node get_node(Metabot m, int i);

void free_cmd_array(char ** cmd);

#endif /* METABOT_H_ */
