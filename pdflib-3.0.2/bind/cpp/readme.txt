Notes on the PDFlib C++ binding:

Unix
====

The PDFlib configure script may fail if your system has a C++ compiler
but there is some problem with the setup or the standard C++ library. You
may have to change the name of the standard C++ library, or adjust some
setting (e.g. the LD_LIBRARY_PATH environment variable) if the C++ binding
doesn't work on your system.
