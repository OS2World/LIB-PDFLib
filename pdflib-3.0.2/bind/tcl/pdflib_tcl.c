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

/* pdflib_tcl.c
 *
 * Wrapper code for the PDFlib Tcl binding
 *
 */

#include <tcl.h>

#include <string.h>
#include <stdlib.h>

#if defined(__WIN32__)
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   undef WIN32_LEAN_AND_MEAN

#   if defined(__WIN32__) && \
	(defined(_MSC_VER) || (defined(__GNUC__) && defined(__declspec)))
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

/* SWIG pointer structure */

typedef struct SwigPtrType {
  char               *name;               /* Datatype name                  */
  int                 len;                /* Length (used for optimization) */
  void               *(*cast)(void *);    /* Pointer casting function       */
  struct SwigPtrType *next;               /* Linked list pointer            */
} SwigPtrType;

/* Pointer cache structure */

typedef struct {
  int                 stat;               /* Status (valid) bit             */
  SwigPtrType        *tp;                 /* Pointer to type structure      */
  char                name[256];          /* Given datatype name            */
  char                mapped[256];        /* Equivalent name                */
} SwigCacheType;

/* Some variables  */

static int SwigPtrMax  = 64;           /* Max entries that can be currently held */
                                       /* This value may be adjusted dynamically */
static int SwigPtrN    = 0;            /* Current number of entries              */
static int SwigPtrSort = 0;            /* Status flag indicating sort            */
static int SwigStart[256];             /* Starting positions of types            */

/* Pointer table */
static SwigPtrType *SwigPtrTable = 0;  /* Table containing pointer equivalences  */

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

SWIGSTATIC 
void SWIG_RegisterMapping(char *origtype, char *newtype, void *(*cast)(void *)) {

  int i;
  SwigPtrType *t = 0,*t1;

  /* Allocate the pointer table if necessary */

  if (!SwigPtrTable) {     
    SwigPtrTable = (SwigPtrType *) malloc(SwigPtrMax*sizeof(SwigPtrType));
    SwigPtrN = 0;
  }
  /* Grow the table */
  if (SwigPtrN >= SwigPtrMax) {
    SwigPtrMax = 2*SwigPtrMax;
    SwigPtrTable = (SwigPtrType *) realloc((char *) SwigPtrTable,SwigPtrMax*sizeof(SwigPtrType));
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

  /* Check for existing entry */

  while (t->next) {
    if ((strcmp(t->name,newtype) == 0)) {
      if (cast) t->cast = cast;
      return;
    }
    t = t->next;
  }
  
  /* Now place entry (in sorted order) */

  t1 = (SwigPtrType *) malloc(sizeof(SwigPtrType));
  t1->name = newtype;
  t1->len = strlen(t1->name);
  t1->cast = cast;
  t1->next = 0;            
  t->next = t1;           
  SwigPtrSort = 0;
}

/* Make a pointer value string */

SWIGSTATIC 
void SWIG_MakePtr(char *_c, const void *_ptr, char *type) {
  static char _hex[16] =
  {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
   'a', 'b', 'c', 'd', 'e', 'f'};
  unsigned long _p, _s;
  char _result[20], *_r;    /* Note : a 64-bit hex number = 16 digits */
  _r = _result;
  _p = (unsigned long) _ptr;
  if (_p > 0) {
    while (_p > 0) {
      _s = _p & 0xf;
      *(_r++) = _hex[_s];
      _p = _p >> 4;
    }
    *_r = '_';
    while (_r >= _result)
      *(_c++) = *(_r--);
  } else {
    strcpy (_c, "NULL");
  }
  if (_ptr)
    strcpy (_c, type);
}

/* Function for getting a pointer value */

SWIGSTATIC 
char *SWIG_GetPtr(char *_c, void **ptr, char *_t)
{
  unsigned long _p;
  char temp_type[256];
  char *name;
  int  i, len;
  SwigPtrType *sp,*tp;
  SwigCacheType *cache;
  int  start, end;
  _p = 0;

  /* Pointer values must start with leading underscore */
  if (*_c == '_') {
      _c++;
      /* Extract hex value from pointer */
      while (*_c) {
	  if ((*_c >= '0') && (*_c <= '9'))
	    _p = (_p << 4) + (*_c - '0');
	  else if ((*_c >= 'a') && (*_c <= 'f'))
	    _p = (_p << 4) + ((*_c - 'a') + 10);
	  else
	    break;
	  _c++;
      }

      if (_t) {
	if (strcmp(_t,_c)) { 
	  if (!SwigPtrSort) {
	    qsort((void *) SwigPtrTable, SwigPtrN, sizeof(SwigPtrType), swigsort); 
	    for (i = 0; i < 256; i++) {
	      SwigStart[i] = SwigPtrN;
	    }
	    for (i = SwigPtrN-1; i >= 0; i--) {
	      SwigStart[(int) (SwigPtrTable[i].name[1])] = i;
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
		  *ptr = (void *) _p;
		  if (cache->tp->cast) *ptr = (*(cache->tp->cast))(*ptr);
		  return (char *) 0;
		}
	      }
	    }
	    SwigLastCache = (SwigLastCache+1) & SWIG_CACHEMASK;
	    if (!SwigLastCache) cache = SwigCache;
	    else cache++;
	  }
	  /* We have a type mismatch.  Will have to look through our type
	     mapping table to figure out whether or not we can accept this datatype */

	  start = SwigStart[(int) _t[1]];
	  end = SwigStart[(int) _t[1]+1];
	  sp = &SwigPtrTable[start];
	  while (start < end) {
	    if (swigcmp(_t,sp) == 0) break;
	    sp++;
	    start++;
	  }
	  if (start >= end) sp = 0;
	  /* Try to find a match for this */
	  if (sp) {
	    while (swigcmp(_t,sp) == 0) {
	      name = sp->name;
	      len = sp->len;
	      tp = sp->next;
	      /* Try to find entry for our given datatype */
	      while(tp) {
		if (tp->len >= 255) {
		  return _c;
		}
		strcpy(temp_type,tp->name);
		strncat(temp_type,_t+len,255-tp->len);
		if (strcmp(_c,temp_type) == 0) {
		  
		  strcpy(SwigCache[SwigCacheIndex].mapped,_c);
		  strcpy(SwigCache[SwigCacheIndex].name,_t);
		  SwigCache[SwigCacheIndex].stat = 1;
		  SwigCache[SwigCacheIndex].tp = tp;
		  SwigCacheIndex = SwigCacheIndex & SWIG_CACHEMASK;
		  
		  /* Get pointer value */
		  *ptr = (void *) _p;
		  if (tp->cast) *ptr = (*(tp->cast))(*ptr);
		  return (char *) 0;
		}
		tp = tp->next;
	      }
	      sp++;
	      /* Hmmm. Didn't find it this time */
	    }
	  }
	  /* Didn't find any sort of match for this data.  
	     Get the pointer value and return the received type */
	  *ptr = (void *) _p;
	  return _c;
	} else {
	  /* Found a match on the first try.  Return pointer value */
	  *ptr = (void *) _p;
	  return (char *) 0;
	}
      } else {
	/* No type specified.  Good luck */
	*ptr = (void *) _p;
	return (char *) 0;
      }
  } else {
    if (strcmp (_c, "NULL") == 0) {
	*ptr = (void *) 0;
	return (char *) 0;
    }
    *ptr = (void *) 0;	
    return _c;
  }
}

#ifdef __cplusplus
extern "C" {
#endif
#ifdef MAC_TCL
#pragma export on
#endif
SWIGEXPORT(int,Pdflib_Init)(Tcl_Interp *);
#ifdef MAC_TCL
#pragma export off
#endif
#ifdef __cplusplus
}
#endif

#include <setjmp.h>

#include "pdflib.h"

/* thread-specific data */
typedef struct {
    jmp_buf     env;
    int         type;
    char        buffer[256];
    int		nativeunicode;
} pdf_wrapper_data;

/* Exception handling */
/* This is used in the wrapper functions for thread-safe exception handling */
#define EXC_STUFF pdf_wrapper_data *ex

#define try		ex = (pdf_wrapper_data *) (PDF_get_opaque(p));	\
			if (setjmp(ex->env) == 0)
#define catch		else { Tcl_SetResult(interp,ex->buffer,TCL_STATIC);	\
				return TCL_ERROR;				\
			}
#define throw(error)	longjmp(ex->env, error)


/* PDFlib error handler */

