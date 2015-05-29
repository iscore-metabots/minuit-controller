all: controller

controller: main.o controller.o metabot.o parse.o
	gcc -L/usr/lib/x86_64-linux-gnu main.o metabot.o controller.o parse.o -lSDL -lSDL_net -o controller

main.o: main.c
	gcc -std=c99 -I/usr/include/SDL -g -Wall -Werror -c main.c
	
parse.o: parse.c
	gcc -std=c99 -I/usr/include/SDL -g -Wall -Werror -c parse.c
metabot.o: metabot.c
	gcc -std=c99 -g -Wall -Werror -c metabot.c
controller.o: controller.c
	gcc -std=c99 -I/usr/include/SDL -g -Wall -Werror -c controller.c

clean:
	rm -f *.o controller
