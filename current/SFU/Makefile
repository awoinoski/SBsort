all:
	echo Making CHECK.
	(cd  check && make -is clean && make && make -is clean && cd ..)
	echo DONE: CHECK.

	echo Making TIME.
	(cd  time && make -is clean && make && make -is clean && cd ..)
	echo DONE: TIME.

	echo Making HPGe.
	(cd  hpge && make -is clean && make && make -is clean && cd ..)
	echo DONE: HPGe.

	echo Making DIAG.
	(cd  diag && make -is clean && make && make -is clean && cd ..)
	echo DONE: DIAG.

	echo Making CALIBRATE.
	(cd  calibrate && make -is clean && make && make -is clean && cd ..)
	echo DONE: CALIBRATE.

clean:
	(cd check           && make -is clean && cd ..)
	(cd time            && make -is clean && cd ..)
	(cd energy          && make -is clean && cd ..)
	(cd diag            && make -is clean && cd ..)
	(cd calibrate       && make -is clean && cd ..)
	rm -f *~
