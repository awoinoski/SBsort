all:
	echo Making CSS_RAW.
	(cd  css_raw && make -is clean && make && make -is clean && cd ..)
	echo DONE: CSS_RAW.

	echo Making AUX_RAW.
	(cd  aux_raw && make -is clean && make && make -is clean && cd ..)
	echo DONE: AUX_RAW.

	echo Making Diag_Class.
	(cd  Diag_Class && make -is clean && make && make -is clean && cd ..)
	echo DONE: Diag_Class.

	echo Making COIN.
	(cd  coin && make -is clean && make && make -is clean && cd ..)
	echo DONE: COIN.

clean:
	(cd css_raw             && make -is clean && cd ..)
	(cd aux_raw             && make -is clean && cd ..)
	(cd Diag_Class          && make -is clean && cd ..)
	(cd coin                && make -is clean && cd ..)

	rm -f *~
