all:
	echo Making CHECK.
	(cd  check && make -is clean && make && make -is clean && cd ..)
	echo DONE: CHECK.

	echo Making TIME.
	(cd  time && make -is clean && make && make -is clean && cd ..)
	echo DONE: TIME.

	echo Making ENERGY.
	(cd  energy && make -is clean && make && make -is clean && cd ..)
	echo DONE: ENERGY.

	echo Making DIAG.
	(cd  diag && make -is clean && make && make -is clean && cd ..)
	echo DONE: DIAG.

clean:
	(cd check           && make -is clean && cd ..)
	(cd time            && make -is clean && cd ..)
	(cd energy          && make -is clean && cd ..)
	(cd diag            && make -is clean && cd ..)
	rm -f *~
