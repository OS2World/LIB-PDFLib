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

/* pdflib_java.c
 *
 * Wrapper code for the PDFlib Python binding
 *
 */

#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
#include <Python.h>
#ifdef __cplusplus
}
#endif

/* Definitions for Windows/Unix exporting */
#if defined(__WIN32__)
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

typedef struct {
  char  *name;
  PyObject *(*get_attr)(void);
  int (*set_attr)(PyObject *);
} swig_globalvar;

typedef struct swig_varlinkobject {
  PyObject_HEAD
  swig_globalvar **vars;
  int      nvars;
  int      maxvars;
} swig_varlinkobject;

/* ----------------------------------------------------------------------
   swig_varlink_repr()

   Function for python repr method
   ---------------------------------------------------------------------- */

static PyObject *
swig_varlink_repr(swig_varlinkobject *v)
{
  v = v;
  return PyString_FromString("<Global variables>");
}

/* ---------------------------------------------------------------------
   swig_varlink_print()

   Print out all of the global variable names
   --------------------------------------------------------------------- */

static int
swig_varlink_print(swig_varlinkobject *v, FILE *fp, int flags)
{

  int i = 0;
  flags = flags;
  fprintf(fp,"Global variables { ");
  while (v->vars[i]) {
    fprintf(fp,"%s", v->vars[i]->name);
    i++;
    if (v->vars[i]) fprintf(fp,", ");
  }
  fprintf(fp," }\n");
  return 0;
}

/* --------------------------------------------------------------------
   swig_varlink_getattr
 
   This function gets the value of a variable and returns it as a
   PyObject.   In our case, we'll be looking at the datatype and
   converting into a number or string
   -------------------------------------------------------------------- */

static PyObject *
swig_varlink_getattr(swig_varlinkobject *v, char *n)
{
  int i = 0;
  char temp[128];

  while (v->vars[i]) {
    if (strcmp(v->vars[i]->name,n) == 0) {
      return (*v->vars[i]->get_attr)();
    }
    i++;
  }
  sprintf(temp,"C global variable %s not found.", n);
  PyErr_SetString(PyExc_NameError,temp);
  return NULL;
}

/* -------------------------------------------------------------------
   swig_varlink_setattr()

   This function sets the value of a variable.
   ------------------------------------------------------------------- */

static int
swig_varlink_setattr(swig_varlinkobject *v, char *n, PyObject *p)
{
  char temp[128];
  int i = 0;
  while (v->vars[i]) {
    if (strcmp(v->vars[i]->name,n) == 0) {
      return (*v->vars[i]->set_attr)(p);
    }
    i++;
  }
  sprintf(temp,"C global variable %s not found.", n);
  PyErr_SetString(PyExc_NameError,temp);
  return 1;
}

statichere PyTypeObject varlinktype = {
/*  PyObject_HEAD_INIT(&PyType_Type)  Note : This doesn't work on some machines */
  PyObject_HEAD_INIT(0)              
  0,
  "varlink",                          /* Type name    */
  sizeof(swig_varlinkobject),         /* Basic size   */
  0,                                  /* Itemsize     */
  0,                                  /* Deallocator  */ 
  (printfunc) swig_varlink_print,     /* Print        */
  (getattrfunc) swig_varlink_getattr, /* get attr     */
  (setattrfunc) swig_varlink_setattr, /* Set attr     */
  0,                                  /* tp_compare   */
  (reprfunc) swig_varlink_repr,       /* tp_repr      */    
  0,                                  /* tp_as_number */
  0,                                  /* tp_as_mapping*/
  0,                                  /* tp_hash      */
};

/* Create a variable linking object for use later */

SWIGSTATIC PyObject *
SWIG_newvarlink(void)
{
  swig_varlinkobject *result = 0;
  result = PyMem_NEW(swig_varlinkobject,1);
  varlinktype.ob_type = &PyType_Type;    /* Patch varlinktype into a PyType */
  result->ob_type = &varlinktype;
  /*  _Py_NewReference(result);  Does not seem to be necessary */
  result->nvars = 0;
  result->maxvars = 64;
  result->vars = (swig_globalvar **) malloc(64*sizeof(swig_globalvar *));
  result->vars[0] = 0;
  result->ob_refcnt = 0;
  Py_XINCREF((PyObject *) result);
  return ((PyObject*) result);
}