static void
pdf_errorhandler(PDF *p, int type, const char* shortmsg)
{
    EXC_STUFF;

    if (!p)
	return;

    /* retrieve opaque pointer */
    ex = (pdf_wrapper_data *) (PDF_get_opaque(p));

    sprintf(ex->buffer, "PDFlib: %s\n", shortmsg);

    /* Issue a warning message and continue for non-fatal errors */
    if (type == PDF_NonfatalError) {
	fprintf(stderr, ex->buffer);
	return;
    }
	
    /* clean up PDFlib internals and delete p */
    PDF_delete(p);

    /* ...and throw an exception */
    throw(type);
}

/* Unicode support is only available in Tcl 8.2 and higher */

#if 10 * TCL_MAJOR_VERSION + TCL_MINOR_VERSION >= 82
/* 
 * Unicode strings - wrap with BOM and double-null if so desired
 */

/* The Unicode byte order mark (BOM) signals Unicode strings */
#define PDF_BOM0		0376		/* '\xFE' */
#define PDF_BOM1		0377		/* '\xFF' */

static char *
GetStringUnicodePDFChars(PDF *p, Tcl_Interp *interp, Tcl_Obj *objPtr, int *lenP)
{
    Tcl_UniChar *unistring;
    unsigned char *result, have_unicode = 0;
    size_t i, len;
    EXC_STUFF;

    if (!objPtr) {
	if (lenP)
	    *lenP = 0;
	return NULL;
    }

    ex = (pdf_wrapper_data *) (PDF_get_opaque(p));

    unistring = Tcl_GetUnicode(objPtr);

    if (!unistring) {
	if (lenP)
	    *lenP = 0;
	return NULL;
    }

    len = (size_t) Tcl_UniCharLen(unistring);

    for (i = 0; i < len; i++) {
	/* check the high bytes */
	if (unistring[i] > 0xFF) {
	    have_unicode = 1;
	    break;
	}
    }

    /* 
     * Unicode: 
     * BOM plus two byte per character plus two null bytes, if the caller
     * requested so by supplying lenP = NULL. If lenP is supplied, it
     * points to the number of Unicode characters in the string.
     *
     * not Unicode: one byte per character plus null byte.
     */
    result = (unsigned char *)
	malloc(have_unicode ? (lenP ? 2*len : 2*len + 4) : len + 1);

    if (!result) {
	if (lenP)
	    *lenP = 0;
	return NULL;
    }

    if (ex->nativeunicode && have_unicode) {
	if (!lenP) {
	    /* copy Unicode and wrap with BOM and double-null */
	    result[0] = PDF_BOM0;
	    result[1] = PDF_BOM1;

	    /* PDF requires the high-order byte first */
	    for (i = 0; i < len; i++) {
		result[2*i+2] = (unsigned char) (unistring[i] >> 8);
		result[2*i+3] = (unsigned char) unistring[i];
	    }

	    result[2*len + 2] = result[2*len + 3] = '\0';

	} else {
	    *lenP = (int) (2 * len);

	    /* pure Unicode string */
	    for (i = 0; i < len; i++) {
		result[2*i] = (unsigned char) (unistring[i] >> 8);
		result[2*i+1] = (unsigned char) unistring[i];
	    }
	}

    } else {
	if (lenP)
	    *lenP = (int) len;

	if (!have_unicode) {
	    /* pick the low-order bytes only */
	    for (i = 0; i < len; i++) {
		result[i] = (unsigned char) unistring[i];
	    }
	    result[i] = '\0';	/* NULL-terminate */
	} else {
#ifdef NATIVE_CONVERSION_DONE
	    /* TODO: convert to native encoding */
	    if (Tcl_UtfToExternal(interp, NULL, unistring, len,
	    	0, NULL, result, 2*len+1, NULL, NULL, NULL) != TCL_OK) {
		return NULL;
	    }
#else
	    for (i = 0; i < len; i++) {
		result[i] = (unsigned char) unistring[i];
	    }
#endif
	    result[i] = '\0';	/* NULL-terminate */
	}
    }

    return (char *) result;
}

#define ReleaseStringPDFChars(chars) if(chars) free(chars);

#else /* Tcl version older than 8.2 */

/* The cheap version doesn't know about Unicode strings */

#define GetStringUnicodePDFChars(p, interp, objPtr, lenP) \
	    Tcl_GetStringFromObj(objPtr, NULL)

#define ReleaseStringPDFChars(chars) /* not needed */

#endif /* Tcl version older than 8.2 */

/* export the PDFlib routines to the shared library */
#ifdef __MWERKS__
#pragma export on
#endif

static int
_wrap_PDF_new(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    char versionbuf[32];
    EXC_STUFF;

    if (argc != 1) {
        Tcl_SetResult(interp, "Wrong # args. PDF_new ",TCL_STATIC);
        return TCL_ERROR;
    }

    if ((ex = malloc(sizeof(pdf_wrapper_data))) == NULL) {
	Tcl_SetResult(interp, "Couldn't initialize PDFlib", TCL_STATIC);
	return TCL_ERROR;
    }

    ex->nativeunicode = 0;

    p = (PDF *)PDF_new2(pdf_errorhandler, NULL, NULL, NULL, (void *) ex);

    if (p) {

/* The GetVersion API appeared in Tcl 8.1 */
#if 10 * TCL_MAJOR_VERSION + TCL_MINOR_VERSION >= 81
	int major, minor, type, patchlevel;
	Tcl_GetVersion(&major, &minor, &patchlevel, &type);
	sprintf(versionbuf, "Tcl %d.%d%c%d", major, minor, "ab."[type], patchlevel);
#else
#ifdef TCL_PATCH_LEVEL
	sprintf(versionbuf, "Tcl %s", TCL_PATCH_LEVEL);
#else
	sprintf(versionbuf, "Tcl (unknown)");
#endif
#endif
	PDF_set_parameter(p, "binding", versionbuf);
    }

    SWIG_MakePtr(interp->result, (void *) p,"_PDF_p");
    return TCL_OK;
}

