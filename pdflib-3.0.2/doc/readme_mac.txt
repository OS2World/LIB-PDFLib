Building PDFlib on the Mac
==========================

To compile PDFlib with Metrowerks CodeWarrior, open the supplied
project file PDFlib.mcp with the Metrowerks IDE. The project file
contains a target for building a static PDFlib library, and a
target for building the pdftest sample program which uses this
static PDFlib library. pdftest is a simple command-line application
which demonstrates the use of PDFlib, but doesn't make any attempt
at doing fancy Mac GUI stuff.

The Mac project file expects to find the auxiliary libraries
in directories parallel to the PDFlib directory. The library
directory names should not contain any version numbers (see
readme_win.txt which discusses more details, although the library names
differ).

Source code for the auxiliary libraries is available from the Web sites
quoted in readme.txt.

Precompiled Mac versions of the auxiliary libraries may be available
from the following Web sites:

TIFFlib: http://www.cwi.nl/~jack/macsoftware.html

Zlib: http://www.cwi.nl/~jack/macsoftware.html

libpng: http://www.cwi.nl/~jack/macsoftware.html

If you don't have or can't use any of the auxiliary libraries, edit
the Mac section in pdflib/p_config.h, and #define or #undef the relevant
entries:

HAVE_LIBZ
HAVE_LIBPNG
HAVE_LIBTIFF


Compiling the language wrappers
===============================

In order to compile the C wrappers for the supported languages you
will have to install the relevant source code package (see manual),
and adjust the include paths for these packages in the project file.
Since we supply prebuilt binaries for all supported languages this will
only rarely be required.
