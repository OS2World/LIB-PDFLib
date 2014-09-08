/*---------------------------------------------------------------------------*
 |        PDFlib - A library for dynamically generating PDF documents        |
 +---------------------------------------------------------------------------+
 |        Copyright (c) 1997-2000 Thomas Merz. All rights reserved.          |
 +---------------------------------------------------------------------------+
 |    This software is NOT in the public domain.  It can be used under two   |
 |    substantially different licensing terms:                               |
 |                                                                           |
 |    The commercial license is available for a fee, and allows you to       |
 |    - ship a commercial product based on PDFlib                            |
 |    - implement commercial Web services with PDFlib                        |
 |    - distribute (free or commercial) software when the source code is     |
 |      not made available                                                   |
 |    Details can be found in the file PDFlib-license.pdf.                   |
 |                                                                           |
 |    The "Aladdin Free Public License" doesn't require any license fee,     |
 |    and allows you to                                                      |
 |    - develop and distribute PDFlib-based software for which the complete  |
 |      source code is made available                                        |
 |    - redistribute PDFlib non-commercially under certain conditions        |
 |    - redistribute PDFlib on digital media for a fee if the complete       |
 |      contents of the media are freely redistributable                     |
 |    Details can be found in the file aladdin-license.pdf.                  |
 |                                                                           |
 |    These conditions extend to ports to other programming languages.       |
 |    PDFlib is distributed with no warranty of any kind. Commercial users,  |
 |    however, will receive warranty and support statements in writing.      |
 *---------------------------------------------------------------------------*/

/* pdflib_pl.c
 *
 * Wrapper code for the PDFlib Perl binding
 *
 */

/*
 * The PERL_OBJECT #define is only required for ActivePerl < 5.6
 * 
 * IMPORTANT: in this case the code must be compiled in C++ mode!
 */
#if defined(WIN32) && \
	defined(PERL_REVISION) && defined(PERL_VERSION) && \
	PERL_REVISION == 5 && PERL_VERSION < 6
#define PERL_OBJECT
#endif

#ifdef __cplusplus
#include <math.h>
#include <stdlib.h>
extern "C" {
#endif /* __cplusplus */

#include <EXTERN.h>
#include <perl.h>
#include <XSUB.h>
#undef free
#undef malloc
#include <string.h>

#ifdef __cplusplus
}
#endif /* __cplusplus */

/* Definitions for compiling Perl extensions on a variety of machines */

#if defined(WIN32) || defined(__WIN32__)
#   if defined(_MSC_VER)
#	define SWIGEXPORT(a,b) __declspec(dllexport) a b
#   else
#	if defined(__BORLANDC__)
#	    define SWIGEXPORT(a,b) a _export b
#	else
#	    define SWIGEXPORT(a,b) a b
#	endif
#   endif
#else
#   define SWIGEXPORT(a,b) a b
#endif

/* #defines for older Perl versions < 5.005_50 */
#if !defined(PERL_REVISION) || \
	((PERL_REVISION >= 5) && \
	((PERL_VERSION<5) || ((PERL_VERSION==5) && (PERL_SUBVERSION<50))))
#ifndef PL_sv_yes
#define PL_sv_yes sv_yes
#endif
#ifndef PL_na
#define PL_na na
#endif
#endif

#ifdef PERL_OBJECT
#define MAGIC_PPERL  CPerlObj *pPerl = (CPerlObj *) this;
#define MAGIC_CAST   (int (CPerlObj::*)(SV *, MAGIC *))
#define SWIGCLASS_STATIC 
#else
#define MAGIC_PPERL
#define MAGIC_CAST
#define SWIGCLASS_STATIC static
#endif

#include <stdlib.h>

#ifdef SWIG_GLOBAL
#ifdef __cplusplus
#define SWIGSTATIC extern "C"
#else
#define SWIGSTATIC
#endif
#endif

#ifndef SWIGSTATIC
#define SWIGSTATIC static
#endif

typedef struct SwigPtrType {
  char               *name;
  int                 len;
  void               *(*cast)(void *);
  struct SwigPtrType *next;
} SwigPtrType;

/* Pointer cache structure */
typedef struct {
  int                 stat;               /* Status (valid) bit             */
  SwigPtrType        *tp;                 /* Pointer to type structure      */
  char                name[256];          /* Given datatype name            */
  char                mapped[256];        /* Equivalent name                */
} SwigCacheType;

static int SwigPtrMax  = 64;	/* Max entries that can be currently held */
static int SwigPtrN    = 0;	/* Current number of entries              */
static int SwigPtrSort = 0;	/* Status flag indicating sort            */
static SwigPtrType *SwigPtrTable = 0;  /* Table containing pointer equivalences  */
static int SwigStart[256];	/* Table containing starting positions    */

/* Cached values */
#define SWIG_CACHESIZE  8
#define SWIG_CACHEMASK  0x7
static SwigCacheType SwigCache[SWIG_CACHESIZE];  
static int SwigCacheIndex = 0;
static int SwigLastCache = 0;

/* Sort comparison function */
static int swigsort(const void *data1, const void *data2) {
	SwigPtrType *d1 = (SwigPtrType *) data1;
	SwigPtrType *d2 = (SwigPtrType *) data2;
	return strcmp(d1->name,d2->name);
}

/* Binary Search function */
static int swigcmp(const void *key, const void *data) {
  char *k = (char *) key;
  SwigPtrType *d = (SwigPtrType *) data;
  return strncmp(k,d->name,d->len);
}

/* Register a new datatype with the type-checker */

