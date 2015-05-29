all: linker

linker: main.o net.o metabot.o parse.o
	gcc -L/usr/lib/x86_64-linux-gnu main.o metabot.o net.o parse.o -lSDL -lSDL_net -o linker

main.o: main.c
	gcc -std=c99 -I/usr/include/SDL -g -Wall -Werror -c main.c
	
parse.o: parse.c
	gcc -std=c99 -I/usr/include/SDL -g -Wall -Werror -c parse.c
metabot.o: metabot.c
	gcc -std=c99 -g -Wall -Werror -c metabot.c
net.o: net.c
	gcc -std=c99 -I/usr/include/SDL -g -Wall -Werror -c net.c

clean:
	rm -f *.o linker
