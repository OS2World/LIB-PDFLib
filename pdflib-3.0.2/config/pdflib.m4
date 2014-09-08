# Locate installed versions of PDFlib, and check the version numbers
# of both PDFlib header file and library.
#
# Thomas Merz 2000-03-14

dnl This file can be used for constructing autoconf-generated configure
dnl scripts. It implements the AM_PATH_PDFLIB macro.
dnl
dnl The first two parameters are the required PDFlib major and minor version
dnl numbers; minor must have two decimal places.
dnl
dnl AM_PATH_PDFLIB([MINIMUM-MAJOR, MINIMUM-MINOR
dnl	[ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl
dnl This macro tests for PDFlib, and defines PDFLIB_CFLAGS and PDFLIB_LIBS
dnl containing PDFlib compiler and linker flags. The macro makes use of
dnl the C compiler. Therefore the AC_PROG_CC should be called before.
dnl
dnl Note: it is assumed that PDFlib has been installed in some "proper"
dnl place. No attempt is made to find the library in obscure places.
dnl
dnl The macro requires the script pdflib-config. It must be located in
dnl the user's path, or can be supplied in the environment variable
dnl PDFLIB_CONFIG.
dnl
dnl PDFLIB_LIBS contains auxiliary libraries which will be required in order
dnl to successfully link a program against PDFlib. Typical PDFLIB_LIBS
dnl output looks similar to the following:
dnl
dnl -lpdf -ltiff -ljpeg -lpng -lz -lm
dnl
dnl PDFLIB_CFLAGS is not strictly required for compiling PDFlib-based
dnl programs since PDFlib client code doesn't rely on the auxiliary
dnl libraries (their headers are only required for PDFlib itself).
dnl It may, however, be useful for clients to know which PDFlib features
dnl are supported by the local PDFlib build. Typical PDFLIB_CFLAGS output
dnl looks similar to the following:
dnl
dnl -DHAVE_LIBTIFF -DHAVE_LIBZ -DHAVE_LIBPNG
dnl
dnl Finally, this macro calls AC_SUBST for PDFLIB_LIBS and PDFLIB_CFLAGS

AC_DEFUN(AM_PATH_PDFLIB,
[
AC_ARG_ENABLE(pdflibtest,
[  --disable-pdflibtest            completely skip PDFlib test],
    enable_pdflibtest=no, enable_pdflibtest=yes)

  if test "$enable_pdflibtest" = "yes" ; then
      AC_PATH_PROG(PDFLIB_CONFIG, pdflib-config, no)
  fi

  min_pdflib_major=$1
  min_pdflib_minor=$2

  AC_MSG_CHECKING(for PDFlib $min_pdflib_major.$min_pdflib_minor or later)

  no_pdflib=""

  if test "$PDFLIB_CONFIG" = "no" -o "$enable_pdflibtest" = "no" ; then
    no_pdflib=yes
  else
    PDFLIB_CFLAGS=`$PDFLIB_CONFIG --cflags`
    PDFLIB_LIBS=`$PDFLIB_CONFIG --libs`
    pdflib_config_major_version=`$PDFLIB_CONFIG --majorversion`
    pdflib_config_minor_version=`$PDFLIB_CONFIG --minorversion`

    if test "x$enable_pdflibtest" = "xyes" ; then
      ac_save_CFLAGS="$CFLAGS"
      ac_save_LIBS="$LIBS"
      CFLAGS="$CFLAGS $PDFLIB_CFLAGS"
      LIBS="$PDFLIB_LIBS $LIBS"

    # Now check if the installed PDFlib is sufficiently new. (Also sanity
    # checks the results of pdflib-config to some extent)

      rm -f conf.pdflibtest

      AC_TRY_RUN([
#include <pdflib.h>
#include <stdio.h>
#include <stdlib.h>

int 
main ()
{
  int libmajor, libminor;

  system ("touch conf.pdflibtest");

  if ((PDFLIB_MAJORVERSION != $pdflib_config_major_version) ||
      (PDFLIB_MINORVERSION != $pdflib_config_minor_version)) {
      printf("\n*** pdflib-config returned %d.%02d, but a PDFlib %d.%02d header\n", 
             $pdflib_config_major_version, $pdflib_config_minor_version,
             PDFLIB_MAJORVERSION, PDFLIB_MINORVERSION);
      printf ("*** was found! If pdflib-config was correct, then it is best\n");
      printf ("*** to remove the old PDFlib header.\n");

      printf("*** If pdflib-config was wrong, set the environment variable PDFLIB_CONFIG\n");
      printf("*** to point to the correct copy of pdflib-config, and remove the file config.cache\n");
      printf("*** before re-running configure\n");

    } else {

      libmajor = PDF_get_majorversion();
      libminor = PDF_get_minorversion();

      if (libmajor != PDFLIB_MAJORVERSION || libminor != PDFLIB_MINORVERSION) {
	  printf("*** PDFlib header files (version %d.%d) do not match\n",
		 PDFLIB_MAJORVERSION, PDFLIB_MINORVERSION);
	  printf("*** library (version %d.%d)\n", libmajor, libminor);

      } else {

	  if (libmajor > $min_pdflib_major ||
	    (libmajor == $min_pdflib_major && libminor >= $min_pdflib_minor)) {
		return 0;

	   } else {

		printf("\n*** An old version of PDFlib (%d.%d) was found.\n",
		       libmajor, libminor);
		printf("*** You need PDFlib version %d.%d or newer. The latest version of\n",
		       $min_pdflib_major, $min_pdflib_minor);
		printf("*** PDFlib is always available from http://www.pdflib.com.\n");
		printf("***\n");
		printf("*** If you have already installed a sufficiently new version, this error\n");
		printf("*** probably means that the wrong copy of the pdflib-config shell script is\n");
		printf("*** being found. The easiest way to fix this is to remove the old version\n");
		printf("*** of PDFlib, but you can also set the PDFLIB_CONFIG environment to point\n");
		printf("*** to the correct copy of pdflib-config. (In this case, you will have to\n");
		printf("*** modify your LD_LIBRARY_PATH enviroment variable, or edit /etc/ld.so.conf\n");
		printf("*** so that the correct libraries are found at run-time))\n");
	  }
      }
    }
  return 1;
}
],, no_pdflib=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
       CFLAGS="$ac_save_CFLAGS"
       LIBS="$ac_save_LIBS"
     fi
  fi
  if test "x$no_pdflib" = x ; then
     AC_MSG_RESULT(yes)
     ifelse([$3], , :, [$3])
  else
     if test "$enable_pdflibtest" = "no" ; then
	 AC_MSG_RESULT(disabled)
     else
     AC_MSG_RESULT(no)
     if test "$PDFLIB_CONFIG" = "no" ; then
       echo "*** The pdflib-config script installed by PDFlib couldn't be."
       echo "*** found. If PDFlib was installed in PREFIX, make sure PREFIX"
       echo "*** is in your path, or set the PDFLIB_CONFIG environment"
       echo "*** variable to the full path to pdflib-config."
     else
       if test -f conf.pdflibtest ; then
        :
       else
          echo "*** Could not run PDFlib test program, checking why..."
          CFLAGS="$CFLAGS $PDFLIB_CFLAGS"
          LIBS="$LIBS $PDFLIB_LIBS"
          AC_TRY_LINK([
#include <pdflib.h>
],      [ return (PDFLIB_MAJORVERSION || PDFLIB_MINORVERSION); ],
        [ echo "*** The test program compiled, but did not run. This usually means"
          echo "*** that the run-time linker is not finding PDFlib or finding the wrong"
          echo "*** version of PDFlib. If it is not finding PDFlib, you'll need to set your"
          echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
          echo "*** to the installed location  Also, make sure you have run ldconfig if that"
          echo "*** is required on your system."
	  echo "***"
          echo "*** If you have an old version installed, it is best to remove it, although"
          echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH"
          echo "***" ],
        [ echo "*** The test program failed to compile or link. See the file config.log for the"
          echo "*** exact error that occured. This usually means PDFlib was incorrectly installed"
          echo "*** or that you have moved PDFlib since it was installed. In the latter case, you"
          echo "*** may want to edit the pdflib-config script: $PDFLIB_CONFIG" ])
          CFLAGS="$ac_save_CFLAGS"
          LIBS="$ac_save_LIBS"
       fi
     fi
     fi
     PDFLIB_CFLAGS=""
     PDFLIB_LIBS=""
     ifelse([$3], , :, [$3])
  fi
  AC_SUBST(PDFLIB_CFLAGS)
  AC_SUBST(PDFLIB_LIBS)
  rm -f conf.pdflibtest
])
