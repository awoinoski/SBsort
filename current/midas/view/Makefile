all:
	echo Making view_fragment.
	(cd fragment r && make -is clean && make && make -is clean && cd ..)
	echo DONE: VIEW_FRAGMENT.

	echo Making view_channel.
	(cd channel r && make -is clean && make && make -is clean && cd ..)
	echo DONE: VIEW_CHANNEL.

	echo Making view_trigger.
	(cd trigger r && make -is clean && make && make -is clean && cd ..)
	echo DONE: VIEW_TRIGGER.

	echo Making view_diagnostics.
	(cd diagnostics r && make -is clean && make && make -is clean && cd ..)
	echo DONE: VIEW_DIAGNOSTICS.

clean:
	(cd fragment             && make -is clean && cd ..)
	(cd channel              && make -is clean && cd ..)
	(cd trigger              && make -is clean && cd ..)
	(cd diagnostics          && make -is clean && cd ..)
	rm -f *~
