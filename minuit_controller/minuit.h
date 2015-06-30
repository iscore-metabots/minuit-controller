#ifndef MINUIT_H_
#define MINUIT_H_

#include "UDPserver.h"
#include "device.h"
#include "str_array.h"

#ifdef __cplusplus
extern "C" {
#endif
  /******************************************************************/
  /* See minuit specification for informations about the minuit     */
  /* protocol : https://github.com/Minuit/Minuit                    */
  /*                                                                */
  /* This implementation isn't complete, but is enough to make a    */
  /* device that is composed of a simple container/data tree.       */
  /******************************************************************/
  
  enum protocol { OSC, minuit_reply, minuit_query, unknown_protocol };
  enum query{ discover, get, set, listen, unknown_query};
  enum attribute{ value, type, service, priority, rangeBounds, rangeClipmode, tags, description, unknown_attribute };
  typedef enum protocol Protocol;
  typedef enum query Query;
  typedef enum attribute Attribute;

  Protocol protocol(Str_array str);
  Query get_query(Str_array str);
  Attribute get_attribute(char * s);



  /**
   * \brief returns a str array containing the apropriate answer for the minuit message in str.
   */
  Str_array minuit_answer(Device d, Str_array str);

  /**
   * \brief Sends the minuit message contained in str to the port "port"
   * \param str Str_array containing a minuit message
   * \param port Port
   */
  void send_answer(Str_array str, int port);

  /**
   * \brief translates an OSC message into a string array
   */
  Str_array OSC_to_str_array(DATA p);

  /**
   * \brief translates a minuit message into a string array
   */
  Str_array minuit_to_str_array(DATA p);

  /**
   * \brief returns an appropriate answer for a namespace query src. Returns NULL if the query asks for a node that doesn't exist.
   */
  Str_array namespace_answer(Device d, Str_array src);

  /**
   * \brief translates a string array into a minuit message
   * The 3 first cases of the array should contain : the device name, the query/answer mark, and the operation. The following cases must contain the elements that have to be separated into 32 bit blocks (see minuit specification)
   */
  DATA write_minuit_packet(Str_array str);

  /**
   * \brief returns the last part of a path. ex : /dev/rfcomm0 becomes rfcomm0
   */
  char * path_to_name(char * path);

#ifdef __cplusplus
}
#endif

#endif /* MINUIT_H_ */