#ifdef PDFLIB_UNUSED
SWIGSTATIC void
SWIG_addvarlink(PyObject *p, char *name,
	   PyObject *(*get_attr)(void), int (*set_attr)(PyObject *p))
{
  swig_varlinkobject *v;
  v= (swig_varlinkobject *) p;
	
  if (v->nvars >= v->maxvars -1) {
    v->maxvars = 2*v->maxvars;
    v->vars = (swig_globalvar **) realloc(v->vars,v->maxvars*sizeof(swig_globalvar *));
    if (v->vars == NULL) {
      fprintf(stderr,"SWIG : Fatal error in initializing Python module.\n");
      exit(1);
    }
  }
  v->vars[v->nvars] = (swig_globalvar *) malloc(sizeof(swig_globalvar));
  v->vars[v->nvars]->name = (char *) malloc(strlen(name)+1);
  strcpy(v->vars[v->nvars]->name,name);
  v->vars[v->nvars]->get_attr = get_attr;
  v->vars[v->nvars]->set_attr = set_attr;
  v->nvars++;
  v->vars[v->nvars] = 0;
}

#endif /* PDFLIB_UNUSED */

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

#include <setjmp.h>

#include "pdflib.h"

static void _SWIG_exception(int code, char *msg) {
  switch(code) {
  case PDF_MemoryError:
    PyErr_SetString(PyExc_MemoryError,msg);
    break;
  case PDF_IOError:
    PyErr_SetString(PyExc_IOError,msg);
    break;
  case PDF_RuntimeError:
    PyErr_SetString(PyExc_RuntimeError,msg);
    break;
  case PDF_IndexError:
    PyErr_SetString(PyExc_IndexError,msg);
    break;
  case PDF_TypeError:
    PyErr_SetString(PyExc_TypeError,msg);
    break;
  case PDF_DivisionByZero:
    PyErr_SetString(PyExc_ZeroDivisionError,msg);
    break;
  case PDF_OverflowError:
    PyErr_SetString(PyExc_OverflowError,msg);
    break;
  case PDF_SyntaxError:
    PyErr_SetString(PyExc_SyntaxError,msg);
    break;
  case PDF_ValueError:
    PyErr_SetString(PyExc_ValueError,msg);
    break;
  case PDF_SystemError:
    PyErr_SetString(PyExc_SystemError,msg);
    break;
  default:
    PyErr_SetString(PyExc_RuntimeError,msg);
    break;
  }
}

/* SWIG exception handling is currently not thread-safe! */
#define SWIG_exception() _SWIG_exception(exception_status, error_message); return NULL
static jmp_buf		exception_buffer;
static int		exception_status;
static char		error_message[256];

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

static PyObject *_wrap_PDF_new(PyObject *self, PyObject *args) {
    PDF * _result;
    char _ptemp[128];
    char versionbuf[32];

    if(!PyArg_ParseTuple(args,":PDF_new")) 
        return NULL;

    try {
	_result =(PDF *)PDF_new2(pdf_swig_errorhandler, NULL, NULL, NULL, NULL);
	if (_result) {
#ifdef PATCHLEVEL
	    sprintf(versionbuf, "Python %s", PATCHLEVEL);
#else
	    sprintf(versionbuf, "Python (unknown)");
#endif
	    PDF_set_parameter(_result, "binding", versionbuf);
	}
 }
    else {
	SWIG_exception();
    }

    SWIG_MakePtr(_ptemp, (char *) _result,"_PDF_p");
    return Py_BuildValue("s",_ptemp);
}

static PyObject *_wrap_PDF_delete(PyObject *self, PyObject *args) {
    PDF * _arg0;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"s:PDF_delete",&_argc0)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_delete. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_delete(_arg0);
 }
    else {
	SWIG_exception();
    }

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_open_file(PyObject *self, PyObject *args) {
    int  _result;
    PDF * _arg0;
    char * _arg1;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"ss:PDF_open_file",&_argc0,&_arg1)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_open_file. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     _result = (int )PDF_open_file(_arg0,_arg1);
 }
    else {
	SWIG_exception();
    }
    return Py_BuildValue("i",_result);
}

static PyObject *_wrap_PDF_close(PyObject *self, PyObject *args) {
    PDF * _arg0;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"s:PDF_close",&_argc0)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_close. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_close(_arg0);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_get_buffer(PyObject *self, PyObject *args) {
    char * _result;
    PDF * _arg0;
    char * _argc0 = 0;
    long size;

    if(!PyArg_ParseTuple(args,"s:PDF_get_buffer",&_argc0)) 
        return NULL;

    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,
	    	"Type error in argument 1 of PDF_get_buffer. Expected _PDF_p.");
	    return NULL;
        }
    }

    try {     _result = (char *)PDF_get_buffer(_arg0, &size);
    }
    else {
	SWIG_exception();
    }
    return Py_BuildValue("s#", _result, (int) size);
}

