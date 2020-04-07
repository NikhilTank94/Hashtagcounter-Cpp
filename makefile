CC=g++
CFLAGS=-I.

hashtagcounter: fHeap.o hashtagcounter.o
	$(CC) -o hashtagcounter fHeap.o hashtagcounter.o $(CFLAGS)

