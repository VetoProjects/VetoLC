QMAKE=qmake
PREFIX=/usr/bin/
BUILDDIR=bin/
TESTDIR=testbin/

TARGET=VetoLC
TESTTARGET=VetoLCTest
SOURCES=$(wildcard src/*.c)
TESTS=$(filter-out src/main.c, $(wildcard test/*.c src/*c))

# Makes everything
all: testing app

# makes the tests
testing:
	mkdir -p $(TESTDIR) $(BUILDDIR)
	cd test && ${QMAKE} LiveCodingTest.pro -o ../$(TESTDIR)Makefile
	cd $(TESTDIR) && make && rm *.o *[ch]pp

# Makes the app
app:
	mkdir -p $(TESTDIR) $(BUILDDIR)
	cd src && ${QMAKE} LiveCoding.pro -o ../$(BUILDDIR)Makefile
	cd $(BUILDDIR) && make && rm *.o *[ch]pp

# Makes the app with Python2.7
python2:
	mkdir -p $(TESTDIR) $(BUILDDIR)
	cd test && ${QMAKE} -makefile CONFIG+=with_python2 ${QMAKE}_CXXFLAGS+=-DWITH_PYTHON LiveCodingTest.pro -o ../$(TESTDIR)Makefile
	cd $(TESTDIR) && make CPPFLAGS+=WITH_PYTHON && rm *.o *[ch]pp
	cd src && ${QMAKE} CONFIG+=with_python2 ${QMAKE}_CXXFLAGS+=-DWITH_PYTHON LiveCoding.pro -o ../$(BUILDDIR)Makefile
	cd $(BUILDDIR) && make CPPFLAGS+=WITH_PYTHON && rm *.o *[ch]pp

# Makes the app with the default Python version
python:
	mkdir -p $(TESTDIR) $(BUILDDIR)
	cd test && ${QMAKE} -makefile CONFIG+=with_python ${QMAKE}_CXXFLAGS+=-DWITH_PYTHON LiveCodingTest.pro -o ../$(TESTDIR)Makefile
	cd $(TESTDIR) && make CPPFLAGS+=WITH_PYTHON && rm *.o *[ch]pp
	cd src && ${QMAKE} CONFIG+=with_python ${QMAKE}_CXXFLAGS+=-DWITH_PYTHON LiveCoding.pro -o ../$(BUILDDIR)Makefile
	cd $(BUILDDIR) && make CPPFLAGS+=WITH_PYTHON && rm *.o *[ch]pp

# Removes everything previously built
clean:
	rm -r $(TESTDIR) $(BUILDDIR)

# Installs the app
install:
	install -d $(PREFIX)$(TARGET)
	install $(BUILDDIR)$(TARGET) $(PREFIX)$(TARGET)

# Uninstalls the app
uninstall:
	rm -rf $(PREFIX)$(TARGET)