static PyObject *_wrap_PDF_begin_page(PyObject *self, PyObject *args) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sff:PDF_begin_page",&_argc0,&_arg1,&_arg2)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_begin_page. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_begin_page(_arg0,_arg1,_arg2);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_end_page(PyObject *self, PyObject *args) {
    PDF * _arg0;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"s:PDF_end_page",&_argc0)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_end_page. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_end_page(_arg0);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_set_parameter(PyObject *self, PyObject *args) {
    PDF * _arg0;
    char * _arg1;
    char * _arg2;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sss:PDF_set_parameter",&_argc0,&_arg1,&_arg2)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_set_parameter. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_set_parameter(_arg0,_arg1,_arg2);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_get_parameter(PyObject *self, PyObject *args) {
    PyObject * _resultobj;
    char * _result;
    PDF * _arg0;
    char * _arg1;
    float  _arg2;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"ssf:PDF_get_parameter",&_argc0,&_arg1,&_arg2)) 
        return NULL;

    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_get_parameter. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     _result = (char *)PDF_get_parameter(_arg0,_arg1,_arg2);
    }
    else {
	SWIG_exception();
    }
    _resultobj = Py_BuildValue("s", _result);
    return _resultobj;
}

static PyObject *_wrap_PDF_set_value(PyObject *self, PyObject *args) {
    PyObject * _resultobj;
    PDF * _arg0;
    char * _arg1;
    float  _arg2;
    char * _argc0 = 0;

    self = self;
    if(!PyArg_ParseTuple(args,"ssf:PDF_set_value",&_argc0,&_arg1,&_arg2)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_set_value. Expected _PDF_p.");
        return NULL;
        }
    }
{
    try {     PDF_set_value(_arg0,_arg1,_arg2);
 }
    else {
	SWIG_exception();
    }
}    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

static PyObject *_wrap_PDF_get_value(PyObject *self, PyObject *args) {
    PyObject * _resultobj;
    float  _result;
    PDF * _arg0;
    char * _arg1;
    float  _arg2;
    char * _argc0 = 0;

    self = self;
    if(!PyArg_ParseTuple(args,"ssf:PDF_get_value",&_argc0,&_arg1,&_arg2)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_get_value. Expected _PDF_p.");
        return NULL;
        }
    }
{
    try {     _result = (float )PDF_get_value(_arg0,_arg1,_arg2);
 }
    else {
	SWIG_exception();
    }
}    _resultobj = Py_BuildValue("f",_result);
    return _resultobj;
}

static PyObject *_wrap_PDF_findfont(PyObject *self, PyObject *args) {
    int  _result;
    PDF * _arg0;
    char * _arg1;
    char * _arg2;
    int  _arg3;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sssi:PDF_findfont",&_argc0,&_arg1,&_arg2,&_arg3)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_findfont. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     _result = (int )PDF_findfont(_arg0,_arg1,_arg2,_arg3);
 }
    else {
	SWIG_exception();
    }
    return Py_BuildValue("i",_result);
}

