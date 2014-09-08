Building PDFlib on Unix systems
===============================

Auxiliary libraries
-------------------

PDFlib can be set up to use the TIFFlib, libpng, and zlib auxiliary
libraries.

The configure script will automatically find out which libraries
are available. Note that certain builds of the TIFF library also need
the IJG JPEG library. The configure script doesn't use the JPEG library
when looking for the TIFF library. If this is required on your system,
use the configure option --with-tiffauxlib (see below).

If you want to use one of the auxiliary libraries but haven't installed
them yet, you must do so before running PDFlib's configure script. If
you install some auxiliary library after having run PDFlib's configure
script, you will have to remove the files config.* since otherwise
configure will use the cached old results, and will not detect the newly
installed library.

In summary, you have the following options regarding the TIFF, PNG, and Zlib
libraries:

- Have the shared libs installed in some default place where
  your system and the loader can find them (usually /lib, /usr/lib, etc).
  Static libs won't necessarily help since not all systems can merge static
  and shared libraries, and the PDFlib language wrappers (except C/C++)
  require shared libraries.

- Set LD_LIBRARY_PATH or a similar environment variable (read libtool's
  informational output when doing "make install"). However, this won't
  work in all cases. For example, when using PDFlib on the Web server you
  can't easily set environment variables. In these cases you must build
  and use PDFlib without the respective library support.

Again: it may not be possible to successfully use uninstalled shared versions
or static versions of the auxiliary libraries with PDFlib.

Source code for the auxiliary libraries is available from the Web sites
quoted in readme.txt.


Building PDFlib
---------------

To start the build process on Unix, type

unix> ./configure
unix> make
unix> make test

Several options can used with the configure script in order to
override some default configuration options, or to assist configure
in finding some software locations on your machine. Type

unix> ./configure --help

before the make command in order to see a list of available configure
options.

IMPORTANT: make sure to use only absolute path names for all custom
directories. Also, wildcards should not be used. This requirement is
necessary because otherwise the paths won't work as include paths with
compiler calls.

If you want to use PDFlib on another machine, do not simply copy
the PDFlib source tree over. Instead, copy the distribution fileset
and re-run the configure script. Otherwise, compiler, shared library
and installation settings could erroneously be taken from the first
machine instead of from the actual one.

Note that in very rare cases it may be necessary to rename the generated
library file in order to avoid name clashes when installing it. This
can be achieved with configure's --with-libname option. Currently this
problem is known to exist on Digital Unix 4.


configure troubleshooting
-------------------------

The configure script helps to keep PDFlib portable across a wide variety
of systems, and to keep track of many different configurations and
the availability of features. Generally the script does a good job.
Given the huge number of different systems, configure may occasionally
fail in one of several ways:

- failing to detect installed software

- failing to complete all tests due to errors during script execution

In the first case, you can help configure by finding out the necessary
paths etc. yourself, and supplying any required --with-... option on
the configure command line.

In the second case, you either also can try to supply --with-... options
in order to prevent the failing test from being called, or abandon
the feature if you don't need it by supplying the value "no" to the
respective configure option, e.g., --with-tcl=no.

If you can determine the cause of a failing configure script, we will
be happy to hear from you. Please supply your system details, the
feature/option in question, and a workaround or improvement if possible.

The configure script may  fail under Cygwin when trying to probe for languages.
It's safer to disable language probing using --with-perl=no etc.


Forcing a certain compiler or compiler flags
--------------------------------------------

You can set several environment variables before running the configure
script. These variables will be used in the generated Makefiles. The more
important ones are:

CC       The C compiler to use
CXX      The C++ compiler to use
CFLAGS   C compiler flags

For example, the following has been reported to work well on Solaris
(in a csh environment):

setenv CC /opt/SUNWspro/SC4.2/bin/cc
setenv CXX /opt/SUNWspro/SC4.2/bin/CC
setenv CFLAGS '-fast -xO3 -xstrconst -xtarget=generic'
./configure


Testing and installing the library
----------------------------------

Optionally, to run sample PDFlib applications in several programming
environments (including the scripting languages which have been
detected by configure), type:

unix> make test

In order to install the library and the support files for all detected
scripting languages, type

# make install

Note that installing will require root privileges on most machines.

If you want to install only selected parts (e.g., only the PDFlib
C library or the Perl support), type "make install" in the
respective subdirectory.


Shared library support
----------------------

Language bindings other than C/C++ require shared library support for
PDFlib to work. By default, the PDFlib core library will be built as a
static library, while the Java, Perl, Tcl, and Python wrapper libraries
must be built as shared libraries (*.so on most systems, possibly with
a version number suffix, see below). When these language wrapper libraries
are built, the PDFlib core routines will be pulled in by the linker.

C or C++ language clients must deploy libtool for using PDFlib, or at least
emulate the calls issued by libtool when binding a program. These can
easily be determined by going to the bind/c or bind/cpp directory,
and watching libtool doing its work when building the supplied demo
programs.

C or C++ language clients which prefer a shared PDFlib library over
a static one can request this via a configure option:

./configure --with-shared-pdflib

This option, however, will disable all language bindings other than C/C++.

PDFlib relies on GNU libtool for shared library support. libtool
shadows the object files and libraries with a layer of .lo and .la
files. The actual library files are placed in a subdirectory called
".libs". The PDFlib Makefiles and libtool will take care of correctly
building, testing, and installing these libraries. If anything goes
wrong on your system, read the manual section on shared libraries,
take a look at the contents of the .libs subdirectory, and observe
what the supplied Makefiles do for compiling, linking, testing, and
installing.


Using PDFlib in other packages
------------------------------

In order to find out details about PDFlib's version, configuration,
and use, the pdflib-config shell script can be used. It is built during
the configure run, and returns all information you'll need for PDFlib
deployment. Running the script without any options lists the supported
command line options.

Locating installed PDFlib versions is even more convenient when the
supplied config/pdflib.m4 macro definition is used. This file is
useful when implementing a configure machinery using the autoconf and
aclocal tools. Although a short description can be found in the file
pdflib.m4 you will have to be familiar with the above-mentioned tools
in order to use these macros.


Library version numbers
-----------------------

Libtool-generated libraries such as PDFlib number their interfaces
with integer interface numbers (no subversions!). In addition to the
interface number, a revision number can be used. A particular library
supports a range of interface numbers, where the range can have a length
of one or more. In particular, libtool defines the following:

CURRENT   The most recent interface number that this library implements.
REVISION  The implementation number of the CURRENT interface.
AGE       The length of the range of supported interfaces (i.e., CURRENT numbers).

The following table relates PDFlib version numbers to the C:R:A library
versioning scheme which you may see in the names of PDFlib shared library files:

PDFlib    C:R:A   comments
-------------------------------------------------------------------------------
3.00      0:0:0   first release based on libtool
3.01      0:1:0   maintenance release; should have increased C since
                  undocumented functions were removed
3.02      1:0:0   cleans up the non-incrementing glitch in 3.01

When the PDFlib core is built as a static library (this is the default), version
numbers will not be visible. However, since language bindings other than
C and C++ are always built as shared libraries, they will have version
numbers visible.

Many thanks to Evgeny Stambulchik for leading me on the right track
with respect to libtool and library versioning schemes!
