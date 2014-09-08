PDFlib V3.02
============
Portable C library for dynamically generating PDF ("Adobe Acrobat") files,
with support for many other programming languages.

Copyright (c) 1997-2000 Thomas Merz and PDFlib GmbH

The PDFlib distribution is available from http://www.pdflib.com

THIS IS NOT PUBLIC DOMAIN OR FREEWARE SOFTWARE -- 
see end of this file for an overview of licensing and
usage issues, and the file doc/PDFlib-license.pdf for details.


Overview
========
PDFlib is a C library for generating PDF files. It offers a graphics
API with support for drawing, text, fonts, images, and hypertext. Call PDFlib
routines from within your client program and voila: dynamic PDF files!
For detailed instructions on PDFlib programming and the associated API,
see the PDFlib Programming Manual, included in PDF format in the
PDFlib distribution.


Supported Programming Environments
==================================
The PDFlib core library can be built as a static library or a shared
C library/DLL. PDFlib can be used from the following environments:

- ANSI C libary (static or dynamic)
- ANSI C++ via an object wrapper
- Perl
- Tcl
- Python
- Java via the JNI, including servlets
- ActiveX/COM (Visual Basic, Active Server Pages with VBScript and JScript,
  Windows Script Host, Delphi, and numerous other environments). The
  ActiveX edition of PDFlib is available separately.

The necessary wrappers for attaching PDFlib to these environments are
included in the distribution (except for ActiveX), as well as sample
programs for all supported environments.

Three sample applications are supplied for all supported language bindings:

- hello:	simple "hello, world!" program
- pdfclock:	generate a PDF file with an analog clock image
- image:	embed a JPEG image in the PDF output


PDF Features
============
PDFlib's many features are covered in detail in the programming manual.
So let's just take a brief look at the most important ones:

- basic drawing
  lines, rectangles, arcs, Bezier curves, ...
  gray scale or RGB color
- text
  text in different fonts and sizes, formatting
  type 1 font embedding, built-in font metrics, AFM metrics files for 
  additional fonts, additional encodings
- image file formats
  support for PNG, CCITT, TIFF, GIF, and JPEG files
- PDF output compression via Zlib
- hypertext features
  bookmarks, page transitions, Web and local links, PDF file attachments
- Unicode support for hypertext and CJK page descriptions
- CID font support for Chinese, Japanese, and Korean text


Supplied Library Clients
========================
PDFlib includes some sample client programs:

- pdftest:	general C test program, illustrates many PDF features
- pdfimage:	convert GIF/TIFF/JPEG/PNG image files to PDF
- text2pdf:	convert text files to PDF


Platform support
================
PDFlib requires an ANSI C compiler and runs on many platforms since
it doesn't make use of an platform-specific features, and doesn't
have any user interface. PDFlib is primarily developed and tested
on Linux, Windows, and MacOS, and is compiled and tested on a
variety of other platforms, including EBCDIC-based platforms.


Binary distributions
====================
PDFlib is available as source code as well as binary distribution
for certain platform/language combinations. If you download the
binary package, you save yourself the hassle of building the
library for your platform. However, you should read the
comprehensive manual (see below) in order to learn how to install
the supplied binaries, and how to use PDFlib.


Where to start?
===============
PDFlib comes with a comprehensive manual which can be found as a
PDF document in the doc directory.

Platform-specific build and install instructions can be found in
the readme_<platform>.txt files in the doc directory.


PDFlib resources
================
Information on building and using PDFlib is available at several places.
Please use these resources:

- The PDFlib manual in the doc directory contains the complete API reference,
  plus extensive information regarding the supported language bindings.

- The PDFlib FAQ collects information about known bugs, patches,
  and workarounds: http://www.pdflib.com.

- The PDFlib mailing list discusses PDFlib deployment in a variety of
  environments. You can access the mailing list archives over the Web,
  and don't need to subscribe in order to use it:
  http://www.egroups.com/group/pdflib

- Commercial PDFlib licensees are eligible for professional product
  support from PDFlib GmbH. Please send your inquiry along with your
  PDFlib license number to support@pdflib.com.


External libraries used in PDFlib
=================================
PDFlib makes use of several other libraries if they are available.
Source code for these libraries is freely available on the Internet:

