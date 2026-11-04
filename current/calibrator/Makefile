CC = g++
CFLAGS   = -O3 -Wall -fPIC
BIN = $(SBsort)/current/calibrator
INC = -I$(SBsort)/current/SB-format\
      -I$(SBsort)/current/utils\
      -I$(SBsort)/current/midas/decoder\
      -I$(ROOTINC)

CALIBRATOR=SB-cal.o

calibrator:$(CALIBRATOR)
	ar rcs $(BIN)/calibrator.a $(CALIBRATOR)


SB-cal.o:SB-cal.c SB-cal.h
	$(CC) -c -o SB-cal.o $(CFLAGS) SB-cal.c $(INC)

clean:
	rm -rf *.o

very-clean:
	rm -rf *.a *.o *.*~ *~
