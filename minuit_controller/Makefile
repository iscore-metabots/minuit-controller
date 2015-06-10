all: linker

linker: main.o minuit.o metabot.o UDPserver.o
	gcc -L/usr/lib/x86_64-linux-gnu main.o metabot.o minuit.o UDPserver.o -o linker

main.o: main.c
	gcc -std=c99 -g -Wall -Werror -c main.c
	
minuit.o: minuit.c
	gcc -std=c99 -g -Wall -Werror -c minuit.c
metabot.o: metabot.c
	gcc -std=c99 -g -Wall -Werror -c metabot.c
UDPserver.o: UDPserver.c
	gcc -std=c99 -g -Wall -Werror -c UDPserver.c

clean:
	rm -f *.o linker
