# Makefile para compilar as 3 versoes do Projeto CDN
CC = gcc
CFLAGS = -Wall -O3 -fopenmp
DEPS = hash_table.h utils.h
OBJ = hash_table.o

all: analyzer_seq analyzer_par_critical analyzer_par_atomic

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

analyzer_seq: analyzer_seq.c $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

analyzer_par_critical: analyzer_par_critical.c $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

analyzer_par_atomic: analyzer_par_atomic.c $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o analyzer_seq analyzer_par_critical analyzer_par_atomic results.csv