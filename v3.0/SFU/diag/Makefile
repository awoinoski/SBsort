all:
	echo Making ET.
	(cd  ET && make -is clean && make && make -is clean && cd ..)
	echo DONE: ET.

	echo Making ET_class.
	(cd  ET_class && make -is clean && make && make -is clean && cd ..)
	echo DONE: ET_class.


clean:
	(cd ET            && make -is clean && cd ..)
	(cd ET            && make -is clean && cd ..)
	rm -f *~
