#ifndef STR_ARRAY_H_
#define STR_ARRAY_H_

#define ARRAY_SIZE 20

/*************************************/
/**** Str_array related functions ****/
/*************************************/

typedef struct str_array * Str_array;

/**
 * \brief Creates a string array. This array needs to be freed.
 */
Str_array new_string_array();

/**
 * \brief Adds the s string at the end of str.
 * \param str String array
 * \param s String
 */
Str_array str_array_append(Str_array str, char * s);

/**
 * \brief Returns the i-th string of the array
 * \param str String array
 * \param i string index
 */
char * get_string(Str_array str, int i);

/**
 * \brief Prints the array str
 */
void print_str_array(Str_array str);

/**
 * \brief Frees a string array
 */
void free_str_array(Str_array str);

/**
 * \brief returns the number of string in a string array
 */
int get_size(Str_array str);


#endif
