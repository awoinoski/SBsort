all:
	echo Making display_XLM_scalers.
	(cd XLM_scalers r && make -is clean && make  && make -is clean && cd ..)
	echo DONE: XLM_SCALERS

clean:
	(cd XLM_scalers      && make -is clean && cd ..)
	rm -f *~
