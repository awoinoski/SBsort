CC = g++
CFLAGS   = -O3 -Wall -fPIC $(shell $(ROOTSYS)/bin/root-config --cflags) -lm
ROOT =  $(shell $(ROOTSYS)/bin/root-config --glibs)
INC=-I../../utils/
OBJ=../../utils/utils.a
all:sort

sort: sort.c functions.o functions.h
	$(CC) -o sort $(CFLAGS) sort.c functions.o $(INC) $(OBJ) 
	mv sort ../../../bin/sfu/calibrator

functions.o:functions.c functions.h
	$(CC) $(CFLAGS) -c -o functions.o functions.c $(INC)  

clean:
	rm -rf *.o

very-clean:
	rm -rf *.o *.*~ *~  
