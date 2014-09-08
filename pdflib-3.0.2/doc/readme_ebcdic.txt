Building PDFlib on EBCDIC platforms
===================================

PDFlib can be used on EBCDIC-based midrange and mainframe platforms
such as IBM AS/400 and S/390 (with the "Unix system services for OS/390").
PDF generation on EBCDIC systems is a delicate issue since PDF is based
on ASCII operators, but nevertheless we want to be able to process EBCDIC
text strings. However, PDFlib is carefully set up to meet these requirements.


Compiling PDFlib
================

PDFlib internally distinguishes between items which must be converted
as text, and items which must be handled in binary mode.

PDFlib's EBCDIC support is built on the following assumptions:

- The package is extracted and all source files converted to EBCDIC.
  On the AS/400 the EBCDIC codepage (CCSID) 819 is assumed, on OS/390 
  CCSID 1047.  Other systems must manually select the required codepage
  by #define'ing #PDF_CODEPAGE in p_config.h appropriately. 

- ASCII support libraries (such as IBM's libascii) are not required,
  and must not be used.

- The C source code strings must not translated automatically back to
  ISO 8859-1 by some compiler directive or similar means (for example,
  setting the __STRING_CODE_SET__ #define), but must be processed as
  EBCDIC text.

- The identifier "PDFLIB_EBCDIC" is #define'd via a test in the PDFlib
  source code (in p_config.h).

- Using the PDFlib auxiliary libraries on EBCDIC systems:
  - libpng and zlib are believed to be EBCDIC-safe.
  - TIFFlib may also be EBCDIC-safe, but has not been tested on EBCDIC
    systems.


Building PDFlib on the AS/400
=============================

The following steps have been reported to result in successful PDFlib
build and deployment on the AS/400:

- Create a library and source files:

  CRTLIB LIB(LIBPDF)
  CHGCURLIB LIB(LIBPDF) 
  CRTSRCPF FILE(H) RCDLEN(240) /* For Header files */ 
  CRTSRCPF FILE(QCSRC) RCDLEN(240) /* For C files */

- Unpack the PDFlib source distribution and transfer to the AS/400
  using FTP. The files must be converted to EBCDIC during the transfer.
  The FTP commands will look similar to the following:

  put pdflib.h H.pdflib 
  put p_afm.c QCSRC.p_afm
  ...

- Create PDFlib modules:

  CHGCURLIB LIB(LIBPDF) 
  CRTCMOD MODULE(P_AFM) DEFINE(AS400 PDFLIB_EBCDIC) SYSIFCOPT(*IFSIO) 
  CRTCMOD MODULE(P_ANNOTS) DEFINE(AS400 PDFLIB_EBCDIC) SYSIFCOPT(*IFSIO)
  ...

- Create the PDFlib service program. First, create a AS/400 export language
  source for each module:

  CHGCURLIB LIB(LIBPDF) 
  CRTSRCPF FILE(PDFMAKE) 
  RTVBNDSRC MODULE(P_AFM) SRCFILE(PDFMAKE) SRCMBR(MODEXPORT) MBROPT(*REPLACE) 
  RTVBNDSRC MODULE(P_ANNOTS) SRCFILE(PDFMAKE) SRCMBR(MODEXPORT) MBROPT(*ADD) 
  RTVBNDSRC MODULE(P_BASIC) SRCFILE(PDFMAKE) SRCMBR(MODEXPORT) MBROPT(*ADD) 
  RTVBNDSRC MODULE(P_CCITT) SRCFILE(PDFMAKE) SRCMBR(MODEXPORT) MBROPT(*ADD)
  ...

  Make a copy of the export language in file PDFMAKE, member MODEXPORT to
  member SRVEXPORT. Edit the text in member SRVEXPORT to remove the export,
  STRPGMEXP, and ENDPGMEXP statements that are not required.

  The final step is to build the service program, using the following AS400
  commands: 

  CHGCURLIB LIB(LIBPDF) 
  CRTSRVPGM SRVPGM(PDFLIB) MODULE(*CURLIB/P_*) +
      EXPORT(*SRCFILE) SRCFILE(PDFMAKE) SRCMBR(SRVEXPORT) +
      TEXT('PdfLib 3.01 Service program')

- To do a test run using the supplied pdftest.c demo edit the prefix entry
  in fonts/pdflib.upr, and upload the contents of the fonts directory to
  the AS/400. Also upload the image test files from the test directory.
  Then create the test program as follows: 

  CHGCURLIB LIBPDF 
  CRTCMOD MODULE(PDFTEST) DEFINE(AS400 PDFLIB_EBCDIC) SYSIFCOPT(*IFSIO) 

  CRTPGM PGM(PDFTEST) MODULE(PDFTEST) BNDSRVPGM(*LIBL/PDFLIB) + 
     TEXT('PDFLIB 3.01 Test Program')

  Then run the test program: CALL PGM(TESTPDF)

Note: we will try to make available more resources for using PDFlib
on the AS/400 -- please check our Web site for updates.


Using PDFlib
============

As with the source code compilation, special care must be taken when
using PDFlib in an EBCDIC environment since some items must be handled
as text (resulting in internal conversion), while others must be handled
as binary data.

PDFlib expects the following items in EBCDIC text format if compiled
in EBCDIC mode:

- PFA font files, UPR configuration files, AFM font metrics files
- document information (EBCDIC or Unicode, see reference manual)
- string parameters to PDFlib functions
- input and output file names
- environment variables (if supported at all)

- PDFlib error messages are also generated in EBCDIC format

In contrast, the following items must be treated in binary mode:
- PDF output files
- PFB font files and PFM font metrics files
- JPEG, GIF, TIFF, PNG, CCITT image files


MVS notes
=========

On MVS you must rename the AFM metrics files, change the name of pdflib.upr
to PDFLBUPR, and change all entries in pdflib.upr accordingly.

Specify the MVS PDFLIB.UPR path as follows:

PDF_set_parameter(p, "resourcefile", "'LRS164.VPS.R80.CNTL(PDFLBUPR)'");
PDF_set_parameter(p,"prefix","'LRS164.VPS.R80.CNTL");
