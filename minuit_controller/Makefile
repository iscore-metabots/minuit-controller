CC=clang-3.6
CFLAGS=-fsanitize=address -fsanitize=undefined -std=c99 -g -Wall -Werror

all: linker music

linker: main.o minuit.o metabot.o UDPserver.o
	$(CC) -g -L/usr/lib/x86_64-linux-gnu -fsanitize=address -fsanitize=undefined main.o metabot.o minuit.o UDPserver.o -o linker

music: musicPlayer.o minuit.o UDPserver.o
	$(CC) -o music musicPlayer.o minuit.o UDPserver.o -fsanitize=address -fsanitize=undefined `sdl-config --libs` -lSDL_mixer

musicPlayer.o: musicPlayer.c
	$(CC) -c `sdl-config --cflags` musicPlayer.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c
	
minuit.o: minuit.c
	$(CC) $(CFLAGS) -c minuit.c
metabot.o: metabot.c
	$(CC) $(CFLAGS) -c metabot.c
UDPserver.o: UDPserver.c
	$(CC) $(CFLAGS) -c UDPserver.c

clean:
	rm -f *.o linker music
