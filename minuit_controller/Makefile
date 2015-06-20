CC=clang-3.6
CFLAGS= -std=c99 -g -Wall -Werror

all: linker music recv_test recv_test_2

linker: main.o minuit.o metabot.o UDPserver.o
	$(CC) -g -L/usr/lib/x86_64-linux-gnu  main.o metabot.o minuit.o UDPserver.o -o linker

music: musicPlayer.o minuit.o UDPserver.o
	$(CC) -o music musicPlayer.o minuit.o UDPserver.o `sdl-config --libs` -lSDL_mixer

recv_test: recv_test.o UDPserver.o minuit.o
	$(CC) -o recv_test recv_test.o UDPserver.o minuit.o

recv_test_2: recv_test_2.o UDPserver.o
	$(CC) -o recv_test_2 recv_test_2.o UDPserver.o

recv_test.o: recv_test.c
	$(CC) $(CFLAGS) -c recv_test.c
recv_test_2.o: recv_test_2.c
	$(CC) $(CFLAGS) -c recv_test_2.c

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
	rm -f *.o linker music recv_test
