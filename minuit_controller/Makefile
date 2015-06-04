all: linker

linker: main.o minuit.o metabot.o
	gcc -L/usr/lib/x86_64-linux-gnu main.o metabot.o minuit.o -lSDL -lSDL_net -o linker

main.o: main.c
	gcc -std=c99 -I/usr/include/SDL -g -Wall -Werror -c main.c
	
minuit.o: minuit.c
	gcc -std=c99 -I/usr/include/SDL -g -Wall -Werror -c minuit.c
metabot.o: metabot.c
	gcc -std=c99 -I/usr/include/SDL -g -Wall -Werror -c metabot.c

clean:
	rm -f *.o linker
