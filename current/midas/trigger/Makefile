all:
	echo Making channelHP.
	(cd channelHP r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CHANNELHP.

	echo Making timeStampDiff
	(cd timeStampDiff r && make -is clean && make && make -is clean && cd ..)
	echo DONE: TIMESTAMPDIFF.
clean:
	(cd channelHP && make -is clean && cd ..)
	(cd timeStampDiff && make -is clean && cd ..)

	rm -f *~
