all:
	echo Making MDGG_O1_O2.
	(cd  MDGG_O1_O2 && make -is clean && make && make -is clean && cd ..)
	echo DONE: MDGG_O1_O2.

	echo Making MDGG_O1_O3.
	(cd  MDGG_O1_O3 && make -is clean && make && make -is clean && cd ..)
	echo DONE: MDGG_O1_O3.

	echo Making MDGG_O1_O4.
	(cd  MDGG_O1_O4 && make -is clean && make && make -is clean && cd ..)
	echo DONE: MDGG_O1_O4.

	echo Making MDGG_O2_O3.
	(cd  MDGG_O2_O3 && make -is clean && make && make -is clean && cd ..)
	echo DONE: MDGG_O2_O3.

	echo Making MDGG_O2_O4.
	(cd  MDGG_O2_O4 && make -is clean && make && make -is clean && cd ..)
	echo DONE: MDGG_O2_O4.

	echo Making MDGG_O3_O4.
	(cd  MDGG_O3_O4 && make -is clean && make && make -is clean && cd ..)
	echo DONE: MDGG_O3_O4.

	echo Making pos_t0_01.
	(cd  pos_t0_O1 && make -is clean && make && make -is clean && cd ..)
	echo DONE: pos_t0_O1.

	echo Making t_diff_css_aux.
	(cd  t_diff_css_aux && make -is clean && make && make -is clean && cd ..)
	echo DONE: t_diff_css_aux.


clean:
	(cd MDGG_O1_O2           && make -is clean && cd ..)
	(cd MDGG_O1_O3           && make -is clean && cd ..)
	(cd MDGG_O1_O4           && make -is clean && cd ..)
	(cd MDGG_O2_O3           && make -is clean && cd ..)
	(cd MDGG_O2_O4           && make -is clean && cd ..)
	(cd MDGG_O3_O4           && make -is clean && cd ..)
	(cd pos_t0_O1            && make -is clean && cd ..)
	(cd t_diff_css_aux       && make -is clean && cd ..)
	rm -f *~
