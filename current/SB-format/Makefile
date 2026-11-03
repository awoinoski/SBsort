CC = g++
CFLAGS   = -O3 -Wall -fPIC
BIN = $(SBsort)/current/SB-format
INC = -I$(SBsort)/current/utils\
      -I$(SBsort)/current/midas/decoder\
      -I$(ROOTINC)

SB-FORMAT=SB-format.o SB-decoder.o SB-encoder.o

SB-format:$(SB-FORMAT)
	$ ar rcs $(BIN)/SB-format.a $(SB-FORMAT)

SB-format.o:SB-format.c SB-format.h SB-common.h
	$(CC) -c -o SB-format.o $(CFLAGS) SB-format.c $(INC)

SB-decoder.o:SB-decoder.c SB-decoder.h
	$(CC) -c -o SB-decoder.o $(CFLAGS) $(INC) SB-decoder.c

SB-encoder.o:SB-encoder.c SB-encoder.h
	$(CC) -c -o SB-encoder.o $(CFLAGS) $(INC) SB-encoder.c

clean:
	rm -rf *.o

very-clean:
	rm -rf *.a *.o *.*~ *~ sort
