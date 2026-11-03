all:
	echo Making decoder.
	(cd decoder && make -is very-clean && make && make -is clean && cd ..)
	echo DONE: DECODER.

	echo Making state-machine.
	(cd state-machine && make -is very-clean && make && make -is clean && cd ..)
	echo DONE: STATE-MACHINE.

	echo Making map.
	(cd map && make -is very-clean && make && make -is clean && cd ..)
	echo DONE: MAP.

	echo Making block_dump.
	(cd block_dump && make -is clean && make && make -is clean && cd ..)
	echo DONE: BLOCK_DUMP.

	echo Making midas2sfu.
	(cd midas2sfu && make -is clean && make && make -is clean && cd ..)
	echo DONE: MIDAS2SFU.

	echo Making project.
	(cd project && make -is clean && make && make -is clean && cd ..)
	echo DONE: PROJECT.

	echo Making display.
	(cd display && make -is clean && make && make -is clean && cd ..)
	echo DONE: DISPLAY.

	echo Making view.
	(cd view && make -is clean && make && make -is clean && cd ..)
	echo DONE: VIEW.

	echo Making trigger.
	(cd trigger && make -is clean && make && make -is clean && cd ..)
	echo DONE: TRIGGER.

	echo Making wfit.
	(cd wfit && make -is clean && make && make -is clean && cd ..)
	echo DONE: WFIT.

clean:
	(cd decoder       && make -is clean && cd ..)
	(cd state-machine && make -is clean && cd ..)
	(cd map           && make -is clean && cd ..)
	(cd block_dump    && make -is clean && cd ..)
	(cd midas2sfu     && make -is clean && cd ..)
	(cd project       && make -is clean && cd ..)
	(cd trigger && make -is clean && cd ..)
	(cd display && make -is clean && cd ..)
	(cd view && make -is clean && cd ..)
	(cd wfit          && make -is clean && cd ..)
	rm -f *~

very-clean:
	(cd decoder       && make -is very-clean && cd ..)
	(cd state-machine && make -is very-clean && cd ..)
	(cd map           && make -is very-clean && cd ..)
	(cd block_dump    && make -is clean && cd ..)
	(cd midas2sfu     && make -is clean && cd ..)
	(cd project       && make -is clean && cd ..)
	(cd trigger && make -is clean && cd ..)	
	(cd display && make -is clean && cd ..)
	(cd view && make -is clean && cd ..)
	(cd wfit          && make -is clean && cd ..)
	rm -f *~
