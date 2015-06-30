#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "metabot.h"
#include "minuit.h"

void usage(char * fct){
  printf("Usage : %s <serial_port> <port> <config_file>\n", fct);
}


int main(int argc, char **argv)
{
  if(argc < 4){
    usage(argv[0]);
    exit(1);
  }
  metabot_controller(argv[1], argv[2], argv[3]);
  return EXIT_SUCCESS;
}
