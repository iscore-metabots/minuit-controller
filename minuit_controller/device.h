#ifndef DEVICE_H_
#define DEVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define TAILLE_MAX 1000

typedef struct node * Node;
typedef struct device * Device;

  /********************************************************/
  /******* Minuit device and node related fonctions *******/
  /********************************************************/

  /**
   * \brief Initialises a node. The node needs to be freed with "free_node". This node is intended to be a minuit device node : this means you should know the minuit specification in order to use it correctly. (https://github.com/Minuit/Minuit)
   * \param name name of the node
   * \param description Short description of the node
   * \param type Value type
   * \param rangeBounds Array of 2 numbers filled with the minimum and maximum value of the node
   * \param objectType Object type (Data, Container...)
   */
  Node new_node(char * name, char * description, char * type, int * rangeBounds, char * objectType);

  /**
   * \brief Initialisation of a minuit device. The device should be freed with free_device.
   */
  Device new_device(char * name, Node root);

  /**
   * \brief Adds a node to the children of the node. If the parent node is freed, it's children will be freed too.
   */
  void add_subnode(Node n, Node subn);


  /**
   *\brief Prints a node and all its children
   */
  void display_node(Node n);

  /**
   * \brief Search a node by name in n's children
   */
  Node find_node(Node n, char * name);

  /**
   * \brief Frees a node and all its children. If the node has a parent, it will be removed from its children.
   * \param n Node
   */
  void free_node(Node n);

  /**
   * \brief Frees a minuit device and its root node
   */
  void free_device(Device d);

  /****************************************************/
  /********************** Getters *********************/
  /****************************************************/

  /**
   * \brief returns a node's name
   * \param n Node
   */
  char * node_name(Node n);

  /**
   * \brief Returns a node description
   * \param n Node
   * \return description de n
   */
  char * node_desc(Node n);

  /**
   * \brief Returns the number of children of the node
   * \param n Node
   */
  int node_size(Node n);

  /**
   * \brief Returns a node's object type
   */
  char * node_objectType(Node n);

  /**
   *\brief Returns a node's service
   */
  char * node_service(Node n);

  /**
   *\brief Returns a node's priority
   */
  int node_priority(Node n);

  /**
   *\brief Returns a node's subnode array
   */
  Node * node_subnodes(Node n);

  /**
   * \brief returns a node's value type
   */
  char * node_type(Node n);

  /**
   * \brief returns a node's service
   */
  char * node_service(Node n);

  /**
   * \brief Returns the device d's root node
   */
  Node get_root(Device d);

  /**
   * \brief returns a device's name
   */
  char * device_name(Device d);


  /********************************************************/
  /************ Reading from the config file **************/
  /************   to create minuit devices   **************/
  /********************************************************/

  /**
   * \brief initialises a device from a config file.
   * The first line of the config file contains the device name.
   * The next lines contain nodes info with the following syntax:
   * name = {<a>, <b>, <c>, ...}
   * where a, b, c ... are the node's properties : objectType, subnodes, desciption, rangeBounds, type.
   * ex : moves = {objectType={Container}, subnodes={h, dx, dy, turn}, description={"Fonctions de mouvement"}}
   * ex 2 : h = {objectType={Data}, type={decimal}, rangeBounds={-30,-120}, description={"Change la hauteur du metabot"}}
   * The returned device has to be freed at some point.
   */
  Device file_device(char * file);

#ifdef __cplusplus
}
#endif

#endif /* DEVICE_H_ */
