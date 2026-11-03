all:
	echo Making RawEvent.
	(cd  RawEvent && make -is clean && make && make -is clean && cd ..)
	echo DONE: RAWEVENT.

	echo Making Triggers.
	(cd  Triggers && make -is clean && make && make -is clean && cd ..)
	echo DONE: TRIGGERS.

	echo Making Rate.
	(cd Rate r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CHECK_RATE.

	echo Making check_TrigBits.
	(cd TrigBits r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CHECK_TRIGBITS.

	echo Making check_EventTimeDifference.
	(cd EventTimeDifference r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CHECK_EVENTTIMEDIFFERENCE.

	echo Making check_TimeStamps.
	(cd TimeStamps r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CHECK_TIMESTAMPS.

	echo Making check_NIMTr.
	(cd NIMTr r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CHECK_NIMTr.

	echo Making check_Diag_Class.
	(cd Diag_Class r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CHECK_DIAG_CLASS.

	echo Making check_Trig_DTgate.
	(cd Trig_DTgate r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CHECK_TRIG_DTGATE.

	echo Making RawEvent_tdiff.
	(cd RawEvent_tdiff r && make -is clean && make && make -is clean && cd ..)
	echo DONE: CHECK_RAWEVENT_TDIFF.

clean:
	(cd RawEvent            && make -is clean && cd ..)
	(cd Triggers            && make -is clean && cd ..)
	(cd Rate                && make -is clean && cd ..)
	(cd TrigBits            && make -is clean && cd ..)
	(cd EventTimeDifference && make -is clean && cd ..)
	(cd TimeStamps          && make -is clean && cd ..)
	(cd NIMTr               && make -is clean && cd ..)
	(cd Diag_Class          && make -is clean && cd ..)
	(cd Trig_DTgate         && make -is clean && cd ..)
	(cd RawEvent_tdiff      && make -is clean && cd ..)
	rm -f *~
