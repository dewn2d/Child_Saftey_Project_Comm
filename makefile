CC=gcc
CFLAGS=-lpthread
DEPS = comm.h
OBJ = comm.o main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

comm: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)