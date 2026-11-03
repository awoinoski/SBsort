CC = g++
CFLAGS   = -O -Wall -fPIC  $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOT =  $(shell $(ROOTSYS)/bin/root-config --glibs)
INC = -I$(ROOTINC)\
      -I$(SBsort)/current/SB-format\
      -I$(SBsort)/current/midas/decoder\
      -I$(SBsort)/current/midas/state-machine
BIN=$(SBsort)/bin/midas
SORT= sort.o\
       $(SBsort)/current/midas/state-machine/sort_but_not_assemble.a\
       $(SBsort)/current/midas/decoder/decoder.a
sort:$(SORT)
	$(CC) -o $(BIN)/view_fragment $(SORT) $(CFLAGS) $(ROOT) -lm -ldl

sort.o:sort.c 
	$(CC) -c -o sort.o sort.c $(CFLAGS) $(INC)

clean:
	rm -rf sort *.o *~