static PyObject *_wrap_PDF_setfont(PyObject *self, PyObject *args) {
    PDF * _arg0;
    int  _arg1;
    float  _arg2;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sif:PDF_setfont",&_argc0,&_arg1,&_arg2)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_setfont. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_setfont(_arg0,_arg1,_arg2);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_show(PyObject *self, PyObject *args) {
    PDF * _arg0;
    char * _arg1;
    char * _argc0 = 0;
    int len;

    if(!PyArg_ParseTuple(args,"ss:PDF_show",&_argc0,&_arg1)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_show. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     
	len = PyString_Size(PyTuple_GetItem(args, 1));
	PDF_show2(_arg0,_arg1, len);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_show_xy(PyObject *self, PyObject *args) {
    PDF * _arg0;
    char * _arg1;
    float  _arg2;
    float  _arg3;
    char * _argc0 = 0;
    int len;

    if(!PyArg_ParseTuple(args,"ssff:PDF_show_xy",&_argc0,&_arg1,&_arg2,&_arg3)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_show_xy. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     
	len = PyString_Size(PyTuple_GetItem(args, 1));
	PDF_show_xy2(_arg0,_arg1, len, _arg2,_arg3);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_continue_text(PyObject *self, PyObject *args) {
    PDF * _arg0;
    char * _arg1;
    char * _argc0 = 0;
    int len;

    if(!PyArg_ParseTuple(args,"ss:PDF_continue_text",&_argc0,&_arg1)) 
        return NULL;

    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_continue_text. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     
	len = PyString_Size(PyTuple_GetItem(args, 1));
	PDF_continue_text2(_arg0,_arg1, len);
    }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_show_boxed(PyObject *self, PyObject *args) {
    int  _result;
    PDF * _arg0;
    char * _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    float  _arg5;
    char * _arg6;
    char * _arg7;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"ssffffss:PDF_show_boxed",&_argc0,&_arg1,&_arg2,&_arg3,&_arg4,&_arg5,&_arg6,&_arg7)) 
        return NULL;

    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_show_boxed. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     _result = (int )PDF_show_boxed(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7);
    }
    else {
	SWIG_exception();
    }

    return Py_BuildValue("i",_result);
}

static PyObject *_wrap_PDF_set_text_pos(PyObject *self, PyObject *args) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sff:PDF_set_text_pos",&_argc0,&_arg1,&_arg2)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_set_text_pos. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_set_text_pos(_arg0,_arg1,_arg2);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_stringwidth(PyObject *self, PyObject *args) {
    float  _result;
    PDF * _arg0;
    char * _arg1;
    int  _arg2;
    float  _arg3;
    char * _argc0 = 0;
    int len;

    if(!PyArg_ParseTuple(args,"ssif:PDF_stringwidth",&_argc0,&_arg1,&_arg2,&_arg3)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_stringwidth. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     
	len = PyString_Size(PyTuple_GetItem(args, 1));
	_result = (float )PDF_stringwidth2(_arg0,_arg1, len, _arg2,_arg3);
 }
    else {
	SWIG_exception();
    }
    return Py_BuildValue("f",_result);
}

static PyObject *_wrap_PDF_setdash(PyObject *self, PyObject *args) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sff:PDF_setdash",&_argc0,&_arg1,&_arg2)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_setdash. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_setdash(_arg0,_arg1,_arg2);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_setpolydash(PyObject *self, PyObject *args) {
    PyObject *val;
    PDF *p;
    float fval, carray[MAX_DASH_LENGTH];
    int  length, i;
    char *_argc0 = NULL;
    PyObject *_argc1 = NULL;

    if(!PyArg_ParseTuple(args, "sO:PDF_setpolydash", &_argc0, &_argc1)) 
        return NULL;

    if (!_argc0 || SWIG_GetPtr(_argc0,(void **) &p,"_PDF_p")) {
	PyErr_SetString(PyExc_TypeError,
	    "Type error in argument 1 of PDF_setpolydash. Expected _PDF_p.");
	return NULL;
    }

    if (!_argc1 || !PyTuple_Check(_argc1)) {
	PyErr_SetString(PyExc_TypeError,
	"Type error in argument 2 of PDF_setpolydash. Expected float tuple.");
	return NULL;
    }

    length = PyTuple_Size(_argc1);

    if (length > MAX_DASH_LENGTH)
	length = MAX_DASH_LENGTH;

    for (i = 0; i < length; i++) {
	val = PyTuple_GetItem(_argc1, i);
	if (!PyArg_Parse(val, "f:PDF_setpolydash", &fval)) {
	    PyErr_SetString(PyExc_TypeError,
	    "Type error in argument 2 of PDF_setpolydash. Expected float.");
	    return NULL;
	}
	carray[i] = fval;
    }

    try {     PDF_setpolydash(p, carray, length);
    }
    else {
	SWIG_exception();
    }

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_setflat(PyObject *self, PyObject *args) {
    PDF * _arg0;
    float  _arg1;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sf:PDF_setflat",&_argc0,&_arg1)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_setflat. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_setflat(_arg0,_arg1);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_setlinejoin(PyObject *self, PyObject *args) {
    PDF * _arg0;
    int  _arg1;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"si:PDF_setlinejoin",&_argc0,&_arg1)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_setlinejoin. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_setlinejoin(_arg0,_arg1);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_setlinecap(PyObject *self, PyObject *args) {
    PDF * _arg0;
    int  _arg1;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"si:PDF_setlinecap",&_argc0,&_arg1)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_setlinecap. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_setlinecap(_arg0,_arg1);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_setmiterlimit(PyObject *self, PyObject *args) {
    PDF * _arg0;
    float  _arg1;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sf:PDF_setmiterlimit",&_argc0,&_arg1)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_setmiterlimit. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_setmiterlimit(_arg0,_arg1);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_setlinewidth(PyObject *self, PyObject *args) {
    PDF * _arg0;
    float  _arg1;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sf:PDF_setlinewidth",&_argc0,&_arg1)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_setlinewidth. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_setlinewidth(_arg0,_arg1);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_save(PyObject *self, PyObject *args) {
    PDF * _arg0;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"s:PDF_save",&_argc0)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_save. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_save(_arg0);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_restore(PyObject *self, PyObject *args) {
    PDF * _arg0;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"s:PDF_restore",&_argc0)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_restore. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_restore(_arg0);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_translate(PyObject *self, PyObject *args) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sff:PDF_translate",&_argc0,&_arg1,&_arg2)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_translate. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_translate(_arg0,_arg1,_arg2);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_scale(PyObject *self, PyObject *args) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sff:PDF_scale",&_argc0,&_arg1,&_arg2)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_scale. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_scale(_arg0,_arg1,_arg2);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_rotate(PyObject *self, PyObject *args) {
    PDF * _arg0;
    float  _arg1;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sf:PDF_rotate",&_argc0,&_arg1)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_rotate. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_rotate(_arg0,_arg1);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_skew(PyObject *self, PyObject *args) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sff:PDF_skew",&_argc0,&_arg1,&_arg2)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_skew. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_skew(_arg0,_arg1,_arg2);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_concat(PyObject *self, PyObject *args) {
    PyObject * _resultobj;
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    float  _arg5;
    float  _arg6;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sffffff:PDF_concat",
    	&_argc0,&_arg1,&_arg2,&_arg3,&_arg4,&_arg5,&_arg6)) 
        return NULL;

    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,
	    	"Type error in argument 1 of PDF_concat. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_concat(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6);
    }
    else {
	SWIG_exception();
    }

    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

