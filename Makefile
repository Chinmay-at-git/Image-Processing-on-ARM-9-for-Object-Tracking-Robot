CC=arm-linux-gcc
CFLAGS=  -Wall
OBJECTS= main.o

all: spc_test4

spc_test4: $(OBJECTS)
	$(CC) -o spc_test4 $(OBJECTS) $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -rf *o spc_test4
