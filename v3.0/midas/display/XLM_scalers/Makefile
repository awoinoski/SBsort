CC = g++
CFLAGS   = -O -Wall -fPIC  

INC = -I$(SBsort)/current/midas/state-machine\
      -I$(SBsort)/current/midas/decoder\
      -I$(SBsort)/current/SB-format	

BIN=$(SBsort)/bin/midas

SORT= sort.o\
       $(SBsort)/current/midas/state-machine/sort_but_not_assemble.a\
       $(SBsort)/current/midas/decoder/decoder.a

sort:$(SORT)
	$(CC) -o $(BIN)/display_XLM_scalers $(SORT) $(CFLAGS)  -lm -ldl

sort.o:sort.c 
	$(CC) -c -o sort.o sort.c $(CFLAGS) $(INC)

clean:
	rm -rf sort *.o *~
