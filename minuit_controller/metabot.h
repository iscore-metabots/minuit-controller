/*
 * metabot.h
 *
 *  Created on: 29 mai 2015
 *      Author: nicho
 */

#ifndef METABOT_H_
#define METABOT_H_
#include "minuit.h"

#define DEBUG_MODE 1 /* 0 active le robot, 1 ne l'active pas */

#define NB_NODES 3

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
 * \brief Execute la commande cmd sur le metabot m
 * \param cmd Commande
 * \param m Metabot
 */
void execute(char * cmd, Metabot m);

void free_cmd_array(char ** cmd);

Device get_device(Metabot m);

#endif /* METABOT_H_ */