static PyObject *_wrap_PDF_moveto(PyObject *self, PyObject *args) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sff:PDF_moveto",&_argc0,&_arg1,&_arg2)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_moveto. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_moveto(_arg0,_arg1,_arg2);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_lineto(PyObject *self, PyObject *args) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sff:PDF_lineto",&_argc0,&_arg1,&_arg2)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_lineto. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_lineto(_arg0,_arg1,_arg2);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_curveto(PyObject *self, PyObject *args) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    float  _arg5;
    float  _arg6;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sffffff:PDF_curveto",&_argc0,&_arg1,&_arg2,&_arg3,&_arg4,&_arg5,&_arg6)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_curveto. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_curveto(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_circle(PyObject *self, PyObject *args) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sfff:PDF_circle",&_argc0,&_arg1,&_arg2,&_arg3)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_circle. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_circle(_arg0,_arg1,_arg2,_arg3);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_arc(PyObject *self, PyObject *args) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    float  _arg5;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sfffff:PDF_arc",&_argc0,&_arg1,&_arg2,&_arg3,&_arg4,&_arg5)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_arc. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_arc(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_rect(PyObject *self, PyObject *args) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sffff:PDF_rect",&_argc0,&_arg1,&_arg2,&_arg3,&_arg4)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_rect. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_rect(_arg0,_arg1,_arg2,_arg3,_arg4);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_closepath(PyObject *self, PyObject *args) {
    PDF * _arg0;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"s:PDF_closepath",&_argc0)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_closepath. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_closepath(_arg0);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_stroke(PyObject *self, PyObject *args) {
    PDF * _arg0;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"s:PDF_stroke",&_argc0)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_stroke. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_stroke(_arg0);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_closepath_stroke(PyObject *self, PyObject *args) {
    PDF * _arg0;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"s:PDF_closepath_stroke",&_argc0)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_closepath_stroke. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_closepath_stroke(_arg0);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_fill(PyObject *self, PyObject *args) {
    PDF * _arg0;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"s:PDF_fill",&_argc0)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_fill. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_fill(_arg0);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_fill_stroke(PyObject *self, PyObject *args) {
    PDF * _arg0;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"s:PDF_fill_stroke",&_argc0)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_fill_stroke. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_fill_stroke(_arg0);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_closepath_fill_stroke(PyObject *self, PyObject *args) {
    PDF * _arg0;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"s:PDF_closepath_fill_stroke",&_argc0)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_closepath_fill_stroke. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_closepath_fill_stroke(_arg0);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_endpath(PyObject *self, PyObject *args) {
    PDF * _arg0;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"s:PDF_endpath",&_argc0)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_endpath. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_endpath(_arg0);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_clip(PyObject *self, PyObject *args) {
    PDF * _arg0;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"s:PDF_clip",&_argc0)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_clip. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_clip(_arg0);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_setgray_fill(PyObject *self, PyObject *args) {
    PDF * _arg0;
    float  _arg1;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sf:PDF_setgray_fill",&_argc0,&_arg1)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_setgray_fill. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_setgray_fill(_arg0,_arg1);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_setgray_stroke(PyObject *self, PyObject *args) {
    PDF * _arg0;
    float  _arg1;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sf:PDF_setgray_stroke",&_argc0,&_arg1)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_setgray_stroke. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_setgray_stroke(_arg0,_arg1);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_setgray(PyObject *self, PyObject *args) {
    PDF * _arg0;
    float  _arg1;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sf:PDF_setgray",&_argc0,&_arg1)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_setgray. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_setgray(_arg0,_arg1);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_setrgbcolor_fill(PyObject *self, PyObject *args) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sfff:PDF_setrgbcolor_fill",&_argc0,&_arg1,&_arg2,&_arg3)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_setrgbcolor_fill. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_setrgbcolor_fill(_arg0,_arg1,_arg2,_arg3);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_setrgbcolor_stroke(PyObject *self, PyObject *args) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sfff:PDF_setrgbcolor_stroke",&_argc0,&_arg1,&_arg2,&_arg3)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_setrgbcolor_stroke. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_setrgbcolor_stroke(_arg0,_arg1,_arg2,_arg3);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_setrgbcolor(PyObject *self, PyObject *args) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sfff:PDF_setrgbcolor",&_argc0,&_arg1,&_arg2,&_arg3)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_setrgbcolor. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_setrgbcolor(_arg0,_arg1,_arg2,_arg3);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_place_image(PyObject *self, PyObject *args) {
    PDF * _arg0;
    int  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sifff:PDF_place_image",&_argc0,&_arg1,&_arg2,&_arg3,&_arg4)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_place_image. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_place_image(_arg0,_arg1,_arg2,_arg3,_arg4);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_open_image(PyObject *self, PyObject *args) {
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
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"ssssliiiis:PDF_open_image",&_argc0,&_arg1,&_arg2,&_arg3,&_arg4,&_arg5,&_arg6,&_arg7,&_arg8,&_arg9)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_open_image. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     _result = (int )PDF_open_image(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7,_arg8,_arg9);
 }
    else {
	SWIG_exception();
    }
    return Py_BuildValue("i",_result);
}

