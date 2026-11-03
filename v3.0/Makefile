all:
	make very-clean
	echo Setting up directories.
	(mkdir ../bin && mkdir ../bin/midas && mkdir ../bin/sfu)

	echo Making SB-format.
	(cd SB-format && make -is very-clean && make && make -is clean && cd ..)
	echo DONE: SB-FORMAT.

	echo Making utils.
	(cd utils && make -is very-clean && make && make -is clean && cd ..)
	echo DONE: UTILS.

	echo Making midas.
	(cd midas && make -is very-clean && make && make -is clean && cd ..)
	echo DONE: MIDAS.

	echo Making SFU.
	(cd SFU && make -is clean && make && make -is clean && cd ..)
	echo DONE: SFU.

clean:
	echo Making clean all directories and bin.
	(cd utils      && make -is clean && cd ..)
	(cd utils      && make -is clean && cd ..)
	(cd midas      && make -is clean && cd ..)
	(cd SFU        && make -is clean && cd ..)
	rm -f *~
	rm -rf ../bin/*
	rmdir ../bin
	echo DONE: Making clean.

very-clean:
	echo Making very-clean all directories and bin.
	(cd SB-format  && make -is very-clean && cd ..)
	(cd utils      && make -is very-clean && cd ..)
	(cd midas      && make -is very-clean && cd ..)
	(cd SFU        && make -is clean && cd ..)
	rm -f *~
	rm -rf ../bin/*
	rmdir ../bin
	echo DONE: Making very-clean.
