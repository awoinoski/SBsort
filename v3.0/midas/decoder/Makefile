CC = g++
CFLAGS   = -O3 -Wall -fPIC 

BIN = $(SBsort)/current/midas/decoder
INC = -I$(SBsort)/current/SB-format
DECODER=midas-format.o xlm-format.o tig-format.o

decoder:$(DECODER)
	$ ar rcs $(BIN)/decoder.a $(DECODER)

midas-format.o:midas-format.c midas-format.h
	$(CC) -c -o midas-format.o $(CFLAGS)  midas-format.c

xlm-format.o:xlm-format.c xlm-format.h
	$(CC) -c -o xlm-format.o $(CFLAGS) $(INC) xlm-format.c

tig-format.o:tig-format.c tig-format.h
	$(CC) -c -o tig-format.o $(CFLAGS) tig-format.c

clean:
	rm -rf *.o

very-clean:
	rm -rf *.a *.o *.*~ *~ sort