static PyObject *_wrap_PDF_close_image(PyObject *self, PyObject *args) {
    PDF * _arg0;
    int  _arg1;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"si:PDF_close_image",&_argc0,&_arg1)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_close_image. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_close_image(_arg0,_arg1);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_open_image_file(PyObject *self, PyObject *args) {
    int  _result;
    PDF * _arg0;
    char * _arg1;
    char * _arg2;
    char * _arg3;
    int  _arg4;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"ssssi:PDF_open_image_file",&_argc0,&_arg1,&_arg2,&_arg3,&_arg4)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_open_image_file. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     _result = (int )PDF_open_image_file(_arg0,_arg1,_arg2,_arg3,_arg4);
 }
    else {
	SWIG_exception();
    }
    return Py_BuildValue("i",_result);
}

static PyObject *_wrap_PDF_open_CCITT(PyObject *self, PyObject *args) {
    int  _result;
    PDF * _arg0;
    char * _arg1;
    int  _arg2;
    int  _arg3;
    int  _arg4;
    int  _arg5;
    int  _arg6;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"ssiiiii:PDF_open_CCITT",&_argc0,&_arg1,&_arg2,&_arg3,&_arg4,&_arg5,&_arg6)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_open_CCITT. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     _result = (int )PDF_open_CCITT(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6);
 }
    else {
	SWIG_exception();
    }
    return Py_BuildValue("i",_result);
}

static PyObject *_wrap_PDF_add_bookmark(PyObject *self, PyObject *args) {
    int  _result;
    PDF * _arg0;
    char * _arg1;
    int  _arg2;
    int  _arg3;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"ssii:PDF_add_bookmark",&_argc0,&_arg1,&_arg2,&_arg3)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_add_bookmark. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     _result = (int )PDF_add_bookmark(_arg0,_arg1,_arg2,_arg3);
 }
    else {
	SWIG_exception();
    }
    return Py_BuildValue("i",_result);
}

