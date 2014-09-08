Notes on the PDFlib Tcl binding:

Unix
====

- You may have to change the name of the tclsh binary in the
  example scripts if you want to run them directly. Alternatively,
  you may want to create a symbolic link pointing to the installed
  version of tclsh.

- Platforms which have a shared library file name suffix which is different
  from ".so" must change the name of the PDFlib Tcl wrapper library
  in pkgIndex.tcl.


Windows
=======

- The precompiled version of the Tcl wrapper DLL requires Tcl 8.2
  or higher because of its Unicode support.

- If you compile the wrapper from source you must change pdflib_tcl.so
  to pdflib_tcl.dll in pkgIndex.tcl. The binary distribution already
  takes care of this.
