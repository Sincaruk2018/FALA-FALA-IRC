
all : utils server client

utils:
	gcc -o utils.o -c src/utils.c

server:
	gcc -o server utils.o src/server.c

client:
	gcc -o client utils.o src/client.c

clean:
	rm -f *.o server client