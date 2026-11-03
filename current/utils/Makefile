CC = g++
CFLAGS   = -g -O3 -Wall -fPIC  $(shell $(ROOTSYS)/bin/root-config --cflags)

ROOT =  $(shell $(ROOTSYS)/bin/root-config --glibs)
BIN = $(SBsort)/current/utils
INC = -I$(SBsort)/current/utils\
      -I$(SBsort)/current/midas/decoder\
      -I$(ROOTINC)

UTILS=lin_eq_solver.o\
      waveform_analyzer.o\
      read_master.o

utils:$(UTILS)
	$ ar rcs $(BIN)/utils.a $(UTILS)

read_master.o:read_master.c read_master.h
	$(CC) -c -o read_master.o $(CFLAGS) read_master.c -I$(INC)

lin_eq_solver.o:lin_eq_solver.c lin_eq_solver.h
	$(CC) -c -o lin_eq_solver.o $(CFLAGS) lin_eq_solver.c -I$(INC)

waveform_analyzer.o:waveform_analyzer.c waveform_analyzer.h
	$(CC) -c -o waveform_analyzer.o $(CFLAGS) waveform_analyzer.c -I$(INC) $(ROOT)

clean:
	rm -rf *.o

very-clean:
	rm -rf *.a *.o *.*~ *~