#ifndef PERL_OBJECT
SWIGSTATIC 
void SWIG_RegisterMapping(char *origtype, char *newtype, void *(*cast)(void *)) {
#else
SWIGSTATIC
#define SWIG_RegisterMapping(a,b,c) _SWIG_RegisterMapping(pPerl, a,b,c)
void _SWIG_RegisterMapping(CPerlObj *pPerl, char *origtype, char *newtype, void *(*cast)(void *)) {
#endif

  int i;
  SwigPtrType *t = 0, *t1;

  if (!SwigPtrTable) {     
    SwigPtrTable = (SwigPtrType *) malloc(SwigPtrMax*sizeof(SwigPtrType));
    SwigPtrN = 0;
  }
  if (SwigPtrN >= SwigPtrMax) {
    SwigPtrMax = 2*SwigPtrMax;
    SwigPtrTable = (SwigPtrType *) realloc(SwigPtrTable,SwigPtrMax*sizeof(SwigPtrType));
  }
  for (i = 0; i < SwigPtrN; i++)
    if (strcmp(SwigPtrTable[i].name,origtype) == 0) {
      t = &SwigPtrTable[i];
      break;
    }
  if (!t) {
    t = &SwigPtrTable[SwigPtrN];
    t->name = origtype;
    t->len = strlen(t->name);
    t->cast = 0;
    t->next = 0;
    SwigPtrN++;
  }
  while (t->next) {
    if (strcmp(t->name,newtype) == 0) {
      if (cast) t->cast = cast;
      return;
    }
    t = t->next;
  }
  t1 = (SwigPtrType *) malloc(sizeof(SwigPtrType));
  t1->name = newtype;
  t1->len = strlen(t1->name);
  t1->cast = cast;
  t1->next = 0;
  t->next = t1;
  SwigPtrSort = 0;
}

/* Function for getting a pointer value */

#ifndef PERL_OBJECT
SWIGSTATIC 
char *SWIG_GetPtr(SV *sv, void **ptr, char *_t)
#else
SWIGSTATIC
#define SWIG_GetPtr(a,b,c) _SWIG_GetPtr(pPerl,a,b,c)
char *_SWIG_GetPtr(CPerlObj *pPerl, SV *sv, void **ptr, char *_t)
#endif
{
  char temp_type[256];
  char *name,*_c;
  int  len,i,start,end;
  IV   tmp;
  SwigPtrType *sp,*tp;
  SwigCacheType *cache;

  /* If magical, apply more magic */

  if (SvGMAGICAL(sv))
    mg_get(sv);

  /* Check to see if this is an object */
  if (sv_isobject(sv)) {
    SV *tsv = (SV*) SvRV(sv);
    if ((SvTYPE(tsv) == SVt_PVHV)) {
      MAGIC *mg;
      if (SvMAGICAL(tsv)) {
	mg = mg_find(tsv,'P');
	if (mg) {
	  SV *rsv = mg->mg_obj;
	  if (sv_isobject(rsv)) {
	    tmp = SvIV((SV*)SvRV(rsv));
	  }
	}
      } else {
	return "Not a valid pointer value";
      }
    } else {
      tmp = SvIV((SV*)SvRV(sv));
    }
    if (!_t) {
      *(ptr) = (void *) tmp;
      return (char *) 0;
    }
  } else if (! SvOK(sv)) {            /* Check for undef */
    *(ptr) = (void *) 0;
    return (char *) 0;
  } else if (SvTYPE(sv) == SVt_RV) {       /* Check for NULL pointer */
    *(ptr) = (void *) 0;
    if (!SvROK(sv)) 
      return (char *) 0;
    else
      return "Not a valid pointer value";
  } else {                                 /* Don't know what it is */
      *(ptr) = (void *) 0;
      return "Not a valid pointer value";
  }
  if (_t) {
    /* Now see if the types match */      

    if (!sv_isa(sv,_t)) {
      _c = HvNAME(SvSTASH(SvRV(sv)));
      if (!SwigPtrSort) {
	qsort((void *) SwigPtrTable, SwigPtrN, sizeof(SwigPtrType), swigsort);  
	for (i = 0; i < 256; i++) {
	  SwigStart[i] = SwigPtrN;
	}
	for (i = SwigPtrN-1; i >= 0; i--) {
	  SwigStart[(int) SwigPtrTable[i].name[0]] = i;
	}
	for (i = 255; i >= 1; i--) {
	  if (SwigStart[i-1] > SwigStart[i])
	    SwigStart[i-1] = SwigStart[i];
	}
	SwigPtrSort = 1;
	for (i = 0; i < SWIG_CACHESIZE; i++)  
	  SwigCache[i].stat = 0;
      }
      /* First check cache for matches.  Uses last cache value as starting point */
      cache = &SwigCache[SwigLastCache];
      for (i = 0; i < SWIG_CACHESIZE; i++) {
	if (cache->stat) {
	  if (strcmp(_t,cache->name) == 0) {
	    if (strcmp(_c,cache->mapped) == 0) {
	      cache->stat++;
	      *ptr = (void *) tmp;
	      if (cache->tp->cast) *ptr = (*(cache->tp->cast))(*ptr);
	      return (char *) 0;
	    }
	  }
	}
	SwigLastCache = (SwigLastCache+1) & SWIG_CACHEMASK;
	if (!SwigLastCache) cache = SwigCache;
	else cache++;
      }

      start = SwigStart[(int) _t[0]];
      end = SwigStart[_t[0]+1];
      sp = &SwigPtrTable[start];
      while (start < end) {
	if (swigcmp(_t,sp) == 0) break;
	sp++;
	start++;
      }
      if (start >= end) sp = 0;
      if (sp) {
	while (swigcmp(_t,sp) == 0) {
	  name = sp->name;
	  len = sp->len;
	  tp = sp->next;
	  while(tp) {
	    if (tp->len >= 255) {
	      return _c;
	    }
	    strcpy(temp_type,tp->name);
	    strncat(temp_type,_t+len,255-tp->len);
	    if (sv_isa(sv,temp_type)) {
	      /* Get pointer value */
	      *ptr = (void *) tmp;
	      if (tp->cast) *ptr = (*(tp->cast))(*ptr);

	      strcpy(SwigCache[SwigCacheIndex].mapped,_c);
	      strcpy(SwigCache[SwigCacheIndex].name,_t);
	      SwigCache[SwigCacheIndex].stat = 1;
	      SwigCache[SwigCacheIndex].tp = tp;
	      SwigCacheIndex = SwigCacheIndex & SWIG_CACHEMASK;
	      return (char *) 0;
	    }
	    tp = tp->next;
	  } 
	  /* Hmmm. Didn't find it this time */
 	  sp++;
	}
      }
      /* Didn't find any sort of match for this data.  
	 Get the pointer value and return the received type */
      *ptr = (void *) tmp;
      return _c;
    } else {
      /* Found a match on the first try.  Return pointer value */
      *ptr = (void *) tmp;
      return (char *) 0;
    }
  } 
  *ptr = (void *) tmp;
  return (char *) 0;
}

#ifdef __cplusplus
extern "C"
#endif
#ifndef PERL_OBJECT
/* If we don't use the XS macro it doesn't work with all Perl configurations */
SWIGEXPORT(/* */, XS(boot_pdflib_pl));
#else
SWIGEXPORT(void,boot_pdflib_pl)(CV *cv, CPerlObj *pPerl);
#endif

#include <setjmp.h>

/* The following sick stuff is an attempt to make recent Perl versions
 * work with recent SWIG versions compiled with recent MS VC++ versions.
 */
#ifdef _MSC_VER
    #undef setjmp
    #define setjmp _setjmp
    #undef longjmp
#endif

#include "pdflib.h"

/* exception handling is currently not thread-safe! */
#define SWIG_exception(msg)   	croak(msg)
static jmp_buf			exception_buffer;
static int			exception_status;
static char			error_message[256];

/* Exception handling */
#define try		if ((exception_status = setjmp(exception_buffer)) == 0)
#define catch(error)	else if (exception_status == error)
#define throw(error)	longjmp(exception_buffer, error)

/* PDFlib error handler */

static void
pdf_swig_errorhandler(PDF *p, int type, const char* shortmsg)
{
    sprintf(error_message, "PDFlib: %s\n", shortmsg);

    /* Issue a warning message and continue for non-fatal errors */
    if (type == PDF_NonfatalError) {
	fprintf(stderr, error_message);
	return;
    }
	
    /* Clean up PDFlib internals and delete p in all other cases */
    if (p != NULL)
	PDF_delete(p);

    /* ...and throw an exception */
    throw(type);
}

/* export the PDFlib routines to the shared library */
#ifdef __MWERKS__
#pragma export on
#endif

XS(_wrap_PDF_new) {
    PDF * _result;
    int argvi = 0;
    dXSARGS ;
    char versionbuf[32];

    if (items != 0)
        croak("Usage: PDF_new();");

    try {
	_result = (PDF*)PDF_new2(pdf_swig_errorhandler, NULL, NULL, NULL, NULL);

	if (_result) {
#if defined(PERL_REVISION) && defined(PERL_VERSION) && defined(PERL_SUBVERSION)
	    sprintf(versionbuf, "Perl %d.%d.%d",
	    	PERL_REVISION, PERL_VERSION, PERL_SUBVERSION);
#elif defined(PRODUCT_BUILD_NUMBER)
	    sprintf(versionbuf, "Active Perl build %s", PRODUCT_BUILD_NUMBER);
#else
	    sprintf(versionbuf, "Perl (unknown)");
#endif
	    PDF_set_parameter(_result, "binding", versionbuf);
	}
    }
    else {
	SWIG_exception(error_message);
    }
    ST(argvi) = sv_newmortal();
    sv_setref_pv(ST(argvi++),"PDFPtr", (void *) _result);
    XSRETURN(argvi);
}

XS(_wrap_PDF_delete) {
    PDF * _arg0;
    dXSARGS ;

    if (items != 1)
        croak("Usage: PDF_delete(p);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_delete. Expected PDFPtr.");
        XSRETURN(1);
    }

    try {     PDF_delete(_arg0);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_open_file) {
    int  _result;
    PDF * _arg0;
    char * _arg1;
    int argvi = 0;
    dXSARGS ;

    if (items != 2)
        croak("Usage: PDF_open_file(p,filename);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_open_file. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (char *) SvPV(ST(1),PL_na);

    try {     _result = (int )PDF_open_file(_arg0,_arg1);
    }
    else {
	SWIG_exception(error_message);
    }
    ST(argvi) = sv_newmortal();
    sv_setiv(ST(argvi++),(IV) _result);
    XSRETURN(argvi);
}

XS(_wrap_PDF_close) {
    PDF * _arg0;
    dXSARGS ;

    if (items != 1)
        croak("Usage: PDF_close(p);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_close. Expected PDFPtr.");
        XSRETURN(1);
    }

    try {     PDF_close(_arg0);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_get_buffer) {
    const char * _result;
    PDF * _arg0;
    long  size;
    dXSARGS ;

    if (items != 1) 
        croak("Usage: PDF_get_buffer(p);");

    if (SWIG_GetPtr(ST(0),(void **) &_arg0, "PDFPtr")) {
        croak("Type error in argument 1 of PDF_get_buffer. Expected PDFPtr.");
        XSRETURN(1);
    }

    try {     _result = (char *)PDF_get_buffer(_arg0, &size);
    }
    else {
	SWIG_exception(error_message);
    }

    ST(0) = sv_newmortal();
    sv_setpvn((SV*)ST(0), (char *) _result, size);
    XSRETURN(1);
}

XS(_wrap_PDF_begin_page) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    dXSARGS ;

    if (items != 3)
        croak("Usage: PDF_begin_page(p,width,height);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_begin_page. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));
    _arg2 = (float ) SvNV(ST(2));

    try {     PDF_begin_page(_arg0,_arg1,_arg2);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_end_page) {
    PDF * _arg0;
    dXSARGS ;

    if (items != 1)
        croak("Usage: PDF_end_page(p);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_end_page. Expected PDFPtr.");
        XSRETURN(1);
    }

    try {     PDF_end_page(_arg0);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_set_parameter) {
    PDF * _arg0;
    char * _arg1;
    char * _arg2;
    dXSARGS ;

    if (items != 3)
        croak("Usage: PDF_set_parameter(p,key,value);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_set_parameter. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (char *) SvPV(ST(1),PL_na);
    _arg2 = (char *) SvPV(ST(2),PL_na);

    try {     PDF_set_parameter(_arg0,_arg1,_arg2);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_get_parameter) {
    char * _result;
    PDF * _arg0;
    char * _arg1;
    float  _arg2;
    int argvi = 0;
    dXSARGS ;

    if (items != 3)
        croak("Usage: PDF_get_parameter(p,key,mod);");

    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_get_parameter. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (char *) SvPV(ST(1),PL_na);
    _arg2 = (float ) SvNV(ST(2));

    try {     _result = (char *)PDF_get_parameter(_arg0,_arg1,_arg2);
    }
    else {
	SWIG_exception(error_message);
    }

    ST(argvi) = sv_newmortal();
    sv_setpv((SV*)ST(argvi++),(char *) _result);
    XSRETURN(argvi);
}

XS(_wrap_PDF_set_value) {
    PDF * _arg0;
    char * _arg1;
    float  _arg2;
    int argvi = 0;
    dXSARGS ;

    if (items != 3)
        croak("Usage: PDF_set_value(p,key,value);");

    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_set_value. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (char *) SvPV(ST(1),PL_na);
    _arg2 = (float ) SvNV(ST(2));

    try {     PDF_set_value(_arg0,_arg1,_arg2);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(argvi);
}

XS(_wrap_PDF_get_value) {
    float  _result;
    PDF * _arg0;
    char * _arg1;
    float  _arg2;
    int argvi = 0;
    dXSARGS ;

    if (items != 3)
        croak("Usage: PDF_get_value(p,key,mod);");

    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_get_value. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (char *) SvPV(ST(1),PL_na);
    _arg2 = (float ) SvNV(ST(2));

    try {     _result = (float )PDF_get_value(_arg0,_arg1,_arg2);
    }
    else {
	SWIG_exception(error_message);
    }

    ST(argvi) = sv_newmortal();
    sv_setnv(ST(argvi++), (double) _result);
    XSRETURN(argvi);
}

XS(_wrap_PDF_findfont) {
    int  _result;
    PDF * _arg0;
    char * _arg1;
    char * _arg2;
    int  _arg3;
    int argvi = 0;
    dXSARGS ;

    if (items != 4)
        croak("Usage: PDF_findfont(p,fontname,encoding,embed);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_findfont. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (char *) SvPV(ST(1),PL_na);
    _arg2 = (char *) SvPV(ST(2),PL_na);
    _arg3 = (int )SvIV(ST(3));

    try {     _result = (int )PDF_findfont(_arg0,_arg1,_arg2,_arg3);
    }
    else {
	SWIG_exception(error_message);
    }
    ST(argvi) = sv_newmortal();
    sv_setiv(ST(argvi++),(IV) _result);
    XSRETURN(argvi);
}

XS(_wrap_PDF_setfont) {
    PDF * _arg0;
    int  _arg1;
    float  _arg2;
    dXSARGS ;

    if (items != 3)
        croak("Usage: PDF_setfont(p,font,fontsize);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_setfont. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (int )SvIV(ST(1));
    _arg2 = (float ) SvNV(ST(2));

    try {     PDF_setfont(_arg0,_arg1,_arg2);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_show) {
    PDF * _arg0;
    char * _arg1;
    dXSARGS ;
    size_t len;

    if (items != 2)
        croak("Usage: PDF_show(p,text);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_show. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (char *) SvPV(ST(1), len);

    try {     PDF_show2(_arg0,_arg1, (int) len);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_show_xy) {
    PDF * _arg0;
    char * _arg1;
    float  _arg2;
    float  _arg3;
    size_t len;
    dXSARGS ;

    if (items != 4)
        croak("Usage: PDF_show_xy(p,text,x,y);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_show_xy. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (char *) SvPV(ST(1), len);
    _arg2 = (float ) SvNV(ST(2));
    _arg3 = (float ) SvNV(ST(3));

    try {     PDF_show_xy2(_arg0,_arg1, (int) len, _arg2,_arg3);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_continue_text) {
    PDF * _arg0;
    char * _arg1;
    size_t len;
    dXSARGS ;

    if (items != 2)
        croak("Usage: PDF_continue_text(p,text);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_continue_text. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (char *) SvPV(ST(1), len);

    try {     PDF_continue_text2(_arg0,_arg1, (int) len);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_show_boxed) {
    int  _result;
    PDF * _arg0;
    char * _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    float  _arg5;
    char * _arg6;
    char * _arg7;
    int argvi = 0;
    dXSARGS ;

    if (items != 8)
        croak("Usage: PDF_show_boxed(p,text,left,top,width,height,hmode,reserved);");

    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_show_boxed. Expected PDFPtr.");
        XSRETURN(1);
    }

    _arg1 = (char *) SvPV(ST(1),PL_na);
    _arg2 = (float ) SvNV(ST(2));
    _arg3 = (float ) SvNV(ST(3));
    _arg4 = (float ) SvNV(ST(4));
    _arg5 = (float ) SvNV(ST(5));
    _arg6 = (char *) SvPV(ST(6),PL_na);
    _arg7 = (char *) SvPV(ST(7),PL_na);

    try {     _result = (int )PDF_show_boxed(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7);
 }
    else {
	SWIG_exception(error_message);
    }
    ST(argvi) = sv_newmortal();
    sv_setiv(ST(argvi++),(IV) _result);
    XSRETURN(argvi);
}

XS(_wrap_PDF_set_text_pos) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    dXSARGS ;

    if (items != 3)
        croak("Usage: PDF_set_text_pos(p,x,y);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_set_text_pos. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));
    _arg2 = (float ) SvNV(ST(2));

    try {     PDF_set_text_pos(_arg0,_arg1,_arg2);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_set_text_matrix) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    float  _arg5;
    float  _arg6;
    dXSARGS ;

    if (items != 7)
        croak("Usage: PDF_set_text_matrix(p,a,b,c,d,e,f);");

    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_set_text_matrix. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));
    _arg2 = (float ) SvNV(ST(2));
    _arg3 = (float ) SvNV(ST(3));
    _arg4 = (float ) SvNV(ST(4));
    _arg5 = (float ) SvNV(ST(5));
    _arg6 = (float ) SvNV(ST(6));

    try {     PDF_set_text_matrix(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_stringwidth) {
    float  _result;
    PDF * _arg0;
    char * _arg1;
    int  _arg2;
    float  _arg3;
    int argvi = 0;
    size_t len;
    dXSARGS ;

    if (items != 4)
        croak("Usage: PDF_stringwidth(p,text,font,size);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_stringwidth. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (char *) SvPV(ST(1), len);
    _arg2 = (int )SvIV(ST(2));
    _arg3 = (float ) SvNV(ST(3));

    try {     _result = (float )PDF_stringwidth2(_arg0,_arg1, (int) len, _arg2,_arg3);
    }
    else {
	SWIG_exception(error_message);
    }
    ST(argvi) = sv_newmortal();
    sv_setnv(ST(argvi++), (double) _result);
    XSRETURN(argvi);
}

XS(_wrap_PDF_setdash) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    dXSARGS ;

    if (items != 3)
        croak("Usage: PDF_setdash(p,b,w);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_setdash. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));
    _arg2 = (float ) SvNV(ST(2));

    try {     PDF_setdash(_arg0,_arg1,_arg2);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_setpolydash) {
    PDF *p;
    SV *val;
    AV *av;
    float carray[MAX_DASH_LENGTH];
    int  length;
    int i;
    dXSARGS ;

    if (items != 2)
        croak("Usage: PDF_setpolydash(p, arrayref);");

    if (SWIG_GetPtr(ST(0),(void **) &p, "PDFPtr")) {
        croak("Type error in argument 1 of PDF_setpolydash. Expected PDFPtr.");
        XSRETURN(1);
    }

    if (!SvROK(ST(1))) {
        croak("Type error in argument 2 of PDF_setpolydash. Expected reference to array.");
        XSRETURN(1);
    }

    av = (AV *) SvRV(ST(1));
    length = (int) av_len(av) + 1;

    if (length > MAX_DASH_LENGTH)
	length = MAX_DASH_LENGTH;

    for (i = 0; i < length; i++) {
	val = *av_fetch(av, i, 0);
	if ((!SvNOK(val)) && (!SvIOK(val))) {
	    croak("expected a reference to a float array in PDF_setpolydash\n");
	}
	carray[i] = SvNV(val);
    }

    try {     PDF_setpolydash(p, carray, length);
    }
    else {
	SWIG_exception(error_message);
    }

    XSRETURN(0);
}

XS(_wrap_PDF_setflat) {
    PDF * _arg0;
    float  _arg1;
    dXSARGS ;

    if (items != 2)
        croak("Usage: PDF_setflat(p,flatness);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_setflat. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));

    try {     PDF_setflat(_arg0,_arg1);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_setlinejoin) {
    PDF * _arg0;
    int  _arg1;
    dXSARGS ;

    if (items != 2)
        croak("Usage: PDF_setlinejoin(p,linejoin);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_setlinejoin. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (int )SvIV(ST(1));

    try {     PDF_setlinejoin(_arg0,_arg1);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_setlinecap) {
    PDF * _arg0;
    int  _arg1;
    dXSARGS ;

    if (items != 2)
        croak("Usage: PDF_setlinecap(p,linecap);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_setlinecap. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (int )SvIV(ST(1));

    try {     PDF_setlinecap(_arg0,_arg1);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_setmiterlimit) {
    PDF * _arg0;
    float  _arg1;
    dXSARGS ;

    if (items != 2)
        croak("Usage: PDF_setmiterlimit(p,miter);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_setmiterlimit. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));

    try {     PDF_setmiterlimit(_arg0,_arg1);
}
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_setlinewidth) {
    PDF * _arg0;
    float  _arg1;
    dXSARGS ;

    if (items != 2)
        croak("Usage: PDF_setlinewidth(p,width);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_setlinewidth. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));

    try {     PDF_setlinewidth(_arg0,_arg1);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_save) {
    PDF * _arg0;
    dXSARGS ;

    if (items != 1)
        croak("Usage: PDF_save(p);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_save. Expected PDFPtr.");
        XSRETURN(1);
    }

    try {     PDF_save(_arg0);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_restore) {
    PDF * _arg0;
    dXSARGS ;

    if (items != 1)
        croak("Usage: PDF_restore(p);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_restore. Expected PDFPtr.");
        XSRETURN(1);
    }

    try {     PDF_restore(_arg0);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_translate) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    dXSARGS ;

    if (items != 3)
        croak("Usage: PDF_translate(p,tx,ty);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_translate. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));
    _arg2 = (float ) SvNV(ST(2));

    try {     PDF_translate(_arg0,_arg1,_arg2);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_scale) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    dXSARGS ;

    if (items != 3)
        croak("Usage: PDF_scale(p,sx,sy);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_scale. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));
    _arg2 = (float ) SvNV(ST(2));

    try {     PDF_scale(_arg0,_arg1,_arg2);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_rotate) {
    PDF * _arg0;
    float  _arg1;
    dXSARGS ;

    if (items != 2)
        croak("Usage: PDF_rotate(p,phi);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_rotate. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));

    try {     PDF_rotate(_arg0,_arg1);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_skew) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    dXSARGS ;

    if (items != 3)
        croak("Usage: PDF_skew(p,alpha,beta);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_skew. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));
    _arg2 = (float ) SvNV(ST(2));

    try {     PDF_skew(_arg0,_arg1,_arg2);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_concat) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    float  _arg5;
    float  _arg6;
    dXSARGS ;

    if (items != 7)
        croak("Usage: PDF_concat(p,a,b,c,d,e,f);");

    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_concat. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));
    _arg2 = (float ) SvNV(ST(2));
    _arg3 = (float ) SvNV(ST(3));
    _arg4 = (float ) SvNV(ST(4));
    _arg5 = (float ) SvNV(ST(5));
    _arg6 = (float ) SvNV(ST(6));

    try {     PDF_concat(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_moveto) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    dXSARGS ;

    if (items != 3)
        croak("Usage: PDF_moveto(p,x,y);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_moveto. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));
    _arg2 = (float ) SvNV(ST(2));

    try {     PDF_moveto(_arg0,_arg1,_arg2);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_lineto) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    dXSARGS ;

    if (items != 3)
        croak("Usage: PDF_lineto(p,x,y);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_lineto. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));
    _arg2 = (float ) SvNV(ST(2));

    try {     PDF_lineto(_arg0,_arg1,_arg2);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_curveto) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    float  _arg5;
    float  _arg6;
    dXSARGS ;

    if (items != 7)
        croak("Usage: PDF_curveto(p,x1,y1,x2,y2,x3,y3);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_curveto. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));
    _arg2 = (float ) SvNV(ST(2));
    _arg3 = (float ) SvNV(ST(3));
    _arg4 = (float ) SvNV(ST(4));
    _arg5 = (float ) SvNV(ST(5));
    _arg6 = (float ) SvNV(ST(6));

    try {     PDF_curveto(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_circle) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    dXSARGS ;

    if (items != 4)
        croak("Usage: PDF_circle(p,x,y,r);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_circle. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));
    _arg2 = (float ) SvNV(ST(2));
    _arg3 = (float ) SvNV(ST(3));

    try {     PDF_circle(_arg0,_arg1,_arg2,_arg3);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_arc) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    float  _arg5;
    dXSARGS ;

    if (items != 6)
        croak("Usage: PDF_arc(p,x,y,r,alpha1,alpha2);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_arc. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));
    _arg2 = (float ) SvNV(ST(2));
    _arg3 = (float ) SvNV(ST(3));
    _arg4 = (float ) SvNV(ST(4));
    _arg5 = (float ) SvNV(ST(5));

    try {     PDF_arc(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_rect) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    dXSARGS ;

    if (items != 5)
        croak("Usage: PDF_rect(p,x,y,width,height);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_rect. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));
    _arg2 = (float ) SvNV(ST(2));
    _arg3 = (float ) SvNV(ST(3));
    _arg4 = (float ) SvNV(ST(4));

    try {     PDF_rect(_arg0,_arg1,_arg2,_arg3,_arg4);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_closepath) {
    PDF * _arg0;
    dXSARGS ;

    if (items != 1)
        croak("Usage: PDF_closepath(p);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_closepath. Expected PDFPtr.");
        XSRETURN(1);
    }

    try {     PDF_closepath(_arg0);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_stroke) {
    PDF * _arg0;
    dXSARGS ;

    if (items != 1)
        croak("Usage: PDF_stroke(p);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_stroke. Expected PDFPtr.");
        XSRETURN(1);
    }

    try {     PDF_stroke(_arg0);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_closepath_stroke) {
    PDF * _arg0;
    dXSARGS ;

    if (items != 1)
        croak("Usage: PDF_closepath_stroke(p);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_closepath_stroke. Expected PDFPtr.");
        XSRETURN(1);
    }

    try {     PDF_closepath_stroke(_arg0);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_fill) {
    PDF * _arg0;
    dXSARGS ;

    if (items != 1)
        croak("Usage: PDF_fill(p);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_fill. Expected PDFPtr.");
        XSRETURN(1);
    }

    try {     PDF_fill(_arg0);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_fill_stroke) {
    PDF * _arg0;
    dXSARGS ;

    if (items != 1)
        croak("Usage: PDF_fill_stroke(p);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_fill_stroke. Expected PDFPtr.");
        XSRETURN(1);
    }

    try {     PDF_fill_stroke(_arg0);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_closepath_fill_stroke) {
    PDF * _arg0;
    dXSARGS ;

    if (items != 1)
        croak("Usage: PDF_closepath_fill_stroke(p);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_closepath_fill_stroke. Expected PDFPtr.");
        XSRETURN(1);
    }

    try {     PDF_closepath_fill_stroke(_arg0);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_endpath) {
    PDF * _arg0;
    dXSARGS ;

    if (items != 1)
        croak("Usage: PDF_endpath(p);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_endpath. Expected PDFPtr.");
        XSRETURN(1);
    }

    try {     PDF_endpath(_arg0);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_clip) {
    PDF * _arg0;
    dXSARGS ;

    if (items != 1)
        croak("Usage: PDF_clip(p);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_clip. Expected PDFPtr.");
        XSRETURN(1);
    }

    try {     PDF_clip(_arg0);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_setgray_fill) {
    PDF * _arg0;
    float  _arg1;
    dXSARGS ;

    if (items != 2)
        croak("Usage: PDF_setgray_fill(p,g);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_setgray_fill. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));

    try {     PDF_setgray_fill(_arg0,_arg1);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_setgray_stroke) {
    PDF * _arg0;
    float  _arg1;
    dXSARGS ;

    if (items != 2)
        croak("Usage: PDF_setgray_stroke(p,g);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_setgray_stroke. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));

    try {     PDF_setgray_stroke(_arg0,_arg1);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_setgray) {
    PDF * _arg0;
    float  _arg1;
    dXSARGS ;

    if (items != 2)
        croak("Usage: PDF_setgray(p,g);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_setgray. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));

    try {     PDF_setgray(_arg0,_arg1);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_setrgbcolor_fill) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    dXSARGS ;

    if (items != 4)
        croak("Usage: PDF_setrgbcolor_fill(p,red,green,blue);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_setrgbcolor_fill. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));
    _arg2 = (float ) SvNV(ST(2));
    _arg3 = (float ) SvNV(ST(3));

    try {     PDF_setrgbcolor_fill(_arg0,_arg1,_arg2,_arg3);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_setrgbcolor_stroke) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    dXSARGS ;

    if (items != 4)
        croak("Usage: PDF_setrgbcolor_stroke(p,red,green,blue);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_setrgbcolor_stroke. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));
    _arg2 = (float ) SvNV(ST(2));
    _arg3 = (float ) SvNV(ST(3));

    try {     PDF_setrgbcolor_stroke(_arg0,_arg1,_arg2,_arg3);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_setrgbcolor) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    dXSARGS ;

    if (items != 4)
        croak("Usage: PDF_setrgbcolor(p,red,green,blue);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_setrgbcolor. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));
    _arg2 = (float ) SvNV(ST(2));
    _arg3 = (float ) SvNV(ST(3));

    try {     PDF_setrgbcolor(_arg0,_arg1,_arg2,_arg3);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_place_image) {
    PDF * _arg0;
    int  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    dXSARGS ;

    if (items != 5)
        croak("Usage: PDF_place_image(p,image,x,y,scale);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_place_image. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (int )SvIV(ST(1));
    _arg2 = (float ) SvNV(ST(2));
    _arg3 = (float ) SvNV(ST(3));
    _arg4 = (float ) SvNV(ST(4));

    try {     PDF_place_image(_arg0,_arg1,_arg2,_arg3,_arg4);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_open_image) {
    int  _result;
    PDF * _arg0;
    char * _arg1;
    char * _arg2;
    char * _arg3;
    long  _arg4;
    int  _arg5;
    int  _arg6;
    int  _arg7;
    int  _arg8;
    char * _arg9;
    int argvi = 0;
    dXSARGS ;

    if (items != 10)
        croak("Usage: PDF_open_image(p,type,source,data,length,width,height,components,bpc,params);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_open_image. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (char *) SvPV(ST(1),PL_na);
    _arg2 = (char *) SvPV(ST(2),PL_na);
    _arg3 = (char *) SvPV(ST(3),PL_na);
    _arg4 = (long )SvIV(ST(4));
    _arg5 = (int )SvIV(ST(5));
    _arg6 = (int )SvIV(ST(6));
    _arg7 = (int )SvIV(ST(7));
    _arg8 = (int )SvIV(ST(8));
    _arg9 = (char *) SvPV(ST(9),PL_na);

    try {     _result = (int )PDF_open_image(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7,_arg8,_arg9);
    }
    else {
	SWIG_exception(error_message);
    }
    ST(argvi) = sv_newmortal();
    sv_setiv(ST(argvi++),(IV) _result);
    XSRETURN(argvi);
}

XS(_wrap_PDF_close_image) {
    PDF * _arg0;
    int  _arg1;
    dXSARGS ;

    if (items != 2)
        croak("Usage: PDF_close_image(p,image);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_close_image. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (int )SvIV(ST(1));

    try {     PDF_close_image(_arg0,_arg1);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_open_image_file) {
    int  _result;
    PDF * _arg0;
    char * _arg1;
    char * _arg2;
    char * _arg3;
    int  _arg4;
    int argvi = 0;
    dXSARGS ;

    if (items != 5)
        croak("Usage: PDF_open_image_file(p,type,filename,stringparam,intparam);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_open_image_file. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (char *) SvPV(ST(1),PL_na);
    _arg2 = (char *) SvPV(ST(2),PL_na);
    _arg3 = (char *) SvPV(ST(3),PL_na);
    _arg4 = (int )SvIV(ST(4));

    try {     _result = (int )PDF_open_image_file(_arg0,_arg1,_arg2,_arg3,_arg4);
    }
    else {
	SWIG_exception(error_message);
    }
    ST(argvi) = sv_newmortal();
    sv_setiv(ST(argvi++),(IV) _result);
    XSRETURN(argvi);
}

XS(_wrap_PDF_open_CCITT) {
    int  _result;
    PDF * _arg0;
    char * _arg1;
    int  _arg2;
    int  _arg3;
    int  _arg4;
    int  _arg5;
    int  _arg6;
    int argvi = 0;
    dXSARGS ;

    if (items != 7)
        croak("Usage: PDF_open_CCITT(p,filename,width,height,BitReverse,K,BlackIs1);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_open_CCITT. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (char *) SvPV(ST(1),PL_na);
    _arg2 = (int )SvIV(ST(2));
    _arg3 = (int )SvIV(ST(3));
    _arg4 = (int )SvIV(ST(4));
    _arg5 = (int )SvIV(ST(5));
    _arg6 = (int )SvIV(ST(6));

    try {     _result = (int )PDF_open_CCITT(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6);
    }
    else {
	SWIG_exception(error_message);
    }
    ST(argvi) = sv_newmortal();
    sv_setiv(ST(argvi++),(IV) _result);
    XSRETURN(argvi);
}

XS(_wrap_PDF_add_bookmark) {
    int  _result;
    PDF * _arg0;
    char * _arg1;
    int  _arg2;
    int  _arg3;
    int argvi = 0;
    dXSARGS ;

    if (items != 4)
        croak("Usage: PDF_add_bookmark(p,text,parent,open);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_add_bookmark. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (char *) SvPV(ST(1),PL_na);
    _arg2 = (int )SvIV(ST(2));
    _arg3 = (int )SvIV(ST(3));

    try {     _result = (int )PDF_add_bookmark(_arg0,_arg1,_arg2,_arg3);
    }
    else {
	SWIG_exception(error_message);
    }
    ST(argvi) = sv_newmortal();
    sv_setiv(ST(argvi++),(IV) _result);
    XSRETURN(argvi);
}

XS(_wrap_PDF_set_info) {
    PDF * _arg0;
    char * _arg1;
    char * _arg2;
    dXSARGS ;

    if (items != 3)
        croak("Usage: PDF_set_info(p,key,value);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_set_info. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (char *) SvPV(ST(1),PL_na);
    _arg2 = (char *) SvPV(ST(2),PL_na);

    try {     PDF_set_info(_arg0,_arg1,_arg2);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_attach_file) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    char * _arg5;
    char * _arg6;
    char * _arg7;
    char * _arg8;
    char * _arg9;
    dXSARGS ;

    if (items != 10)
        croak("Usage: PDF_attach_file(p,llx,lly,urx,ury,filename,description,author,mimetype,icon);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_attach_file. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));
    _arg2 = (float ) SvNV(ST(2));
    _arg3 = (float ) SvNV(ST(3));
    _arg4 = (float ) SvNV(ST(4));
    _arg5 = (char *) SvPV(ST(5),PL_na);
    _arg6 = (char *) SvPV(ST(6),PL_na);
    _arg7 = (char *) SvPV(ST(7),PL_na);
    _arg8 = (char *) SvPV(ST(8),PL_na);
    _arg9 = (char *) SvPV(ST(9),PL_na);

    try {     PDF_attach_file(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7,_arg8,_arg9);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_add_note) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    char * _arg5;
    char * _arg6;
    char * _arg7;
    int  _arg8;
    dXSARGS ;

    if (items != 9)
        croak("Usage: PDF_add_note(p,llx,lly,urx,ury,contents,title,icon,open);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_add_note. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));
    _arg2 = (float ) SvNV(ST(2));
    _arg3 = (float ) SvNV(ST(3));
    _arg4 = (float ) SvNV(ST(4));
    _arg5 = (char *) SvPV(ST(5),PL_na);
    _arg6 = (char *) SvPV(ST(6),PL_na);
    _arg7 = (char *) SvPV(ST(7),PL_na);
    _arg8 = (int )SvIV(ST(8));

    try {     PDF_add_note(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7,_arg8);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_add_pdflink) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    char * _arg5;
    int  _arg6;
    char * _arg7;
    dXSARGS ;

    if (items != 8)
        croak("Usage: PDF_add_pdflink(p,llx,lly,urx,ury,filename,page,dest);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_add_pdflink. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));
    _arg2 = (float ) SvNV(ST(2));
    _arg3 = (float ) SvNV(ST(3));
    _arg4 = (float ) SvNV(ST(4));
    _arg5 = (char *) SvPV(ST(5),PL_na);
    _arg6 = (int )SvIV(ST(6));
    _arg7 = (char *) SvPV(ST(7),PL_na);

    try {     PDF_add_pdflink(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_add_launchlink) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    char * _arg5;
    dXSARGS ;

    if (items != 6)
        croak("Usage: PDF_add_launchlink(p,llx,lly,urx,ury,filename);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_add_launchlink. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));
    _arg2 = (float ) SvNV(ST(2));
    _arg3 = (float ) SvNV(ST(3));
    _arg4 = (float ) SvNV(ST(4));
    _arg5 = (char *) SvPV(ST(5),PL_na);

    try {     PDF_add_launchlink(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_add_locallink) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    int  _arg5;
    char * _arg6;
    dXSARGS ;

    if (items != 7)
        croak("Usage: PDF_add_locallink(p,llx,lly,urx,ury,page,dest);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_add_locallink. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));
    _arg2 = (float ) SvNV(ST(2));
    _arg3 = (float ) SvNV(ST(3));
    _arg4 = (float ) SvNV(ST(4));
    _arg5 = (int )SvIV(ST(5));
    _arg6 = (char *) SvPV(ST(6),PL_na);

    try {     PDF_add_locallink(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_add_weblink) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    char * _arg5;
    dXSARGS ;

    if (items != 6)
        croak("Usage: PDF_add_weblink(p,llx,lly,urx,ury,url);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_add_weblink. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));
    _arg2 = (float ) SvNV(ST(2));
    _arg3 = (float ) SvNV(ST(3));
    _arg4 = (float ) SvNV(ST(4));
    _arg5 = (char *) SvPV(ST(5),PL_na);

    try {     PDF_add_weblink(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_set_border_style) {
    PDF * _arg0;
    char * _arg1;
    float  _arg2;
    dXSARGS ;

    if (items != 3)
        croak("Usage: PDF_set_border_style(p,style,width);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_set_border_style. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (char *) SvPV(ST(1),PL_na);
    _arg2 = (float ) SvNV(ST(2));

    try {     PDF_set_border_style(_arg0,_arg1,_arg2);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_set_border_color) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    dXSARGS ;

    if (items != 4)
        croak("Usage: PDF_set_border_color(p,red,green,blue);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_set_border_color. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));
    _arg2 = (float ) SvNV(ST(2));
    _arg3 = (float ) SvNV(ST(3));

    try {     PDF_set_border_color(_arg0,_arg1,_arg2,_arg3);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_PDF_set_border_dash) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    dXSARGS ;

    if (items != 3)
        croak("Usage: PDF_set_border_dash(p,w,b);");
    if (SWIG_GetPtr(ST(0),(void **) &_arg0,"PDFPtr")) {
        croak("Type error in argument 1 of PDF_set_border_dash. Expected PDFPtr.");
        XSRETURN(1);
    }
    _arg1 = (float ) SvNV(ST(1));
    _arg2 = (float ) SvNV(ST(2));

    try {     PDF_set_border_dash(_arg0,_arg1,_arg2);
    }
    else {
	SWIG_exception(error_message);
    }
    XSRETURN(0);
}

XS(_wrap_perl5_pdflib_var_init) {
    dXSARGS;
    XSRETURN(1);
}
#ifdef __cplusplus
extern "C"
#endif
XS(boot_pdflib_pl) {
    dXSARGS;
    char *file = __FILE__;
    newXS("pdflibc::var_pdflib_init", _wrap_perl5_pdflib_var_init, file);

    /* Boot the PDFlib core */
    PDF_boot();

    newXS("pdflibc::PDF_new", _wrap_PDF_new, file);
    newXS("pdflibc::PDF_delete", _wrap_PDF_delete, file);
    newXS("pdflibc::PDF_open_file", _wrap_PDF_open_file, file);
    newXS("pdflibc::PDF_close", _wrap_PDF_close, file);
    newXS("pdflibc::PDF_get_buffer", _wrap_PDF_get_buffer, file);
    newXS("pdflibc::PDF_begin_page", _wrap_PDF_begin_page, file);
    newXS("pdflibc::PDF_end_page", _wrap_PDF_end_page, file);
    newXS("pdflibc::PDF_set_parameter", _wrap_PDF_set_parameter, file);
    newXS("pdflibc::PDF_get_parameter", _wrap_PDF_get_parameter, file);
    newXS("pdflibc::PDF_set_value", _wrap_PDF_set_value, file);
    newXS("pdflibc::PDF_get_value", _wrap_PDF_get_value, file);
    newXS("pdflibc::PDF_findfont", _wrap_PDF_findfont, file);
    newXS("pdflibc::PDF_setfont", _wrap_PDF_setfont, file);
    newXS("pdflibc::PDF_show", _wrap_PDF_show, file);
    newXS("pdflibc::PDF_show_xy", _wrap_PDF_show_xy, file);
    newXS("pdflibc::PDF_continue_text", _wrap_PDF_continue_text, file);
    newXS("pdflibc::PDF_show_boxed", _wrap_PDF_show_boxed, file);
    newXS("pdflibc::PDF_set_text_pos", _wrap_PDF_set_text_pos, file);
    newXS("pdflibc::PDF_set_text_matrix", _wrap_PDF_set_text_matrix, file);
    newXS("pdflibc::PDF_stringwidth", _wrap_PDF_stringwidth, file);
    newXS("pdflibc::PDF_setdash", _wrap_PDF_setdash, file);
    newXS("pdflibc::PDF_setpolydash", _wrap_PDF_setpolydash, file);
    newXS("pdflibc::PDF_setflat", _wrap_PDF_setflat, file);
    newXS("pdflibc::PDF_setlinejoin", _wrap_PDF_setlinejoin, file);
    newXS("pdflibc::PDF_setlinecap", _wrap_PDF_setlinecap, file);
    newXS("pdflibc::PDF_setmiterlimit", _wrap_PDF_setmiterlimit, file);
    newXS("pdflibc::PDF_setlinewidth", _wrap_PDF_setlinewidth, file);
    newXS("pdflibc::PDF_save", _wrap_PDF_save, file);
    newXS("pdflibc::PDF_restore", _wrap_PDF_restore, file);
    newXS("pdflibc::PDF_translate", _wrap_PDF_translate, file);
    newXS("pdflibc::PDF_scale", _wrap_PDF_scale, file);
    newXS("pdflibc::PDF_rotate", _wrap_PDF_rotate, file);
    newXS("pdflibc::PDF_skew", _wrap_PDF_skew, file);
    newXS("pdflibc::PDF_concat", _wrap_PDF_concat, file);
    newXS("pdflibc::PDF_moveto", _wrap_PDF_moveto, file);
    newXS("pdflibc::PDF_lineto", _wrap_PDF_lineto, file);
    newXS("pdflibc::PDF_curveto", _wrap_PDF_curveto, file);
    newXS("pdflibc::PDF_circle", _wrap_PDF_circle, file);
    newXS("pdflibc::PDF_arc", _wrap_PDF_arc, file);
    newXS("pdflibc::PDF_rect", _wrap_PDF_rect, file);
    newXS("pdflibc::PDF_closepath", _wrap_PDF_closepath, file);
    newXS("pdflibc::PDF_stroke", _wrap_PDF_stroke, file);
    newXS("pdflibc::PDF_closepath_stroke", _wrap_PDF_closepath_stroke, file);
    newXS("pdflibc::PDF_fill", _wrap_PDF_fill, file);
    newXS("pdflibc::PDF_fill_stroke", _wrap_PDF_fill_stroke, file);
    newXS("pdflibc::PDF_closepath_fill_stroke", _wrap_PDF_closepath_fill_stroke, file);
    newXS("pdflibc::PDF_endpath", _wrap_PDF_endpath, file);
    newXS("pdflibc::PDF_clip", _wrap_PDF_clip, file);
    newXS("pdflibc::PDF_setgray_fill", _wrap_PDF_setgray_fill, file);
    newXS("pdflibc::PDF_setgray_stroke", _wrap_PDF_setgray_stroke, file);
    newXS("pdflibc::PDF_setgray", _wrap_PDF_setgray, file);
    newXS("pdflibc::PDF_setrgbcolor_fill", _wrap_PDF_setrgbcolor_fill, file);
    newXS("pdflibc::PDF_setrgbcolor_stroke", _wrap_PDF_setrgbcolor_stroke, file);
    newXS("pdflibc::PDF_setrgbcolor", _wrap_PDF_setrgbcolor, file);
    newXS("pdflibc::PDF_place_image", _wrap_PDF_place_image, file);
    newXS("pdflibc::PDF_open_image", _wrap_PDF_open_image, file);
    newXS("pdflibc::PDF_close_image", _wrap_PDF_close_image, file);
    newXS("pdflibc::PDF_open_image_file", _wrap_PDF_open_image_file, file);
    newXS("pdflibc::PDF_open_CCITT", _wrap_PDF_open_CCITT, file);
    newXS("pdflibc::PDF_add_bookmark", _wrap_PDF_add_bookmark, file);
    newXS("pdflibc::PDF_set_info", _wrap_PDF_set_info, file);
    newXS("pdflibc::PDF_attach_file", _wrap_PDF_attach_file, file);
    newXS("pdflibc::PDF_add_note", _wrap_PDF_add_note, file);
    newXS("pdflibc::PDF_add_pdflink", _wrap_PDF_add_pdflink, file);
    newXS("pdflibc::PDF_add_launchlink", _wrap_PDF_add_launchlink, file);
    newXS("pdflibc::PDF_add_locallink", _wrap_PDF_add_locallink, file);
    newXS("pdflibc::PDF_add_weblink", _wrap_PDF_add_weblink, file);
    newXS("pdflibc::PDF_set_border_style", _wrap_PDF_set_border_style, file);
    newXS("pdflibc::PDF_set_border_color", _wrap_PDF_set_border_color, file);
    newXS("pdflibc::PDF_set_border_dash", _wrap_PDF_set_border_dash, file);
/*
 * These are the pointer type-equivalency mappings. 
 * (Used by the SWIG pointer type-checker).
 */
    SWIG_RegisterMapping("unsigned short","short",0);
    SWIG_RegisterMapping("PDF","struct PDF_s",0);
    SWIG_RegisterMapping("long","unsigned long",0);
    SWIG_RegisterMapping("long","signed long",0);
    SWIG_RegisterMapping("signed short","short",0);
    SWIG_RegisterMapping("signed int","int",0);
    SWIG_RegisterMapping("short","unsigned short",0);
    SWIG_RegisterMapping("short","signed short",0);
    SWIG_RegisterMapping("unsigned long","long",0);
    SWIG_RegisterMapping("int","unsigned int",0);
    SWIG_RegisterMapping("int","signed int",0);
    SWIG_RegisterMapping("unsigned int","int",0);
    SWIG_RegisterMapping("struct PDF_s","PDF",0);
    SWIG_RegisterMapping("signed long","long",0);
    ST(0) = &PL_sv_yes;
    XSRETURN(1);
}
