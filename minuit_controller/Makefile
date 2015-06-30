CC=clang-3.6
CFLAGS= -std=c99 -g -Wall -Werror

all: linker music

linker: main.o minuit.o metabot.o UDPserver.o device.o str_array.o
	g++ -g -L/usr/lib/x86_64-linux-gnu  main.o str_array.o metabot.o minuit.o device.o UDPserver.o -o linker

music: musicPlayer.o minuit.o UDPserver.o str_array.o
	$(CC) -o music musicPlayer.o minuit.o UDPserver.o str_array.o device.o `sdl-config --libs` -lSDL_mixer

musicPlayer.o: musicPlayer.c
	$(CC) -c `sdl-config --cflags` musicPlayer.c

main.o: main.cpp
	g++ -g -Wall -Werror -c main.cpp
minuit.o: minuit.c
	$(CC) $(CFLAGS) -c minuit.c
device.o: device.c
	$(CC) $(CFLAGS) -c device.c
metabot.o: metabot.c
	$(CC) $(CFLAGS) -c metabot.c
str_array.o: str_array.c
	$(CC) $(CFLAGS) -c str_array.c
UDPserver.o: UDPserver.c
	$(CC) $(CFLAGS) -c UDPserver.c
controller.o: controller.cpp
	g++ -g -Wall -Werror -c controller.cpp
clean:
	rm -f *.o linker music recv_test