- Source code for Sam Leffler's TIFFlib is available from
  http://www.libtiff.org
  Note that some implementations of the TIFF library in turn
  require the JPEG library which is available from http://www.ijg.org

- Source code for the zlib compression library is available from
  http://www.info-zip.org/pub/infozip/zlib

- Source code for the PNG image library libpng is available from
  http://www.libpng.org/pub/png
  Note that the PNG library in turn requires the Zlib library.

Platform-specific information and download locations for these libraries
can be found in doc/readme_<platform>.txt

Pre-built Windows binaries for these libraries are available from our
Web site.

If any of these libraries is missing, PDFlib will work anyway,
although (of course) it will be lacking the respective features.


Submitting Bug Reports
======================
In case of trouble you should always check the PDFlib Web site
in order to see whether your problem is already known, or a patch exists.
If not so, please observe the following:

If you have trouble building PDFlib on your system, please send the following
to support@pdflib.com:

- a description of your problem and the platform in use
- the PDFlib version number you are using
- the output of "./libtool --config" (Unix systems only)
- most welcome: suggested patches or solutions, other helpful information

If you have trouble with the behaviour of PDFlib, please send the following:

- a description of your problem
- the platform in use
- the PDFlib version number you are using
- the language binding you are using, along with relevant version numbers
- relevant code snippets for reproducing the problem, or a small PDF file
  exhibiting the problem if you can't construct a code snippet easily
- sample data files if necessary (image files, for example)


A Shameless Plug
================
My book has more information on PDFlib and PDF/Web integration.
The book is currently available in German, English, and Japanese:

Mit Acrobat ins World Wide Web
Effiziente Erstellung von PDF-Dateien und ihre Einbindung ins Web.
An updated second edition will be available in October 2000 (ISBN 3-935320-00-0)
Thomas Merz Verlag Muenchen 1998, ISBN 3-9804943-1-4.

Web Publishing with Acrobat/PDF
Springer-Verlag Heidelberg Berlin New York 1998, ISBN 3-540-63762-1

Japanese edition:
Tokyo Denki Daigaku 1999, ISBN 4-501-53020-0
See http://plaza4.mbn.or.jp/~unit


Licensing and Copyright
=======================
THIS IS NOT PUBLIC DOMAIN OR FREEWARE SOFTWARE!

PDFlib is available under two different licensing terms which are
substantially different, and meet the needs of different developer groups.
Please take the time to read the short summaries below in order to decide
which one applies to your development.


The Aladdin Free Public License
-------------------------------
This license applies to the main PDFlib package, but not to the
ActiveX edition and any EBCDIC editions (both of which are only
available under the terms of the commercial PDFlib license). The
complete text of the license agreement can be found in the
file aladdin-license.pdf. In short and non-legal terms:

- You may develop free software with PDFlib, provided you make your source
  code available.
- You may develop software for your own use with PDFlib, as long as you
  don't sell it.
- You may redistribute PDFlib non-commercially.
- You may redistribute PDFlib on digital media if the complete contents of
  the media are freely redistributable.
  
Only the text in the file doc/aladdin-license.pdf is considered to completely
describe the licensing conditions. Project managers please note: Using PDFlib
in your commercial projects is not covered by the Aladdin license, and
effectively means jeopardizing your project through unlicensed software!


The commercial PDFlib license
-----------------------------
A commercial PDFlib license is required for all uses of the software which
are not explicitly covered by the Aladdin Free Public License, for example:

- shipping a commercial product which contains PDFlib,
- distributing (free or commercial) software based on PDFlib when the source
  code is not made available,
- implementing commercial Web services with PDFlib.

Details of the commercial PDFlib license and a purchase order form
can be found in the doc directory.

Please contact us if you're interested in obtaining a commercial PDFlib license:

PDFlib GmbH
Tal 40, 80331 Muenchen, Germany
fax +49/89/29 16 46 86

Thomas Merz, the primary author of PDFlib: tm@pdflib.com

License inquiries: sales@pdflib.com

Technical inquiries if you have not licensed PDFlib:
mailing list (and archives) at http://www.egroups.com/group/pdflib

Support for PDFlib licensees: support@pdflib.com

Copyright (c) 1997-2000 PDFlib GmbH and Thomas Merz.  All rights reserved.
