
all : utils commands server client

utils:
	gcc -o utils.o -c src/utils.c

commands:
	gcc -o commands.o -c src/commands.c

server:
	gcc -o server utils.o commands.o src/server.c

client:
	gcc -o client utils.o commands.o src/client.c

clean:
	rm -f *.o server client
