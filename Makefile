
all : utils server client

utils:
	g++ -o utils.o -c src/utils.c

server:
	g++ -o server utils.o src/server.c

client:
	g++ -o client utils.o src/client.c

clean:
	rm -f *.o server client