static PyObject *_wrap_PDF_set_info(PyObject *self, PyObject *args) {
    PDF * _arg0;
    char * _arg1;
    char * _arg2;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sss:PDF_set_info",&_argc0,&_arg1,&_arg2)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_set_info. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_set_info(_arg0,_arg1,_arg2);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_attach_file(PyObject *self, PyObject *args) {
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
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sffffsssss:PDF_attach_file",&_argc0,&_arg1,&_arg2,&_arg3,&_arg4,&_arg5,&_arg6,&_arg7,&_arg8,&_arg9)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_attach_file. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_attach_file(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7,_arg8,_arg9);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_add_note(PyObject *self, PyObject *args) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    char * _arg5;
    char * _arg6;
    char * _arg7;
    int  _arg8;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sffffsssi:PDF_add_note",&_argc0,&_arg1,&_arg2,&_arg3,&_arg4,&_arg5,&_arg6,&_arg7,&_arg8)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_add_note. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_add_note(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7,_arg8);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_add_pdflink(PyObject *self, PyObject *args) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    char * _arg5;
    int  _arg6;
    char * _arg7;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sffffsis:PDF_add_pdflink",&_argc0,&_arg1,&_arg2,&_arg3,&_arg4,&_arg5,&_arg6,&_arg7)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_add_pdflink. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_add_pdflink(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_add_launchlink(PyObject *self, PyObject *args) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    char * _arg5;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sffffs:PDF_add_launchlink",&_argc0,&_arg1,&_arg2,&_arg3,&_arg4,&_arg5)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_add_launchlink. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_add_launchlink(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_add_locallink(PyObject *self, PyObject *args) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    int  _arg5;
    char * _arg6;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sffffis:PDF_add_locallink",&_argc0,&_arg1,&_arg2,&_arg3,&_arg4,&_arg5,&_arg6)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_add_locallink. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_add_locallink(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_add_weblink(PyObject *self, PyObject *args) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    char * _arg5;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sffffs:PDF_add_weblink",&_argc0,&_arg1,&_arg2,&_arg3,&_arg4,&_arg5)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_add_weblink. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_add_weblink(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_set_border_style(PyObject *self, PyObject *args) {
    PDF * _arg0;
    char * _arg1;
    float  _arg2;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"ssf:PDF_set_border_style",&_argc0,&_arg1,&_arg2)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_set_border_style. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_set_border_style(_arg0,_arg1,_arg2);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_set_border_color(PyObject *self, PyObject *args) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sfff:PDF_set_border_color",&_argc0,&_arg1,&_arg2,&_arg3)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_set_border_color. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_set_border_color(_arg0,_arg1,_arg2,_arg3);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *_wrap_PDF_set_border_dash(PyObject *self, PyObject *args) {
    PDF * _arg0;
    float  _arg1;
    float  _arg2;
    char * _argc0 = 0;

    if(!PyArg_ParseTuple(args,"sff:PDF_set_border_dash",&_argc0,&_arg1,&_arg2)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_PDF_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of PDF_set_border_dash. Expected _PDF_p.");
        return NULL;
        }
    }

    try {     PDF_set_border_dash(_arg0,_arg1,_arg2);
 }
    else {
	SWIG_exception();
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef pdflibMethods[] = {
	 { "PDF_set_border_dash", _wrap_PDF_set_border_dash, 1 },
	 { "PDF_set_border_color", _wrap_PDF_set_border_color, 1 },
	 { "PDF_set_border_style", _wrap_PDF_set_border_style, 1 },
	 { "PDF_add_weblink", _wrap_PDF_add_weblink, 1 },
	 { "PDF_add_locallink", _wrap_PDF_add_locallink, 1 },
	 { "PDF_add_launchlink", _wrap_PDF_add_launchlink, 1 },
	 { "PDF_add_pdflink", _wrap_PDF_add_pdflink, 1 },
	 { "PDF_add_note", _wrap_PDF_add_note, 1 },
	 { "PDF_attach_file", _wrap_PDF_attach_file, 1 },
	 { "PDF_set_info", _wrap_PDF_set_info, 1 },
	 { "PDF_add_bookmark", _wrap_PDF_add_bookmark, 1 },
	 { "PDF_open_CCITT", _wrap_PDF_open_CCITT, 1 },
	 { "PDF_close_image", _wrap_PDF_close_image, 1 },
	 { "PDF_open_image", _wrap_PDF_open_image, 1 },
	 { "PDF_open_image_file", _wrap_PDF_open_image_file, 1 },
	 { "PDF_place_image", _wrap_PDF_place_image, 1 },
	 { "PDF_setrgbcolor", _wrap_PDF_setrgbcolor, 1 },
	 { "PDF_setrgbcolor_stroke", _wrap_PDF_setrgbcolor_stroke, 1 },
	 { "PDF_setrgbcolor_fill", _wrap_PDF_setrgbcolor_fill, 1 },
	 { "PDF_setgray", _wrap_PDF_setgray, 1 },
	 { "PDF_setgray_stroke", _wrap_PDF_setgray_stroke, 1 },
	 { "PDF_setgray_fill", _wrap_PDF_setgray_fill, 1 },
	 { "PDF_clip", _wrap_PDF_clip, 1 },
	 { "PDF_endpath", _wrap_PDF_endpath, 1 },
	 { "PDF_closepath_fill_stroke", _wrap_PDF_closepath_fill_stroke, 1 },
	 { "PDF_fill_stroke", _wrap_PDF_fill_stroke, 1 },
	 { "PDF_fill", _wrap_PDF_fill, 1 },
	 { "PDF_closepath_stroke", _wrap_PDF_closepath_stroke, 1 },
	 { "PDF_stroke", _wrap_PDF_stroke, 1 },
	 { "PDF_closepath", _wrap_PDF_closepath, 1 },
	 { "PDF_rect", _wrap_PDF_rect, 1 },
	 { "PDF_arc", _wrap_PDF_arc, 1 },
	 { "PDF_circle", _wrap_PDF_circle, 1 },
	 { "PDF_curveto", _wrap_PDF_curveto, 1 },
	 { "PDF_lineto", _wrap_PDF_lineto, 1 },
	 { "PDF_moveto", _wrap_PDF_moveto, 1 },
	 { "PDF_skew", _wrap_PDF_skew, 1 },
	 { "PDF_concat", _wrap_PDF_concat, 1 },
	 { "PDF_rotate", _wrap_PDF_rotate, 1 },
	 { "PDF_scale", _wrap_PDF_scale, 1 },
	 { "PDF_translate", _wrap_PDF_translate, 1 },
	 { "PDF_restore", _wrap_PDF_restore, 1 },
	 { "PDF_save", _wrap_PDF_save, 1 },
	 { "PDF_setlinewidth", _wrap_PDF_setlinewidth, 1 },
	 { "PDF_setmiterlimit", _wrap_PDF_setmiterlimit, 1 },
	 { "PDF_setlinecap", _wrap_PDF_setlinecap, 1 },
	 { "PDF_setlinejoin", _wrap_PDF_setlinejoin, 1 },
	 { "PDF_setflat", _wrap_PDF_setflat, 1 },
	 { "PDF_setpolydash", _wrap_PDF_setpolydash, 1 },
	 { "PDF_setdash", _wrap_PDF_setdash, 1 },
	 { "PDF_stringwidth", _wrap_PDF_stringwidth, 1 },
	 { "PDF_set_text_pos", _wrap_PDF_set_text_pos, 1 },
	 { "PDF_continue_text", _wrap_PDF_continue_text, 1 },
	 { "PDF_show_boxed", _wrap_PDF_show_boxed, 1 },
	 { "PDF_show_xy", _wrap_PDF_show_xy, 1 },
	 { "PDF_show", _wrap_PDF_show, 1 },
	 { "PDF_setfont", _wrap_PDF_setfont, 1 },
	 { "PDF_findfont", _wrap_PDF_findfont, 1 },
	 { "PDF_set_parameter", _wrap_PDF_set_parameter, 1 },
	 { "PDF_get_parameter", _wrap_PDF_get_parameter, 1 },
	 { "PDF_get_value", _wrap_PDF_get_value, 1 },
	 { "PDF_set_value", _wrap_PDF_set_value, 1 },
	 { "PDF_end_page", _wrap_PDF_end_page, 1 },
	 { "PDF_begin_page", _wrap_PDF_begin_page, 1 },
	 { "PDF_get_buffer", _wrap_PDF_get_buffer, 1 },
	 { "PDF_close", _wrap_PDF_close, 1 },
	 { "PDF_open_file", _wrap_PDF_open_file, 1 },
	 { "PDF_delete", _wrap_PDF_delete, 1 },
	 { "PDF_new", _wrap_PDF_new, 1 },
	 { NULL, NULL }
};
static PyObject *SWIG_globals;
#ifdef __cplusplus
extern "C" 
#endif
SWIGEXPORT(void,initpdflib_py)() {
	 PyObject *m, *d;
	 SWIG_globals = SWIG_newvarlink();
	 m = Py_InitModule("pdflib_py", pdflibMethods);
	 d = PyModule_GetDict(m);

	/* Boot the PDFlib core */
	PDF_boot();
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
}
