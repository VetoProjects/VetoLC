QMAKE=qmake

all: testing app

testing:
	mkdir testbin
	cd test && ${QMAKE} LiveCodingTest.pro -o ../testbin/Makefile
	cd testbin && make && rm *.o *[ch]pp

app:
	mkdir bin
	cd src && ${QMAKE} LiveCoding.pro -o ../bin/Makefile
	cd bin && make && rm *.o *[ch]pp

python2:
	mkdir testbin
	cd test && ${QMAKE} -makefile CONFIG+=with_python2 ${QMAKE}_CXXFLAGS+=-DWITH_PYTHON LiveCodingTest.pro -o ../testbin/Makefile
	cd testbin && make CPPFLAGS+=WITH_PYTHON && rm *.o *[ch]pp
	mkdir bin
	cd src && ${QMAKE} CONFIG+=with_python2 ${QMAKE}_CXXFLAGS+=-DWITH_PYTHON LiveCoding.pro -o ../bin/Makefile
	cd bin && make CPPFLAGS+=WITH_PYTHON && rm *.o *[ch]pp

python:
	mkdir testbin
	cd test && ${QMAKE} -makefile CONFIG+=with_python ${QMAKE}_CXXFLAGS+=-DWITH_PYTHON LiveCodingTest.pro -o ../testbin/Makefile
	cd testbin && make CPPFLAGS+=WITH_PYTHON && rm *.o *[ch]pp
	mkdir bin
	cd src && ${QMAKE} CONFIG+=with_python ${QMAKE}_CXXFLAGS+=-DWITH_PYTHON LiveCoding.pro -o ../bin/Makefile
	cd bin && make CPPFLAGS+=WITH_PYTHON && rm *.o *[ch]pp

clean:
	rm -r testbin || rm -r bin
