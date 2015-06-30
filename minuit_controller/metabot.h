/*
 * metabot.h
 *
 *  Created on: 29 mai 2015
 *      Author: nicho
 */

#ifndef METABOT_H_
#define METABOT_H_

#include "minuit.h"
#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG_MODE 0 /* 0 activates the robot, 1 doesn't */

#define NB_NODES 3

  typedef struct metabot * Metabot;

  /**
   * \brief Initialises a metabot and connects it if DEBUG_MODE == 1
   * \param path serial port
   */
  extern Metabot new_metabot(char * path, char * configfile);

  /**
   * \brief Sends "start" command to the metabot
   * \param m Metabot
   */
  extern void start(Metabot m);

  /**
   * \brief Sends "stop" command to the metabot
   * \param m Metabot
   */
  extern void stop(Metabot m);

  /**
   * \brief displays the metabot inuit device tree
   * \param m Metabot
   */
  extern void display_metabot(Metabot m);

  /**
   * \brief frees a metabot
   * \param m Metabot
   */
  extern void free_metabot(Metabot m);

  /**
   * \brief executes a command on the metabot
   * \param cmd Command
   * \param m Metabot
   */
  void execute(char * cmd, Metabot m);

  /**
   * \brief returns a metabot's device
   */
  Device get_device(Metabot m);

  /**
   * \brief Main loop function. Initialises a metabot, connects to it and runs the UDP server. This function contains recv which blocks the function until a message arrives. You need to send a OSC message containing "quit" in order to stop it.
   */
  void metabot_controller(char * serialport, char * port, char * configfile);

#ifdef __cplusplus
}
#endif

#endif /* METABOT_H_ */
