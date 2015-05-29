/*
 * metabot.h
 *
 *  Created on: 29 mai 2015
 *      Author: nicho
 */

#ifndef METABOT_H_
#define METABOT_H_

#define DEBUG_MODE 1

#define NB_NODES 3

typedef struct node * Node;
typedef Node * Metabot;

/**
 * \brief Initialise un métabot
 * \return metabot initialisé
 */
extern Metabot new_metabot();

/**
 * \brief Affiche les nodes et leurs attributs du métabot m
 * \param m Metabot à afficher
 */
extern void display_metabot(Metabot m);

/**
 * \brief libère la mémoire allouée à un metabot
 * \param m Metabot
 */
extern void free_metabot(m);

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
char ** namespace_node_cmd_array(Metabot m, char * node);

/**
 * \brief Execute la commande cmd sur le fichier fd (correspondant au metabot)
 * \param cmd Commande
 * \param fd Fichier correspondant au metabot
 */
void execute(char * cmd, int fd);

#endif /* METABOT_H_ */
