all:
	echo Making wfit_T0Fit.
	(cd T0Fit r && make -is clean && make  && make -is clean && cd ..) 
	echo DONE: WFIT_T0FIT.

	echo Making wfit_show_NIMFit.
	(cd NIMFit r && make -is clean && make  && make -is clean && cd ..) 
	echo DONE: WFIT_NIMFIT.

	echo Making wfit_show_T0dist.
	(cd T0dist r && make -is clean && make  && make -is clean && cd ..) 
	echo DONE: WFIT_T0DIST.

	echo Making wfit_show_NIMT0dist.
	(cd NIMT0dist r && make -is clean && make  && make -is clean && cd ..) 
	echo DONE: WFIT_SHOW_NIMT0DIST.
clean:
	(cd T0Fit				&& make -is clean && cd ..)
	(cd NIMFit				&& make -is clean && cd ..)
	(cd T0dist  				&& make -is clean && cd ..)
	(cd NIMT0dist  				&& make -is clean && cd ..)
	rm -rf *~
