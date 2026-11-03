CC = g++
CFLAGS   = -O -Wall -fPIC 

INC = -I$(SBsort)/current/midas/decoder

BIN=$(SBsort)/bin/midas

SORT= sort.o\
       $(SBsort)/current/midas/decoder/decoder.a

sort:$(SORT)
	$(CC) -o $(BIN)/midas_block_dump $(SORT) $(CFLAGS) $(ROOT) -lm -ldl

sort.o:sort.c 
	$(CC) -c -o sort.o sort.c $(CFLAGS) $(INC)

clean:
	rm -rf sort *.o *~
