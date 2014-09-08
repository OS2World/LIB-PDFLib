Building PDFlib with MS Visual C++
==================================

To compile PDFlib with MS Visual C++, open the supplied workspace
file PDFlib.dsw which contains several projects for the core library, test
programs, and language bindings. Set "test" to be the active project,
select the configuration called "no auxiliary libs", and build. By default,
a static library pdflib.lib will be built.

If you want to build a Windows DLL, the PDFLIB_EXPORTS #define must be set
when building the library. This has already been done in the pdflib_dll
project.  Clients must define PDFLIB_DLL before including pdflib.h in order
to use the DLL.

There are two PDFlib projects: "pdflib" builds a static library (pdflib.lib),
and "pdflib_dll" builds a dynamic library (pdflib.dll). If you experiment
with both the static and the DLL version of PDFlib, make sure to delete
all object files ("Build, "Clean") before switching to the other kind of
library since we do not attempt to keep the objects files separately.


Building PDFlib with Borland C++ Builder
========================================

Project files for Borland C++ builder 5 are supplied with the PDFlib
distribution.  After launching the IDE with the supplied project group
file PDFlib.bpg create the directory pdflib/Release (for the static
library project, PDFlib.lib), or pdflib/Release_DLL (for the DLL project,
PDFlib.dll). (If you know how to have the Borland IDE create these
directories automatically please let us know!).

In order to build PDFlib with other compilers, observe the above
notes and make sure to define the preprocessor symbol WIN32.


Building PDFlib with Metrowerks CodeWarrior
===========================================

The PDFlib distribution contains a project file for CodeWarrior 5 which
can be used on both Mac and Windows. The project file contains targets
for building static and DLL versions of PDFlib, as well as the pdftest
client which uses the static PDFlib library.

The CodeWarrior project file is set up to not use any auxiliary library.
If you want these, you must configure them according to the description
below.


Auxiliary libraries
===================

By default, the project files are set up for a stand-alone
PDFlib without the auxiliary libraries TIFFlib, zlib, and libpng. In order
to use these libraries with PDFlib do the following (the last two steps are
already implemented in the MSVC "Release" configuration):

- Compile and install the libraries and respective header files in directories
  parallel to the PDFlib directory (no version numbers in the directory names
  of the auxiliary libraries):

  |
  +-- pdflib-#.##\pdflib.h
  |              \ ...
  |
  +-- zlib\zlib.lib
  |       \zlib.h
  |       \zconf.h
  |
  +-- libtiff\libtiff.lib
  |          \tiff.h
  |          \tiffio.h
  |
  +-- libpng\libpng.lib
            \png.h
	    \pngconf.h

  Source code for the auxiliary libraries is available from the Web sites
  quoted in readme.txt.

  Precompiled Win32 versions of the auxiliary libraries for use with MSVC
  (and the corresponding include files) can be obtained from our Web site
  at http://www.pdflib.com.

- Add one or more of the predefined names HAVE_LIBZ, HAVE_LIBPNG, HAVE_LIBTIFF
  to the project options (or p_config.h if you use CodeWarrior).

- Add the names of the libraries (libtiff.lib, libpng.lib, zlib.lib) to the list
  of libraries in the project options.


Compiling the language wrappers
===============================

In order to compile the C wrappers for the supported languages you
will have to install the relevant source code package (see manual),
and adjust the include paths for these packages in the project files.
Since we supply prebuilt binaries for all supported languages this will
only rarely be required.
