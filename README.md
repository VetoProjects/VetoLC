Veto-LiveCoding
===============

A Live Coding Environment for AudioPython, QML and GLSL(also rudimentary Python IDE capabilities) in QT/C++.
Should be platform independent but GLSL is only working in Windows right now; in order to get AudioPython
to work, you will install that as well(naturally). No additional Python packages are required.

Version & General Infos
-----------------------

Package Version:    `0.1.0 Alpha/Test`

Packages used to develop this application(and their versions):

    Qt:     5.2/5.3 (QT 5 is necessary)
    Python: 2.7 (Python 3.x support is experimental and not really working, sorry for that)

Compilation
-----------

There is two ways to compile the project: with or without Python support. Per default, Python
is disabled. If you want to keep it that way, just run your normal `qmake`/`make`toolchain.
If you want to enable Python, you will have to provide a few extra arguments and the final
command chain will look like this

```bash
qmake -CONFIG+=with_python QMAKE_CXXFLAGS+=-DWITH_PYTHON
make CPP_FLAGS+="-DWITH_PYTHON"
```

You will notice redundance here, this is because the MOC is dumb and I do not know of any
more elegant ways to cope with that.

Anyway, this will enable Python 2.7 support for you. I do not provide any guarantees for 
Python 3.x just yet.

**Additional info:**
Please note that there are actually three ways to compile the project. There is an option
called with_python_custom where you will have to provide any additional config flags yourself.
You can try it out with, e.g.:

```bash
qmake -CONFIG+=with_python_custom QMAKE_CXXFLAGS+=-DWITH_PYTHON \
QMAKE_CXX_FLAGS+=`/usr/bin/python2.7-cnfig --clfags` \
QMAKE_LFLAGS+=`/usr/bin/python2.7-config --ldflags`
make CPP_FLAGS+=\"-DWITH_PYTHON\"
```

This toolchain will essentially do the same thing that the option ```with_python``` does on Unix.

Usage
-----

Coming soon.

Capabilities
------------

Coming soon.

Examples & Resources
--------------------

Coming soon.

Contribute
----------

Coming soon.
