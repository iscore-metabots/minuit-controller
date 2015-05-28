all: controller

controller: main.o controller.o
	gcc -L/usr/lib/x86_64-linux-gnu main.o controller.o -lSDL -lSDL_net -o controller

main.o: main.c
	gcc -std=c99 -I/usr/include/SDL -g -Wall -Werror -c main.c
controller.o: controller.c
	gcc -std=c99 -I/usr/include/SDL -O2 -g -Wall -Werror -c controller.c

clean:
	rm -f *.o controller