static int
_wrap_PDF_delete(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    EXC_STUFF;

    if (argc != 2) {
        Tcl_SetResult(interp, "Wrong # args. PDF_delete p ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_delete. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    try {     PDF_delete(p);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_open_file(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    int  _result;
    PDF *p;
    char * _arg1;
    EXC_STUFF;

    if (argc != 3) {
        Tcl_SetResult(interp, "Wrong # args. PDF_open_file p filename ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_open_file. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = argv[2];

    try {     _result = (int)PDF_open_file(p,_arg1);
    } catch;

    sprintf(interp->result,"%ld", (long) _result);
    return TCL_OK;
}

static int
_wrap_PDF_close(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    EXC_STUFF;

    if (argc != 2) {
        Tcl_SetResult(interp, "Wrong # args. PDF_close p ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_close. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    try {     PDF_close(p);
    } catch;

    return TCL_OK;
}

static int _wrap_PDF_get_buffer(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {

    const char * buffer;
    PDF *p;
    Tcl_Obj *result;
    char *res;
    long size;
    EXC_STUFF;

    if (objc != 2) {
        Tcl_SetResult(interp, "Wrong # args. PDF_get_buffer p", TCL_STATIC);
        return TCL_ERROR;
    }

    if ((res = Tcl_GetStringFromObj(objv[1], NULL)) == NULL) {
        Tcl_SetResult(interp, "Couldn't retrieve PDF pointer", TCL_STATIC);
	return TCL_ERROR;
    }

    if (SWIG_GetPtr(res, (void **) &p, "_PDF_p")) {
        Tcl_SetResult(interp,
	"Type error in argument 1 of PDF_get_buffer. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, res, (char *) NULL);
        return TCL_ERROR;
    }

    try {     buffer = PDF_get_buffer(p, &size);
    } catch;

    result = Tcl_GetObjResult(interp);

/* ByteArrays appeared only in Tcl 8.1 */

#if 10 * TCL_MAJOR_VERSION + TCL_MINOR_VERSION >= 81
    Tcl_SetByteArrayObj(result, (unsigned char *) buffer, (int) size);

#else /* Tcl 8.0 */

    Tcl_SetStringObj(result, (char *) buffer, (int) size);

#endif /* Tcl 8.0 */

    return TCL_OK;
}

static int
_wrap_PDF_begin_page(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    float  _arg1;
    float  _arg2;
    EXC_STUFF;

    if (argc != 4) {
        Tcl_SetResult(interp, "Wrong # args. PDF_begin_page p width height ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_begin_page. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = (float) atof(argv[2]);
    _arg2 = (float) atof(argv[3]);

    try {     PDF_begin_page(p,_arg1,_arg2);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_end_page(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    EXC_STUFF;

    if (argc != 2) {
        Tcl_SetResult(interp, "Wrong # args. PDF_end_page p ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_end_page. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    try {     PDF_end_page(p);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_set_parameter(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    char * _arg1;
    char * _arg2;
    EXC_STUFF;

    if (argc != 4) {
        Tcl_SetResult(interp, "Wrong # args. PDF_set_parameter p key value ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_set_parameter. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = argv[2];
    _arg2 = argv[3];

    try {
	if (!strcmp(_arg1, "nativeunicode")) {
	    if (!strcmp(_arg2, "true"))
		ex->nativeunicode = 1;
	    else if (!strcmp(_arg2, "false"))
		ex->nativeunicode = 0;
	    else
		PDF_set_parameter(p, _arg1, _arg2);
	} else
	    PDF_set_parameter(p, _arg1, _arg2);

    } catch;

    return TCL_OK;
}

static int _wrap_PDF_get_parameter(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    char * _result;
    PDF *p;
    char * _arg1;
    float  _arg2;
    EXC_STUFF;

    if (argc != 4) {
        Tcl_SetResult(interp, "Wrong # args. PDF_get_parameter p key mod ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_get_parameter. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }
    _arg1 = argv[2];
    _arg2 = (float ) atof(argv[3]);

    try {     _result = (char *)PDF_get_parameter(p,_arg1,_arg2);
    } catch;

    Tcl_SetResult(interp, (char *) _result, TCL_VOLATILE);
    return TCL_OK;
}

static int
_wrap_PDF_set_value(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {

    PDF *p;
    char * _arg1;
    float  _arg2;
    EXC_STUFF;

    if (argc != 4) {
        Tcl_SetResult(interp, "Wrong # args. PDF_set_value p key value ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_set_value. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = argv[2];
    _arg2 = (float ) atof(argv[3]);

    try {     PDF_set_value(p,_arg1,_arg2);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_get_value(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {

    float  _result;
    PDF *p;
    char * _arg1;
    float  _arg2;
    EXC_STUFF;

    if (argc != 4) {
        Tcl_SetResult(interp, "Wrong # args. PDF_get_value p key mod ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_get_value. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }
    _arg1 = argv[2];
    _arg2 = (float ) atof(argv[3]);

    try {     _result = (float )PDF_get_value(p,_arg1,_arg2);
    } catch;

    Tcl_PrintDouble(interp,(double) _result, interp->result);
    return TCL_OK;
}

static int
_wrap_PDF_findfont(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    int  _result;
    PDF *p;
    char * _arg1;
    char * _arg2;
    int  _arg3;
    EXC_STUFF;

    if (argc != 5) {
        Tcl_SetResult(interp, "Wrong # args. PDF_findfont p fontname encoding embed ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_findfont. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = argv[2];
    _arg2 = argv[3];

    if (Tcl_GetInt(interp, argv[4], &_arg3) != TCL_OK) {
        Tcl_SetResult(interp, "Type error in argument 4 of PDF_findfont: ",
	    TCL_STATIC);
        Tcl_AppendResult(interp, argv[4], (char *) NULL);
	return TCL_ERROR;
    }

    try {     _result = (int)PDF_findfont(p,_arg1,_arg2,_arg3);
    } catch;

    sprintf(interp->result,"%ld", (long) _result);
    return TCL_OK;
}

static int
_wrap_PDF_setfont(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    int  _arg1;
    float  _arg2;
    EXC_STUFF;

    if (argc != 4) {
        Tcl_SetResult(interp, "Wrong # args. PDF_setfont p font fontsize ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_setfont. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    if (Tcl_GetInt(interp, argv[2], &_arg1) != TCL_OK) {
        Tcl_SetResult(interp, "Type error in argument 2 of PDF_setfont: ",
	    TCL_STATIC);
        Tcl_AppendResult(interp, argv[2], (char *) NULL);
	return TCL_ERROR;
    }
    _arg2 = (float) atof(argv[3]);

    try {     PDF_setfont(p,_arg1,_arg2);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_show(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
    PDF *p;
    char * _arg1;
    int len = 0;
    char *res;
    EXC_STUFF;

    if (objc != 3) {
        Tcl_SetResult(interp, "Wrong # args. PDF_show p text ",TCL_STATIC);
        return TCL_ERROR;
    }

    if ((res = Tcl_GetStringFromObj(objv[1], NULL)) == NULL) {
        Tcl_SetResult(interp, "Couldn't retrieve PDF pointer", TCL_STATIC);
	return TCL_ERROR;
    }

    if (SWIG_GetPtr(res, (void **) &p, "_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_show. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, res, (char *) NULL);
        return TCL_ERROR;
    }

    if ((_arg1 = GetStringUnicodePDFChars(p, interp, objv[2], &len)) == NULL) {
        Tcl_SetResult(interp, "Couldn't retrieve text argument in PDF_show", TCL_STATIC);
	return TCL_ERROR;
    }

    try {     PDF_show2(p, _arg1, len);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_show_xy(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
    PDF *p;
    char * _arg1;
    double  _arg2;
    double  _arg3;
    int len = 0, error;
    char *res;
    EXC_STUFF;

    if (objc != 5) {
        Tcl_SetResult(interp, "Wrong # args. PDF_show_xy p text x y ",TCL_STATIC);
        return TCL_ERROR;
    }

    if ((res = Tcl_GetStringFromObj(objv[1], NULL)) == NULL) {
        Tcl_SetResult(interp, "Couldn't retrieve PDF pointer", TCL_STATIC);
	return TCL_ERROR;
    }

    if (SWIG_GetPtr(res, (void **) &p, "_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_show_xy. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, res, (char *) NULL);
        return TCL_ERROR;
    }

    if ((_arg1 = GetStringUnicodePDFChars(p, interp, objv[2], &len)) == NULL) {
        Tcl_SetResult(interp, "Couldn't retrieve text argument in PDF_show_xy", TCL_STATIC);
	return TCL_ERROR;
    }

    if ((error = Tcl_GetDoubleFromObj(interp, objv[3], &_arg2)) != TCL_OK) {
        Tcl_SetResult(interp, "Couldn't retrieve x argument in PDF_show_xy", TCL_STATIC);
	return TCL_ERROR;
    }

    if ((error = Tcl_GetDoubleFromObj(interp, objv[4], &_arg3)) != TCL_OK) {
        Tcl_SetResult(interp, "Couldn't retrieve y argument in PDF_show_xy", TCL_STATIC);
	return TCL_ERROR;
    }

    try {     
	PDF_show_xy2(p, _arg1, len, (float) _arg2, (float) _arg3);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_continue_text(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
    PDF *p;
    char * _arg1;
    int len = 0;
    char *res;
    EXC_STUFF;

    if (objc != 3) {
        Tcl_SetResult(interp, "Wrong # args. PDF_continue_text p text ",TCL_STATIC);
        return TCL_ERROR;
    }

    if ((res = Tcl_GetStringFromObj(objv[1], NULL)) == NULL) {
        Tcl_SetResult(interp, "Couldn't retrieve PDF pointer", TCL_STATIC);
	return TCL_ERROR;
    }

    if (SWIG_GetPtr(res, (void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_continue_text. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, res, (char *) NULL);
        return TCL_ERROR;
    }

    if ((_arg1 = GetStringUnicodePDFChars(p, interp, objv[2], &len)) == NULL) {
        Tcl_SetResult(interp, "Couldn't retrieve text argument in PDF_continue_text", TCL_STATIC);
	return TCL_ERROR;
    }

    try {     
	PDF_continue_text2(p, _arg1, len);
    } catch;

    return TCL_OK;
}

static int _wrap_PDF_show_boxed(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    int  _result;
    PDF *p;
    char * _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    float  _arg5;
    char * _arg6;
    char * _arg7;
    EXC_STUFF;

    if (argc != 9) {
        Tcl_SetResult(interp, "Wrong # args. PDF_show_boxed p text left top width height hmode reserved ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_show_boxed. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }
    _arg1 = argv[2];
    _arg2 = (float ) atof(argv[3]);
    _arg3 = (float ) atof(argv[4]);
    _arg4 = (float ) atof(argv[5]);
    _arg5 = (float ) atof(argv[6]);
    _arg6 = argv[7];
    _arg7 = argv[8];

    try {     _result = (int )PDF_show_boxed(p,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7);
    } catch;

    sprintf(interp->result,"%ld", (long) _result);
    return TCL_OK;
}

static int
_wrap_PDF_set_text_pos(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    float  _arg1;
    float  _arg2;
    EXC_STUFF;

    if (argc != 4) {
        Tcl_SetResult(interp, "Wrong # args. PDF_set_text_pos p x y ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_set_text_pos. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = (float) atof(argv[2]);
    _arg2 = (float) atof(argv[3]);

    try {     PDF_set_text_pos(p,_arg1,_arg2);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_stringwidth(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
    float  _result;
    PDF *p;
    char * _arg1;
    int  _arg2;
    double  _arg3;
    int len = 0, error;
    char *res;
    Tcl_Obj *resultPtr;
    EXC_STUFF;

    if (objc != 5) {
        Tcl_SetResult(interp, "Wrong # args. PDF_stringwidth p text font size ",TCL_STATIC);
        return TCL_ERROR;
    }

    if ((res = Tcl_GetStringFromObj(objv[1], NULL)) == NULL) {
        Tcl_SetResult(interp, "Couldn't retrieve PDF pointer in PDF_stringwidth", TCL_STATIC);
	return TCL_ERROR;
    }

    if (SWIG_GetPtr(res, (void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_stringwidth. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, res, (char *) NULL);
        return TCL_ERROR;
    }

    if ((_arg1 = Tcl_GetStringFromObj(objv[2], &len)) == NULL) {
        Tcl_SetResult(interp, "Couldn't retrieve text argument in PDF_stringwidth", TCL_STATIC);
	return TCL_ERROR;
    }

    if ((error = Tcl_GetIntFromObj(interp, objv[3], &_arg2)) != TCL_OK) {
        Tcl_SetResult(interp, "Couldn't retrieve font argument in PDF_stringwidth", TCL_STATIC);
	return TCL_ERROR;
    }

    if ((error = Tcl_GetDoubleFromObj(interp, objv[4], &_arg3)) != TCL_OK) {
        Tcl_SetResult(interp, "Couldn't retrieve size argument in PDF_stringwidth", TCL_STATIC);
	return TCL_ERROR;
    }

    try {
	_result = (float) PDF_stringwidth2(p, _arg1, len, _arg2, (float) _arg3);
    } catch;

    resultPtr = Tcl_GetObjResult(interp);
    Tcl_SetDoubleObj(resultPtr, (double) _result);

    return TCL_OK;
}

static int
_wrap_PDF_setdash(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    float  _arg1;
    float  _arg2;
    EXC_STUFF;

    if (argc != 4) {
        Tcl_SetResult(interp, "Wrong # args. PDF_setdash p b w ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_setdash. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = (float) atof(argv[2]);
    _arg2 = (float) atof(argv[3]);

    try {     PDF_setdash(p,_arg1,_arg2);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_setpolydash(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
    PDF *p;
    float carray[MAX_DASH_LENGTH];
    double dval;
    int  length, i;
    Tcl_Obj *val;
    char *res;
    EXC_STUFF;

    if (objc != 3) {
        Tcl_SetResult(interp,
	    "Wrong # args. PDF_setpolydash p darray length ", TCL_STATIC);
        return TCL_ERROR;
    }

    if ((res = Tcl_GetStringFromObj(objv[1], NULL)) == NULL) {
	Tcl_SetResult(interp, "Couldn't retrieve PDF pointer", TCL_STATIC);
	return TCL_ERROR;
    }

    if (SWIG_GetPtr(res, (void **) &p, "_PDF_p")) {
	Tcl_SetResult(interp,
	"Type error in argument 1 of PDF_setpolydash. Expected _PDF_p, received ", TCL_STATIC);
	Tcl_AppendResult(interp, res, (char *) NULL);
	return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[2], &length) != TCL_OK) {
	Tcl_SetResult(interp,
	    "Couldn't retrieve array length in PDF_setpolydash", TCL_STATIC);
	return TCL_ERROR;
    }
    if (length > MAX_DASH_LENGTH)
	length = MAX_DASH_LENGTH;

    for (i = 0; i < length; i++) {
	if (Tcl_ListObjIndex(interp, objv[2], i, &val) != TCL_OK ||
	    Tcl_GetDoubleFromObj(interp, val, &dval) != TCL_OK) {
	    Tcl_SetResult(interp,
		"Couldn't retrieve array value in PDF_setpolydash", TCL_STATIC);
	    return TCL_ERROR;
	}
	carray[i] = (float) dval;
    }

    try {     PDF_setpolydash(p, carray, length);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_setflat(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    float  _arg1;
    EXC_STUFF;

    if (argc != 3) {
        Tcl_SetResult(interp, "Wrong # args. PDF_setflat p flatness ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_setflat. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }
    _arg1 = (float) atof(argv[2]);

    try {     PDF_setflat(p,_arg1);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_setlinejoin(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    int  _arg1;
    EXC_STUFF;

    if (argc != 3) {
        Tcl_SetResult(interp, "Wrong # args. PDF_setlinejoin p linejoin ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_setlinejoin. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    if (Tcl_GetInt(interp, argv[2], &_arg1) != TCL_OK) {
        Tcl_SetResult(interp,
	    "Type error in argument 2 of PDF_setlinejoin.", TCL_STATIC);
	return TCL_ERROR;
    }

    try {     PDF_setlinejoin(p,_arg1);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_setlinecap(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    int  _arg1;
    EXC_STUFF;

    if (argc != 3) {
        Tcl_SetResult(interp, "Wrong # args. PDF_setlinecap p linecap ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_setlinecap. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    if (Tcl_GetInt(interp, argv[2], &_arg1) != TCL_OK) {
        Tcl_SetResult(interp,
	    "Type error in argument 2 of PDF_setlinecap.", TCL_STATIC);
	return TCL_ERROR;
    }

    try {     PDF_setlinecap(p,_arg1);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_setmiterlimit(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    float  _arg1;
    EXC_STUFF;

    if (argc != 3) {
        Tcl_SetResult(interp, "Wrong # args. PDF_setmiterlimit p miter ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_setmiterlimit. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = (float) atof(argv[2]);

    try {     PDF_setmiterlimit(p,_arg1);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_setlinewidth(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    float  _arg1;
    EXC_STUFF;

    if (argc != 3) {
        Tcl_SetResult(interp, "Wrong # args. PDF_setlinewidth p width ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_setlinewidth. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = (float) atof(argv[2]);

    try {     PDF_setlinewidth(p,_arg1);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_save(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    EXC_STUFF;

    if (argc != 2) {
        Tcl_SetResult(interp, "Wrong # args. PDF_save p ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_save. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    try {     PDF_save(p);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_restore(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    EXC_STUFF;

    if (argc != 2) {
        Tcl_SetResult(interp, "Wrong # args. PDF_restore p ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_restore. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    try {     PDF_restore(p);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_translate(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    float  _arg1;
    float  _arg2;
    EXC_STUFF;

    if (argc != 4) {
        Tcl_SetResult(interp, "Wrong # args. PDF_translate p tx ty ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_translate. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = (float) atof(argv[2]);
    _arg2 = (float) atof(argv[3]);

    try {     PDF_translate(p,_arg1,_arg2);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_scale(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    float  _arg1;
    float  _arg2;
    EXC_STUFF;

    if (argc != 4) {
        Tcl_SetResult(interp, "Wrong # args. PDF_scale p sx sy ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_scale. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = (float) atof(argv[2]);
    _arg2 = (float) atof(argv[3]);

    try {     PDF_scale(p,_arg1,_arg2);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_rotate(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    float  _arg1;
    EXC_STUFF;

    if (argc != 3) {
        Tcl_SetResult(interp, "Wrong # args. PDF_rotate p phi ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_rotate. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = (float) atof(argv[2]);

    try {     PDF_rotate(p,_arg1);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_skew(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    float  _arg1;
    float  _arg2;
    EXC_STUFF;

    if (argc != 4) {
        Tcl_SetResult(interp, "Wrong # args. PDF_skew p alpha beta ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_skew. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = (float) atof(argv[2]);
    _arg2 = (float) atof(argv[3]);

    try {     PDF_skew(p,_arg1,_arg2);
    } catch;

    return TCL_OK;
}

static int _wrap_PDF_concat(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    float  _arg5;
    float  _arg6;
    EXC_STUFF;

    if (argc != 8) {
        Tcl_SetResult(interp,
		"Wrong # args. PDF_concat p a b c d e f ", TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp,
	"Type error in argument 1 of PDF_concat. Expected _PDF_p, received ",
	    TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }
    _arg1 = (float) atof(argv[2]);
    _arg2 = (float) atof(argv[3]);
    _arg3 = (float) atof(argv[4]);
    _arg4 = (float) atof(argv[5]);
    _arg5 = (float) atof(argv[6]);
    _arg6 = (float) atof(argv[7]);

    try {     PDF_concat(p,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_moveto(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    float  _arg1;
    float  _arg2;
    EXC_STUFF;

    if (argc != 4) {
        Tcl_SetResult(interp, "Wrong # args. PDF_moveto p x y ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_moveto. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = (float) atof(argv[2]);
    _arg2 = (float) atof(argv[3]);

    try {     PDF_moveto(p,_arg1,_arg2);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_lineto(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    float  _arg1;
    float  _arg2;
    EXC_STUFF;

    if (argc != 4) {
        Tcl_SetResult(interp, "Wrong # args. PDF_lineto p x y ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_lineto. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = (float) atof(argv[2]);
    _arg2 = (float) atof(argv[3]);

    try {     PDF_lineto(p,_arg1,_arg2);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_curveto(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    float  _arg5;
    float  _arg6;
    EXC_STUFF;

    if (argc != 8) {
        Tcl_SetResult(interp, "Wrong # args. PDF_curveto p x1 y1 x2 y2 x3 y3 ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_curveto. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = (float) atof(argv[2]);
    _arg2 = (float) atof(argv[3]);
    _arg3 = (float) atof(argv[4]);
    _arg4 = (float) atof(argv[5]);
    _arg5 = (float) atof(argv[6]);
    _arg6 = (float) atof(argv[7]);

    try {     PDF_curveto(p,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_circle(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    EXC_STUFF;

    if (argc != 5) {
        Tcl_SetResult(interp, "Wrong # args. PDF_circle p x y r ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_circle. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }
    _arg1 = (float) atof(argv[2]);
    _arg2 = (float) atof(argv[3]);
    _arg3 = (float) atof(argv[4]);

    try {     PDF_circle(p,_arg1,_arg2,_arg3);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_arc(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    float  _arg5;
    EXC_STUFF;

    if (argc != 7) {
        Tcl_SetResult(interp, "Wrong # args. PDF_arc p x y r alpha1 alpha2 ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_arc. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = (float) atof(argv[2]);
    _arg2 = (float) atof(argv[3]);
    _arg3 = (float) atof(argv[4]);
    _arg4 = (float) atof(argv[5]);
    _arg5 = (float) atof(argv[6]);

    try {     PDF_arc(p,_arg1,_arg2,_arg3,_arg4,_arg5);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_rect(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    EXC_STUFF;

    if (argc != 6) {
        Tcl_SetResult(interp, "Wrong # args. PDF_rect p x y width height ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_rect. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = (float) atof(argv[2]);
    _arg2 = (float) atof(argv[3]);
    _arg3 = (float) atof(argv[4]);
    _arg4 = (float) atof(argv[5]);

    try {     PDF_rect(p,_arg1,_arg2,_arg3,_arg4);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_closepath(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    EXC_STUFF;

    if (argc != 2) {
        Tcl_SetResult(interp, "Wrong # args. PDF_closepath p ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_closepath. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    try {     PDF_closepath(p);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_stroke(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    EXC_STUFF;

    if (argc != 2) {
        Tcl_SetResult(interp, "Wrong # args. PDF_stroke p ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_stroke. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    try {     PDF_stroke(p);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_closepath_stroke(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    EXC_STUFF;

    if (argc != 2) {
        Tcl_SetResult(interp, "Wrong # args. PDF_closepath_stroke p ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_closepath_stroke. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    try {     PDF_closepath_stroke(p);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_fill(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    EXC_STUFF;

    if (argc != 2) {
        Tcl_SetResult(interp, "Wrong # args. PDF_fill p ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_fill. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    try {     PDF_fill(p);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_fill_stroke(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    EXC_STUFF;

    if (argc != 2) {
        Tcl_SetResult(interp, "Wrong # args. PDF_fill_stroke p ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_fill_stroke. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    try {     PDF_fill_stroke(p);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_closepath_fill_stroke(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    EXC_STUFF;

    if (argc != 2) {
        Tcl_SetResult(interp, "Wrong # args. PDF_closepath_fill_stroke p ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_closepath_fill_stroke. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    try {     PDF_closepath_fill_stroke(p);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_endpath(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    EXC_STUFF;

    if (argc != 2) {
        Tcl_SetResult(interp, "Wrong # args. PDF_endpath p ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_endpath. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    try {     PDF_endpath(p);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_clip(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    EXC_STUFF;

    if (argc != 2) {
        Tcl_SetResult(interp, "Wrong # args. PDF_clip p ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_clip. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    try {     PDF_clip(p);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_setgray_fill(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    float  _arg1;
    EXC_STUFF;

    if (argc != 3) {
        Tcl_SetResult(interp, "Wrong # args. PDF_setgray_fill p g ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_setgray_fill. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = (float) atof(argv[2]);

    try {     PDF_setgray_fill(p,_arg1);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_setgray_stroke(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    float  _arg1;
    EXC_STUFF;

    if (argc != 3) {
        Tcl_SetResult(interp, "Wrong # args. PDF_setgray_stroke p g ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_setgray_stroke. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = (float) atof(argv[2]);

    try {     PDF_setgray_stroke(p,_arg1);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_setgray(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    float  _arg1;
    EXC_STUFF;

    if (argc != 3) {
        Tcl_SetResult(interp, "Wrong # args. PDF_setgray p g ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_setgray. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = (float) atof(argv[2]);

    try {     PDF_setgray(p,_arg1);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_setrgbcolor_fill(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    EXC_STUFF;

    if (argc != 5) {
        Tcl_SetResult(interp, "Wrong # args. PDF_setrgbcolor_fill p red green blue ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_setrgbcolor_fill. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = (float) atof(argv[2]);
    _arg2 = (float) atof(argv[3]);
    _arg3 = (float) atof(argv[4]);

    try {     PDF_setrgbcolor_fill(p,_arg1,_arg2,_arg3);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_setrgbcolor_stroke(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    EXC_STUFF;

    if (argc != 5) {
        Tcl_SetResult(interp, "Wrong # args. PDF_setrgbcolor_stroke p red green blue ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_setrgbcolor_stroke. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = (float) atof(argv[2]);
    _arg2 = (float) atof(argv[3]);
    _arg3 = (float) atof(argv[4]);

    try {     PDF_setrgbcolor_stroke(p,_arg1,_arg2,_arg3);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_setrgbcolor(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    EXC_STUFF;

    if (argc != 5) {
        Tcl_SetResult(interp, "Wrong # args. PDF_setrgbcolor p red green blue ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_setrgbcolor. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = (float) atof(argv[2]);
    _arg2 = (float) atof(argv[3]);
    _arg3 = (float) atof(argv[4]);

    try {     PDF_setrgbcolor(p,_arg1,_arg2,_arg3);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_place_image(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    int  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    EXC_STUFF;

    if (argc != 6) {
        Tcl_SetResult(interp, "Wrong # args. PDF_place_image p image x y scale ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_place_image. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    if (Tcl_GetInt(interp, argv[2], &_arg1) != TCL_OK) {
        Tcl_SetResult(interp,
	    "Type error in argument 2 of PDF_place_image.", TCL_STATIC);
	return TCL_ERROR;
    }
    _arg2 = (float) atof(argv[3]);
    _arg3 = (float) atof(argv[4]);
    _arg4 = (float) atof(argv[5]);

    try {     PDF_place_image(p,_arg1,_arg2,_arg3,_arg4);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_open_image(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {

    int  _result;
    PDF *p;
    char * _arg1;
    char * _arg2;
    char * _arg3;
    long  _arg4;
    int  _arg5;
    int  _arg6;
    int  _arg7;
    int  _arg8;
    char * _arg9;
    int len = 0;
    char *res;
    EXC_STUFF;

    if (objc != 11) {
        Tcl_SetResult(interp, "Wrong # args. PDF_open_image p type source data length width height components bpc params", TCL_STATIC);
        return TCL_ERROR;
    }

    if ((res = Tcl_GetStringFromObj(objv[1], NULL)) == NULL) {
        Tcl_SetResult(interp, "Couldn't retrieve PDF pointer in PDF_open_image",
	    TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(res, (void **) &p, "_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_open_image. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, res, (char *) NULL);
        return TCL_ERROR;
    }

    if ((_arg1 = Tcl_GetStringFromObj(objv[2], &len)) == NULL) {
        Tcl_SetResult(interp,
	    "Couldn't retrieve type argument in PDF_open_image", TCL_STATIC);
        return TCL_ERROR;
    }
    if ((_arg2 = Tcl_GetStringFromObj(objv[3], &len)) == NULL) {
        Tcl_SetResult(interp,
	    "Couldn't retrieve source argument in PDF_open_image", TCL_STATIC);
        return TCL_ERROR;
    }

/* ByteArrays appeared only in Tcl 8.1 */

#if 10 * TCL_MAJOR_VERSION + TCL_MINOR_VERSION >= 81

    if ((_arg3 = (char *) Tcl_GetByteArrayFromObj(objv[4], &len)) == NULL) {

#else /* Tcl 8.0 */

    if ((_arg3 = (char *) Tcl_GetStringFromObj(objv[4], &len)) == NULL) {
#endif /* Tcl 8.0 */

        Tcl_SetResult(interp,
	    "Couldn't retrieve data argument in PDF_open_image", TCL_STATIC);
        return TCL_ERROR;
    }

    if (Tcl_GetLongFromObj(interp, objv[5], &_arg4) != TCL_OK) {
        Tcl_SetResult(interp,
	    "Type error in argument 5 of PDF_open_image.", TCL_STATIC);
	return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[6], &_arg5) != TCL_OK) {
        Tcl_SetResult(interp,
	    "Type error in argument 6 of PDF_open_image.", TCL_STATIC);
	return TCL_ERROR;
    }
    if (Tcl_GetIntFromObj(interp, objv[7], &_arg6) != TCL_OK) {
        Tcl_SetResult(interp,
	    "Type error in argument 7 of PDF_open_image.", TCL_STATIC);
	return TCL_ERROR;
    }
    if (Tcl_GetIntFromObj(interp, objv[8], &_arg7) != TCL_OK) {
        Tcl_SetResult(interp,
	    "Type error in argument 8 of PDF_open_image.", TCL_STATIC);
	return TCL_ERROR;
    }
    if (Tcl_GetIntFromObj(interp, objv[9], &_arg8) != TCL_OK) {
        Tcl_SetResult(interp,
	    "Type error in argument 9 of PDF_open_image.", TCL_STATIC);
	return TCL_ERROR;
    }
    if ((_arg9 = Tcl_GetStringFromObj(objv[10], &len)) == NULL) {
        Tcl_SetResult(interp,
	    "Couldn't retrieve argument 10 in PDF_open_image", TCL_STATIC);
        return TCL_ERROR;
    }

    try {     
	_result = (int)PDF_open_image(p,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7,_arg8,_arg9);
    } catch;

    sprintf(interp->result,"%ld", (long) _result);
    return TCL_OK;
}

static int
_wrap_PDF_close_image(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    int  _arg1;
    EXC_STUFF;

    if (argc != 3) {
        Tcl_SetResult(interp, "Wrong # args. PDF_close_image p image ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_close_image. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    if (Tcl_GetInt(interp, argv[2], &_arg1) != TCL_OK) {
        Tcl_SetResult(interp,
	    "Type error in argument 2 of PDF_close_image.", TCL_STATIC);
	return TCL_ERROR;
    }

    try {     PDF_close_image(p,_arg1);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_open_image_file(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    int  _result;
    PDF *p;
    char * _arg1;
    char * _arg2;
    char * _arg3;
    int  _arg4;
    EXC_STUFF;

    if (argc != 6) {
        Tcl_SetResult(interp, "Wrong # args. PDF_open_image_file p type filename stringparam intparam ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_open_image_file. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = argv[2];
    _arg2 = argv[3];
    _arg3 = argv[4];
    if (Tcl_GetInt(interp, argv[5], &_arg4) != TCL_OK) {
        Tcl_SetResult(interp,
	    "Type error in argument 5 of PDF_open_image_file.", TCL_STATIC);
	return TCL_ERROR;
    }

    try {     _result = (int)PDF_open_image_file(p,_arg1,_arg2,_arg3,_arg4);
    } catch;

    sprintf(interp->result,"%ld", (long) _result);
    return TCL_OK;
}

static int
_wrap_PDF_open_CCITT(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    int  _result;
    PDF *p;
    char * _arg1;
    int  _arg2;
    int  _arg3;
    int  _arg4;
    int  _arg5;
    int  _arg6;
    EXC_STUFF;

    if (argc != 8) {
        Tcl_SetResult(interp, "Wrong # args. PDF_open_CCITT p filename width height BitReverse K BlackIs1 ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_open_CCITT. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = argv[2];
    if (Tcl_GetInt(interp, argv[3], &_arg2) != TCL_OK) {
        Tcl_SetResult(interp,
	    "Type error in argument 3 of PDF_open_CCITT.", TCL_STATIC);
	return TCL_ERROR;
    }
    if (Tcl_GetInt(interp, argv[4], &_arg3) != TCL_OK) {
        Tcl_SetResult(interp,
	    "Type error in argument 4 of PDF_open_CCITT.", TCL_STATIC);
	return TCL_ERROR;
    }
    if (Tcl_GetInt(interp, argv[5], &_arg4) != TCL_OK) {
        Tcl_SetResult(interp,
	    "Type error in argument 5 of PDF_open_CCITT.", TCL_STATIC);
	return TCL_ERROR;
    }
    if (Tcl_GetInt(interp, argv[6], &_arg5) != TCL_OK) {
        Tcl_SetResult(interp,
	    "Type error in argument 6 of PDF_open_CCITT.", TCL_STATIC);
	return TCL_ERROR;
    }
    if (Tcl_GetInt(interp, argv[7], &_arg6) != TCL_OK) {
        Tcl_SetResult(interp,
	    "Type error in argument 7 of PDF_open_CCITT.", TCL_STATIC);
	return TCL_ERROR;
    }

    try {     _result = (int)PDF_open_CCITT(p,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6);
    } catch;

    sprintf(interp->result,"%ld", (long) _result);
    return TCL_OK;
}

static int
_wrap_PDF_add_bookmark(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
    int  _result;
    PDF *p;
    char * _arg1;
    int  _arg2;
    int  _arg3;
    char *res;
    int error;
    EXC_STUFF;

    if (objc != 5) {
        Tcl_SetResult(interp, "Wrong # args. PDF_add_bookmark p text parent open ", TCL_STATIC);
        return TCL_ERROR;
    }

    if ((res = Tcl_GetStringFromObj(objv[1], NULL)) == NULL) {
        Tcl_SetResult(interp, "Couldn't retrieve PDF pointer in PDF_add_bookmark", TCL_STATIC);
	return TCL_ERROR;
    }

    if (SWIG_GetPtr(res, (void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_add_bookmark. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, res, (char *) NULL);
        return TCL_ERROR;
    }

    if ((_arg1 = GetStringUnicodePDFChars(p, interp, objv[2], NULL)) == NULL) {
        Tcl_SetResult(interp, "Tcl internal string allocation failed in PDF_add_bookmark", TCL_STATIC);
	return TCL_ERROR;
    }

    if ((error = Tcl_GetIntFromObj(interp, objv[3], &_arg2)) != TCL_OK) {
        Tcl_SetResult(interp, "Couldn't retrieve font argument in PDF_add_bookmark", TCL_STATIC);
	return TCL_ERROR;
    }

    if ((error = Tcl_GetIntFromObj(interp, objv[4], &_arg3)) != TCL_OK) {
        Tcl_SetResult(interp, "Couldn't retrieve font argument in PDF_add_bookmark", TCL_STATIC);
	return TCL_ERROR;
    }

    try {     _result = (int)PDF_add_bookmark(p,_arg1,_arg2,_arg3);
    } catch;

    ReleaseStringPDFChars(_arg1);
    sprintf(interp->result,"%ld", (long) _result);
    return TCL_OK;
}

static int
_wrap_PDF_set_info(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
    PDF *p;
    char * _arg1;
    char * _arg2;
    char *res;
    int len;
    EXC_STUFF;

    if (objc != 4) {
        Tcl_SetResult(interp, "Wrong # args. PDF_set_info p key value ",TCL_STATIC);
        return TCL_ERROR;
    }

    if ((res = Tcl_GetStringFromObj(objv[1], NULL)) == NULL) {
        Tcl_SetResult(interp, "Couldn't retrieve PDF pointer", TCL_STATIC);
	return TCL_ERROR;
    }

    if (SWIG_GetPtr(res, (void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_set_info. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, res, (char *) NULL);
        return TCL_ERROR;
    }

    if ((_arg1 = Tcl_GetStringFromObj(objv[2], &len)) == NULL) {
        Tcl_SetResult(interp, "Couldn't retrieve key argument in PDF_set_info", TCL_STATIC);
	return TCL_ERROR;
    }

    if ((_arg2 = GetStringUnicodePDFChars(p, interp, objv[3], NULL)) == NULL) {
        Tcl_SetResult(interp, "Couldn't retrieve value argument in PDF_set_info", TCL_STATIC);
	return TCL_ERROR;
    }

    try {     PDF_set_info(p,_arg1,_arg2);
    } catch;

    ReleaseStringPDFChars(_arg2);
    return TCL_OK;
}

static int
_wrap_PDF_attach_file(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
    PDF *p;
    double  _arg1;
    double  _arg2;
    double  _arg3;
    double  _arg4;
    char * _arg5;
    char * _arg6;
    char * _arg7;
    char * _arg8;
    char * _arg9;
    char *res;
    EXC_STUFF;

    if (objc != 11) {
        Tcl_SetResult(interp, "Wrong # args. PDF_attach_file p llx lly urx ury filename description author mimetype icon ",TCL_STATIC);
        return TCL_ERROR;
    }

    if ((res = Tcl_GetStringFromObj(objv[1], NULL)) == NULL) {
        Tcl_SetResult(interp, "Couldn't retrieve PDF pointer", TCL_STATIC);
	return TCL_ERROR;
    }

    if (SWIG_GetPtr(res,(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_attach_file. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, res, (char *) NULL);
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[2], &_arg1) != TCL_OK) {
        Tcl_SetResult(interp, "Couldn't retrieve llx in PDF_attach_file", TCL_STATIC);
	return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[3], &_arg2) != TCL_OK) {
        Tcl_SetResult(interp, "Couldn't retrieve lly in PDF_attach_file", TCL_STATIC);
	return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[4], &_arg3) != TCL_OK) {
        Tcl_SetResult(interp, "Couldn't retrieve urx in PDF_attach_file", TCL_STATIC);
	return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[5], &_arg4) != TCL_OK) {
        Tcl_SetResult(interp, "Couldn't retrieve ury in PDF_attach_file", TCL_STATIC);
	return TCL_ERROR;
    }

    if ((_arg5 = Tcl_GetStringFromObj(objv[6], NULL)) == NULL) {
        Tcl_SetResult(interp, "Couldn't retrieve filename in PDF_attach_file", TCL_STATIC);
	return TCL_ERROR;
    }

    if ((_arg6 = GetStringUnicodePDFChars(p, interp, objv[7], NULL)) == NULL) {
        Tcl_SetResult(interp, "Couldn't retrieve description argument in PDF_attach_file", TCL_STATIC);
	return TCL_ERROR;
    }

    if ((_arg7 = GetStringUnicodePDFChars(p, interp, objv[8], NULL)) == NULL) {
        Tcl_SetResult(interp, "Couldn't retrieve author argument in PDF_attach_file", TCL_STATIC);
	return TCL_ERROR;
    }

    if ((_arg8 = Tcl_GetStringFromObj(objv[9], NULL)) == NULL) {
        Tcl_SetResult(interp, "Couldn't retrieve mimetype in PDF_attach_file", TCL_STATIC);
	return TCL_ERROR;
    }

    if ((_arg9 = Tcl_GetStringFromObj(objv[10], NULL)) == NULL) {
        Tcl_SetResult(interp, "Couldn't retrieve icon in PDF_attach_file", TCL_STATIC);
	return TCL_ERROR;
    }

    try {     PDF_attach_file(p, (float) _arg1, (float) _arg2,
		    (float) _arg3, (float) _arg4,_arg5,_arg6,_arg7,_arg8,_arg9);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_add_note(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
    PDF *p;
    double  _arg1;
    double  _arg2;
    double  _arg3;
    double  _arg4;
    char * _arg5;
    char * _arg6;
    char * _arg7;
    int  _arg8;
    char *res;
    EXC_STUFF;

    if (objc != 10) {
        Tcl_SetResult(interp, "Wrong # args. PDF_add_note p llx lly urx ury contents title icon open ",TCL_STATIC);
        return TCL_ERROR;
    }

    if ((res = Tcl_GetStringFromObj(objv[1], NULL)) == NULL) {
        Tcl_SetResult(interp, "Couldn't retrieve PDF pointer", TCL_STATIC);
	return TCL_ERROR;
    }

    if (SWIG_GetPtr(res, (void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_add_note. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, res, (char *) NULL);
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[2], &_arg1) != TCL_OK) {
        Tcl_SetResult(interp, "Couldn't retrieve llx in PDF_attach_file", TCL_STATIC);
	return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[3], &_arg2) != TCL_OK) {
        Tcl_SetResult(interp, "Couldn't retrieve lly in PDF_attach_file", TCL_STATIC);
	return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[4], &_arg3) != TCL_OK) {
        Tcl_SetResult(interp, "Couldn't retrieve urx in PDF_attach_file", TCL_STATIC);
	return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[5], &_arg4) != TCL_OK) {
        Tcl_SetResult(interp, "Couldn't retrieve ury in PDF_attach_file", TCL_STATIC);
	return TCL_ERROR;
    }

    if ((_arg5 = GetStringUnicodePDFChars(p, interp, objv[6], NULL)) == NULL) {
        Tcl_SetResult(interp, "Couldn't retrieve contents argument in PDF_add_note", TCL_STATIC);
	return TCL_ERROR;
    }

    if ((_arg6 = GetStringUnicodePDFChars(p, interp, objv[7], NULL)) == NULL) {
        Tcl_SetResult(interp, "Couldn't retrieve title argument in PDF_add_note", TCL_STATIC);
	return TCL_ERROR;
    }

    if ((_arg7 = Tcl_GetStringFromObj(objv[8], NULL)) == NULL) {
        Tcl_SetResult(interp, "Couldn't retrieve icon in PDF_add_note", TCL_STATIC);
	return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[9], &_arg8) != TCL_OK) {
        Tcl_SetResult(interp, "Couldn't retrieve open argument in PDF_add_note", TCL_STATIC);
	return TCL_ERROR;
    }

    try {     PDF_add_note(p, (float) _arg1, (float) _arg2, (float) _arg3,
    			(float) _arg4,_arg5,_arg6,_arg7,_arg8);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_add_pdflink(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    char * _arg5;
    int  _arg6;
    char * _arg7;
    EXC_STUFF;

    if (argc != 9) {
        Tcl_SetResult(interp, "Wrong # args. PDF_add_pdflink p llx lly urx ury filename page dest ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_add_pdflink. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = (float) atof(argv[2]);
    _arg2 = (float) atof(argv[3]);
    _arg3 = (float) atof(argv[4]);
    _arg4 = (float) atof(argv[5]);
    _arg5 = argv[6];
    if (Tcl_GetInt(interp, argv[7], &_arg6) != TCL_OK) {
        Tcl_SetResult(interp,
	    "Type error in argument 7 of PDF_add_pdflink.", TCL_STATIC);
	return TCL_ERROR;
    }
    _arg7 = argv[8];

    try {     PDF_add_pdflink(p,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_add_launchlink(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    char * _arg5;
    EXC_STUFF;

    if (argc != 7) {
        Tcl_SetResult(interp, "Wrong # args. PDF_add_launchlink p llx lly urx ury filename ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_add_launchlink. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = (float) atof(argv[2]);
    _arg2 = (float) atof(argv[3]);
    _arg3 = (float) atof(argv[4]);
    _arg4 = (float) atof(argv[5]);
    _arg5 = argv[6];

    try {     PDF_add_launchlink(p,_arg1,_arg2,_arg3,_arg4,_arg5);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_add_locallink(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    int  _arg5;
    char * _arg6;
    EXC_STUFF;

    if (argc != 8) {
        Tcl_SetResult(interp, "Wrong # args. PDF_add_locallink p llx lly urx ury page dest ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_add_locallink. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = (float) atof(argv[2]);
    _arg2 = (float) atof(argv[3]);
    _arg3 = (float) atof(argv[4]);
    _arg4 = (float) atof(argv[5]);
    if (Tcl_GetInt(interp, argv[6], &_arg5) != TCL_OK) {
        Tcl_SetResult(interp,
	    "Type error in argument 6 of PDF_add_locallink.", TCL_STATIC);
	return TCL_ERROR;
    }
    _arg6 = argv[7];

    try {     PDF_add_locallink(p,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_add_weblink(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    char * _arg5;
    EXC_STUFF;

    if (argc != 7) {
        Tcl_SetResult(interp, "Wrong # args. PDF_add_weblink p llx lly urx ury url ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_add_weblink. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = (float) atof(argv[2]);
    _arg2 = (float) atof(argv[3]);
    _arg3 = (float) atof(argv[4]);
    _arg4 = (float) atof(argv[5]);
    _arg5 = argv[6];

    try {     PDF_add_weblink(p,_arg1,_arg2,_arg3,_arg4,_arg5);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_set_border_style(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    char * _arg1;
    float  _arg2;
    EXC_STUFF;

    if (argc != 4) {
        Tcl_SetResult(interp, "Wrong # args. PDF_set_border_style p style width ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_set_border_style. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = argv[2];
    _arg2 = (float) atof(argv[3]);

    try {     PDF_set_border_style(p,_arg1,_arg2);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_set_border_color(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    EXC_STUFF;

    if (argc != 5) {
        Tcl_SetResult(interp, "Wrong # args. PDF_set_border_color p red green blue ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_set_border_color. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = (float) atof(argv[2]);
    _arg2 = (float) atof(argv[3]);
    _arg3 = (float) atof(argv[4]);

    try {     PDF_set_border_color(p,_arg1,_arg2,_arg3);
    } catch;

    return TCL_OK;
}

static int
_wrap_PDF_set_border_dash(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    PDF *p;
    float  _arg1;
    float  _arg2;
    EXC_STUFF;

    if (argc != 4) {
        Tcl_SetResult(interp, "Wrong # args. PDF_set_border_dash p w b ",TCL_STATIC);
        return TCL_ERROR;
    }

    if (SWIG_GetPtr(argv[1],(void **) &p,"_PDF_p")) {
        Tcl_SetResult(interp, "Type error in argument 1 of PDF_set_border_dash. Expected _PDF_p, received ", TCL_STATIC);
        Tcl_AppendResult(interp, argv[1], (char *) NULL);
        return TCL_ERROR;
    }

    _arg1 = (float) atof(argv[2]);
    _arg2 = (float) atof(argv[3]);

    try {     PDF_set_border_dash(p,_arg1,_arg2);
    } catch;

    return TCL_OK;
}

SWIGEXPORT(int,Pdflib_Init)(Tcl_Interp *interp)
{
    if (interp == 0) 
	return TCL_ERROR;

#ifdef USE_TCL_STUBS
    if (Tcl_InitStubs(interp, "8.2", 0) == NULL) {
 	return TCL_ERROR;
    }
#else
    if (Tcl_PkgRequire(interp, "Tcl", TCL_VERSION, 1) == NULL) {
 	return TCL_ERROR;
    }
#endif

    /* Boot the PDFlib core */
    PDF_boot();

    /* Tell Tcl which package we are going to define */
    Tcl_PkgProvide(interp, "pdflib", PDFLIB_VERSIONSTRING);

    Tcl_CreateCommand(interp, "PDF_new", _wrap_PDF_new, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_delete", _wrap_PDF_delete, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_open_file", _wrap_PDF_open_file, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_close", _wrap_PDF_close, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
     Tcl_CreateObjCommand(interp, "PDF_get_buffer", _wrap_PDF_get_buffer, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_begin_page", _wrap_PDF_begin_page, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_end_page", _wrap_PDF_end_page, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_set_parameter", _wrap_PDF_set_parameter, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_set_value", _wrap_PDF_set_value, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_get_value", _wrap_PDF_get_value, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_get_parameter", _wrap_PDF_get_parameter, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_findfont", _wrap_PDF_findfont, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_setfont", _wrap_PDF_setfont, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);

    Tcl_CreateObjCommand(interp, "PDF_show", _wrap_PDF_show, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateObjCommand(interp, "PDF_show_xy", _wrap_PDF_show_xy, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateObjCommand(interp, "PDF_continue_text", _wrap_PDF_continue_text, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_show_boxed", _wrap_PDF_show_boxed, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_set_text_pos", _wrap_PDF_set_text_pos, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);

    Tcl_CreateObjCommand(interp, "PDF_stringwidth", _wrap_PDF_stringwidth, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_setdash", _wrap_PDF_setdash, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateObjCommand(interp, "PDF_setpolydash", _wrap_PDF_setpolydash, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_setflat", _wrap_PDF_setflat, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_setlinejoin", _wrap_PDF_setlinejoin, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_setlinecap", _wrap_PDF_setlinecap, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_setmiterlimit", _wrap_PDF_setmiterlimit, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_setlinewidth", _wrap_PDF_setlinewidth, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_save", _wrap_PDF_save, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_restore", _wrap_PDF_restore, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_translate", _wrap_PDF_translate, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_scale", _wrap_PDF_scale, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_rotate", _wrap_PDF_rotate, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_skew", _wrap_PDF_skew, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_concat", _wrap_PDF_concat, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_moveto", _wrap_PDF_moveto, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_lineto", _wrap_PDF_lineto, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_curveto", _wrap_PDF_curveto, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_circle", _wrap_PDF_circle, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_arc", _wrap_PDF_arc, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_rect", _wrap_PDF_rect, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_closepath", _wrap_PDF_closepath, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_stroke", _wrap_PDF_stroke, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_closepath_stroke", _wrap_PDF_closepath_stroke, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_fill", _wrap_PDF_fill, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_fill_stroke", _wrap_PDF_fill_stroke, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_closepath_fill_stroke", _wrap_PDF_closepath_fill_stroke, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_endpath", _wrap_PDF_endpath, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_clip", _wrap_PDF_clip, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_setgray_fill", _wrap_PDF_setgray_fill, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_setgray_stroke", _wrap_PDF_setgray_stroke, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_setgray", _wrap_PDF_setgray, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_setrgbcolor_fill", _wrap_PDF_setrgbcolor_fill, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_setrgbcolor_stroke", _wrap_PDF_setrgbcolor_stroke, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_setrgbcolor", _wrap_PDF_setrgbcolor, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_place_image", _wrap_PDF_place_image, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);

    Tcl_CreateCommand(interp, "PDF_open_image_file", _wrap_PDF_open_image_file, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateObjCommand(interp, "PDF_open_image", _wrap_PDF_open_image, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_close_image", _wrap_PDF_close_image, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_open_CCITT", _wrap_PDF_open_CCITT, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateObjCommand(interp, "PDF_add_bookmark", _wrap_PDF_add_bookmark, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateObjCommand(interp, "PDF_set_info", _wrap_PDF_set_info, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateObjCommand(interp, "PDF_attach_file", _wrap_PDF_attach_file, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateObjCommand(interp, "PDF_add_note", _wrap_PDF_add_note, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_add_pdflink", _wrap_PDF_add_pdflink, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_add_launchlink", _wrap_PDF_add_launchlink, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_add_locallink", _wrap_PDF_add_locallink, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_add_weblink", _wrap_PDF_add_weblink, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_set_border_style", _wrap_PDF_set_border_style, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_set_border_color", _wrap_PDF_set_border_color, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "PDF_set_border_dash", _wrap_PDF_set_border_dash, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);

/*
 * These are the pointer type-equivalency mappings. 
 * (Used by the SWIG pointer type-checker).
 */
    SWIG_RegisterMapping("_signed_long","_long",0);
    SWIG_RegisterMapping("_struct_PDF_s","_PDF",0);
    SWIG_RegisterMapping("_long","_unsigned_long",0);
    SWIG_RegisterMapping("_long","_signed_long",0);
    SWIG_RegisterMapping("_PDF","_struct_PDF_s",0);
    SWIG_RegisterMapping("_unsigned_long","_long",0);
    SWIG_RegisterMapping("_signed_int","_int",0);
    SWIG_RegisterMapping("_unsigned_short","_short",0);
    SWIG_RegisterMapping("_signed_short","_short",0);
    SWIG_RegisterMapping("_unsigned_int","_int",0);
    SWIG_RegisterMapping("_short","_unsigned_short",0);
    SWIG_RegisterMapping("_short","_signed_short",0);
    SWIG_RegisterMapping("_int","_unsigned_int",0);
    SWIG_RegisterMapping("_int","_signed_int",0);
    return TCL_OK;
}
