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
 * JNI wrapper code for the PDFlib Java binding
 *
 */

#include <stdlib.h>
#include <string.h>
#include <jni.h>
#include <setjmp.h>

#include "pdflib.h"

/* thread-specific data */
typedef struct {
    jmp_buf     env;
    int         type;
    char        buffer[256];
    int         nativeunicode;
    jint        jdkversion;
    jmethodID   MID_String_getBytes;    /* cached method identifier */
} pdf_wrapper_data;

/* This is used in the wrapper functions for thread-safe exception handling */
#define EXC_STUFF pdf_wrapper_data *ex

/* Exception handling */
#define try     ex = (pdf_wrapper_data *) (PDF_get_opaque(p));  \
                if (setjmp(ex->env) == 0)

#define jthrow(type, msg) (*jenv)->ThrowNew(jenv,               \
            (*jenv)->FindClass(jenv, pdf_java_exceptions[type]), msg)

#define catch   else {                                  \
                    jthrow(ex->type, ex->buffer);       \
                }

static void
pdf_errorhandler(PDF *p, int type, const char* shortmsg)
{
    EXC_STUFF;

    if (!p)
        return;

    /* retrieve opaque pointer, clean up PDFlib internals */
    ex = (pdf_wrapper_data *) (PDF_get_opaque(p));

    /* ...and throw an exception */
    strcpy(ex->buffer, shortmsg);
    ex->type = type;
    longjmp(ex->env, 1);
}

/* Map PDFlib errors to Java exceptions */
static const char *pdf_java_exceptions[] = {
NULL,
"java/lang/OutOfMemoryError",           /* PDF_MemoryError    1 */
"java/io/IOException",                  /* PDF_IOError        2 */
"java/lang/IllegalArgumentException",   /* PDF_RuntimeError   3 */
"java/lang/IndexOutOfBoundsException",  /* PDF_IndexError     4 */
"java/lang/ClassCastException",         /* PDF_TypeError      5 */
"java/lang/ArithmeticException",        /* PDF_DivisionByZero 6 */
"java/lang/ArithmeticException",        /* PDF_OverflowError  7 */
"java/lang/RuntimeException",           /* PDF_SyntaxError    8 */
"java/lang/IllegalArgumentException",   /* PDF_ValueError     9 */
"java/lang/InternalError",              /* PDF_SystemError   10 */
"java/lang/UnknownError",               /* PDF_NonfatalError 11 */
"java/lang/UnknownError",               /* PDF_UnknownError  12 */
};

/*
 * Single-byte sequence (PDFDocEncoding): pick only low bytes from Unicode
 * and end with null byte.
 */

static char *
GetStringPDFChars(PDF *p, JNIEnv *jenv, jstring string)
{
    const jchar *unistring;
    char *result;
    size_t i, len;

    if (!string)
        return NULL;

    len = (size_t) (*jenv)->GetStringLength(jenv, string);

    if (len == (size_t) 0)
        return NULL;

    unistring = (*jenv)->GetStringChars(jenv, string, NULL);

    if (!unistring) {
        pdf_errorhandler(p,
            PDF_MemoryError, "JNI internal string allocation failed");
    }

    result = (char *) malloc(len + 1);

    if (!result) {
        pdf_errorhandler(p,
            PDF_MemoryError, "JNI internal string allocation failed");
    }

    /* pick the low-order bytes only */
    for (i = 0; i < len; i++) {
        result[i] = (char) unistring[i];
    }

    result[i] = '\0';   /* NULL-terminate */

    (*jenv)->ReleaseStringChars(jenv, string, unistring);

    return result;
}

/*
 * Utility function for converting JNI's UTF-8 chars to native C strings;
 * Based on S. Liang "The Java Native Interface" (Addison-Wesley).
 */

static char *
JNU_GetString_NativeChars(JNIEnv *jenv, pdf_wrapper_data *ex, jstring jstr)
{
    jbyteArray bytes = 0;
    jthrowable exc;
    char *result = NULL;

#ifdef JNI_VERSION_1_2  /* compile-time version test... */

    /* ...and run-time version test */
    if (ex->jdkversion == JNI_VERSION_1_2 && (*jenv)->EnsureLocalCapacity(jenv, 2) < 0) {
        return NULL;            /* out of memory error */
    }
#endif

    bytes = (*jenv)->CallObjectMethod(jenv, jstr, ex->MID_String_getBytes);
    exc = (*jenv)->ExceptionOccurred(jenv);

    if (!exc) {
        jint len = (*jenv)->GetArrayLength(jenv, bytes);
        result = (char *) malloc(len + 1);

        if (result == NULL) {
            jthrow(PDF_MemoryError, "JNI internal string allocation failed");
            (*jenv)->DeleteLocalRef(jenv, bytes);
            return NULL;
        }

        (*jenv)->GetByteArrayRegion(jenv, bytes, 0, len, (jbyte *) result);
        result[len] = 0;                /* NULL-terminate */

    } else {
        (*jenv)->DeleteLocalRef(jenv, exc);
    }

    (*jenv)->DeleteLocalRef(jenv, bytes);

    return result;
}

#define ReleaseStringPDFChars(jenv, string, chars) if (chars) free(chars);

/*
 * Unicode strings - wrap with BOM and double-null
 */

/* The Unicode byte order mark (BOM) signals Unicode strings */
#define PDF_BOM0                0376            /* '\xFE' */
#define PDF_BOM1                0377            /* '\xFF' */

static char *
GetStringUnicodePDFChars(PDF *p, JNIEnv *jenv, jstring string, int *lenP)
{
    const jchar *unistring;
    unsigned char *result, have_unicode = 0;
    size_t i, len;
    EXC_STUFF;

    if (!string) {
        if (lenP)
            *lenP = 0;
        return NULL;
    }

    len = (size_t) (*jenv)->GetStringLength(jenv, string);

    if (len == (size_t) 0) {
        if (lenP)
            *lenP = 0;
        return NULL;
    }

    ex = (pdf_wrapper_data *) (PDF_get_opaque(p));

    unistring = (*jenv)->GetStringChars(jenv, string, NULL);

    if (!unistring) {
        pdf_errorhandler(p,
            PDF_MemoryError, "JNI internal string allocation failed");
    }

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
        pdf_errorhandler(p,
            PDF_MemoryError, "JNI internal string allocation failed");
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
            result[i] = '\0';   /* NULL-terminate */
        } else {
            /* kludge: JNU_GetString_NativeChars mallocs itself */
            ReleaseStringPDFChars(jenv, NULL, result);

            /* not in Unicode mode, but Unicode supplied: produce ANSI (native) output */
            result = (unsigned char *) JNU_GetString_NativeChars(jenv, ex, string);
        }
    }

    (*jenv)->ReleaseStringChars(jenv, string, unistring);

    return (char *) result;
}

/* export the PDFlib routines to the shared library */
#ifdef __MWERKS__
#pragma export on
#endif

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1boot(JNIEnv *jenv, jclass jcls)
{
    char buffer[256];

    /* throws nothing from within the library */
    PDF_boot();

    /* Compare our version number with the PDFlib library's version */
    if (PDF_get_majorversion() != PDFLIB_MAJORVERSION ||
        PDF_get_minorversion() != PDFLIB_MINORVERSION) {
            sprintf(buffer,
                "PDFlib library version mismatch (expected V%d.%02d)\n",
                PDFLIB_MAJORVERSION, PDFLIB_MINORVERSION);
            jthrow(PDF_SystemError, buffer);
    }
}


JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1shutdown(JNIEnv *jenv, jclass jcls)
{
    /* throws nothing */
    PDF_shutdown();
}

JNIEXPORT jlong JNICALL
Java_com_pdflib_pdflib_PDF_1new(JNIEnv *jenv, jclass jcls)
{
    jlong _jresult;
    PDF *p;
    EXC_STUFF;
    char jdkname[64];
    jclass stringClass;

    if ((ex = malloc(sizeof(pdf_wrapper_data))) == NULL) {
        jthrow(PDF_MemoryError, "Couldn't initialize PDFlib");
     #ifdef __IBMC__
        _jresult.lo = 0;
        _jresult.hi = 0;
        return _jresult;
     #else
        return (jlong)0;
     #endif
    }

    ex->nativeunicode = 0;

    p = (PDF *)PDF_new2(pdf_errorhandler, NULL, NULL, NULL, (void *) ex);

    if (p) {
        ex->jdkversion = (*jenv)->GetVersion(jenv);
        sprintf(jdkname, "JDK %d.%d",
                (int) ((ex->jdkversion & 0xFF0000) >> 16),
                (int) (ex->jdkversion & 0xFF));
        PDF_set_parameter(p, "binding", jdkname);

        stringClass = (*jenv)->FindClass(jenv, "java/lang/String");

        if (stringClass == NULL)
            jthrow(PDF_SystemError, "Could't initialize PDFlib");

        ex->MID_String_getBytes =
            (*jenv)->GetMethodID(jenv, stringClass, "getBytes", "()[B");

        if (ex->MID_String_getBytes == NULL)
            jthrow(PDF_SystemError, "Could't initialize PDFlib");

    } else {
        jthrow(PDF_MemoryError, "Couldn't initialize PDFlib");
    }

    *(PDF **)&_jresult = p;
    return _jresult;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1delete(JNIEnv *jenv, jclass jcls, jlong jarg0)
{
    PDF *p;
    EXC_STUFF;

    p = *(PDF **)&jarg0;

    try {     PDF_delete(p);
    } catch;
}

JNIEXPORT jint JNICALL
Java_com_pdflib_pdflib_PDF_1open_1file(JNIEnv *jenv, jclass jcls, jlong jarg0, jstring jarg1)
{
    jint _jresult = -1;
    int  _result;
    PDF *p;
    EXC_STUFF;
    char *_arg1;

    p = *(PDF **)&jarg0;

    try {
        _arg1 = (char *)GetStringPDFChars(p, jenv, jarg1);

        _result = (int )PDF_open_file(p,_arg1);
    } catch;

    _jresult = (jint) _result;
    ReleaseStringPDFChars(jenv, jarg1, _arg1);
    return _jresult;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1close(JNIEnv *jenv, jclass jcls, jlong jarg0)
{
    PDF *p;
    EXC_STUFF;

    p = *(PDF **)&jarg0;

    try {     PDF_close(p);
    } catch;
}

JNIEXPORT jbyteArray JNICALL
Java_com_pdflib_pdflib_PDF_1get_1buffer(JNIEnv *jenv, jclass jcls, jlong jarg0) {
    jbyteArray jbuffer;
    const unsigned char *buffer;
    PDF *p;
    EXC_STUFF;
    long size;

    p = *(PDF**) &jarg0;

    try {
        buffer = (const unsigned char *) PDF_get_buffer(p, &size);
        jbuffer = (*jenv)->NewByteArray(jenv, (jsize) size);

        if (jbuffer == (jbyteArray) NULL) {
            pdf_errorhandler(p, PDF_MemoryError,
                "Couldn't allocate PDF output buffer");
        } else {

            (*jenv)->SetByteArrayRegion(jenv, jbuffer,
                        0, (jsize) size, (jbyte *) buffer);
        }
    } catch;

    return jbuffer;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1begin_1page(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1, jfloat jarg2)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;
    float  _arg2;

    p = *(PDF **)&jarg0;
    _arg1 = (float )jarg1;
    _arg2 = (float )jarg2;

    try {
        PDF_begin_page(p,_arg1,_arg2);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1end_1page(JNIEnv *jenv, jclass jcls, jlong jarg0)
{
    PDF *p;
    EXC_STUFF;

    p = *(PDF **)&jarg0;

    try {     PDF_end_page(p);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1set_1parameter(JNIEnv *jenv, jclass jcls, jlong jarg0, jstring jarg1, jstring jarg2)
{
    PDF *p;
    EXC_STUFF;
    char * _arg1;
    char * _arg2;

    p = *(PDF **)&jarg0;

    try {
        _arg1 = (char *)GetStringPDFChars(p, jenv, jarg1);
        _arg2 = (char *)GetStringPDFChars(p, jenv, jarg2);
        ex = (pdf_wrapper_data *) (PDF_get_opaque(p));

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

    ReleaseStringPDFChars(jenv, jarg1, _arg1);
    ReleaseStringPDFChars(jenv, jarg1, _arg2);
}

JNIEXPORT jstring JNICALL
Java_com_pdflib_pdflib_PDF_1get_1parameter(JNIEnv *jenv, jclass jcls, jlong jarg0, jstring jkey, jfloat jmod)
{
    jstring _jresult = 0;
    char * _result = NULL;
    char *key;
    float mod;
    PDF *p;
    EXC_STUFF;

    p = *(PDF **)&jarg0;
    key = (char *) GetStringPDFChars(p, jenv, jkey);
    mod = (float )jmod;

    try {     _result = (char *)PDF_get_parameter(p, key, mod);
    } catch;

    if(_result)
        _jresult = (jstring)(*jenv)->NewStringUTF(jenv, _result);
    return _jresult;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1set_1value(JNIEnv *jenv, jclass jcls, jlong jarg0, jstring jarg1, jfloat jarg2) {
    PDF *p;
    EXC_STUFF;
    char * _arg1;
    float  _arg2;

    p = *(PDF **)&jarg0;
    _arg1 = (char *) GetStringPDFChars(p, jenv, jarg1);
    _arg2 = (float )jarg2;

    try {     PDF_set_value(p, _arg1, _arg2);
    } catch;

    ReleaseStringPDFChars(jenv, jarg1, _arg1);
}

JNIEXPORT jfloat JNICALL
Java_com_pdflib_pdflib_PDF_1get_1value(JNIEnv *jenv, jclass jcls, jlong jarg0, jstring jarg1, jfloat jarg2) {
    jfloat _jresult = 0;
    float  _result;
    PDF *p;
    EXC_STUFF;
    char * _arg1;
    float  _arg2;

    p = *(PDF **)&jarg0;
    _arg1 = (char *) GetStringPDFChars(p, jenv, jarg1);
    _arg2 = (float )jarg2;

    try {     _result = (float )PDF_get_value(p, _arg1, _arg2);
    } catch;

    ReleaseStringPDFChars(jenv, jarg1, _arg1);
    _jresult = (jfloat) _result;
    return _jresult;
}

JNIEXPORT jint JNICALL
Java_com_pdflib_pdflib_PDF_1findfont(JNIEnv *jenv, jclass jcls, jlong jarg0, jstring jarg1, jstring jarg2, jint jarg3)
{
    jint _jresult = 0;
    int  _result;
    PDF *p;
    EXC_STUFF;
    char * _arg1;
    char * _arg2;
    int  _arg3;

    p = *(PDF **)&jarg0;

    try {
        _arg1 = (char *)GetStringPDFChars(p, jenv, jarg1);
        _arg2 = (char *)GetStringPDFChars(p, jenv, jarg2);
        _arg3 = (int )jarg3;

        _result = (int )PDF_findfont(p,_arg1,_arg2,_arg3);
    } catch;

    _jresult = (jint) _result;

    ReleaseStringPDFChars(jenv, jarg1, _arg1);
    ReleaseStringPDFChars(jenv, jarg2, _arg2);
    return _jresult;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1setfont(JNIEnv *jenv, jclass jcls, jlong jarg0, jint jarg1, jfloat jarg2)
{
    PDF *p;
    EXC_STUFF;
    int  _arg1;
    float  _arg2;

    p = *(PDF **)&jarg0;
    _arg1 = (int )jarg1;
    _arg2 = (float )jarg2;

    try {     PDF_setfont(p,_arg1,_arg2);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1show(JNIEnv *jenv, jclass jcls, jlong jarg0, jstring jarg1)
{
    PDF *p;
    EXC_STUFF;
    char * _arg1;
    int len;

    p = *(PDF **)&jarg0;

    try {
        _arg1 = (char *)GetStringUnicodePDFChars(p, jenv, jarg1, &len);

        PDF_show2(p, _arg1, len);
    } catch;

    ReleaseStringPDFChars(jenv, jarg1, _arg1);
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1show_1xy(JNIEnv *jenv, jclass jcls, jlong jarg0, jstring jarg1, jfloat jarg2, jfloat jarg3)
{
    PDF *p;
    EXC_STUFF;
    char * _arg1;
    float  _arg2;
    float  _arg3;
    int len;

    p = *(PDF **)&jarg0;

    try {
        _arg1 = (char *)GetStringUnicodePDFChars(p, jenv, jarg1, &len);
        _arg2 = (float )jarg2;
        _arg3 = (float )jarg3;

        PDF_show_xy2(p,_arg1, len, _arg2,_arg3);
    } catch;

    ReleaseStringPDFChars(jenv, jarg1, _arg1);
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1continue_1text(JNIEnv *jenv, jclass jcls, jlong jarg0, jstring jarg1)
{
    PDF *p;
    EXC_STUFF;
    char * _arg1;
    int len;

    p = *(PDF **)&jarg0;

    try {
        _arg1 = (char *)GetStringUnicodePDFChars(p, jenv, jarg1, &len);

        PDF_continue_text2(p,_arg1, len);
    } catch;

    ReleaseStringPDFChars(jenv, jarg1, _arg1);
}

JNIEXPORT jint JNICALL
Java_com_pdflib_pdflib_PDF_1show_1boxed(JNIEnv *jenv, jclass jcls, jlong jarg0, jstring jarg1, jfloat jarg2, jfloat jarg3, jfloat jarg4, jfloat jarg5, jstring jarg6, jstring jarg7) {
    int _result;
    PDF *p;
    EXC_STUFF;
    char * _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    float  _arg5;
    char * _arg6;
    char * _arg7;

    p = *(PDF **)&jarg0;

    try {
        _arg1 = (char *)GetStringPDFChars(p, jenv, jarg1);
        _arg2 = (float )jarg2;
        _arg3 = (float )jarg3;
        _arg4 = (float )jarg4;
        _arg5 = (float )jarg5;
        _arg6 = (char *)GetStringPDFChars(p, jenv, jarg6);
        _arg7 = (char *)GetStringPDFChars(p, jenv, jarg7);

        _result = PDF_show_boxed(p,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7);
    } catch;

    ReleaseStringPDFChars(jenv, jarg1, _arg1);
    ReleaseStringPDFChars(jenv, jarg6, _arg6);
    ReleaseStringPDFChars(jenv, jarg7, _arg7);

    return (jint) _result;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1set_1text_1pos(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1, jfloat jarg2)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;
    float  _arg2;

    p = *(PDF **)&jarg0;
    _arg1 = (float )jarg1;
    _arg2 = (float )jarg2;

    try {     PDF_set_text_pos(p,_arg1,_arg2);
    } catch;
}

JNIEXPORT jfloat JNICALL
Java_com_pdflib_pdflib_PDF_1stringwidth(JNIEnv *jenv, jclass jcls, jlong jarg0, jstring jarg1, jint jarg2, jfloat jarg3)
{
    jfloat _jresult = 0;
    float  _result;
    PDF *p;
    EXC_STUFF;
    char * _arg1;
    int  _arg2;
    float  _arg3;
    int len;

    p = *(PDF **)&jarg0;

    try {
        _arg1 = (char *)GetStringPDFChars(p, jenv, jarg1);
        len = (*jenv)->GetStringLength(jenv, jarg1);
        _arg2 = (int )jarg2;
        _arg3 = (float )jarg3;

        _result = (float )PDF_stringwidth2(p,_arg1, len, _arg2,_arg3);
    } catch;

    _jresult = (jfloat) _result;
    ReleaseStringPDFChars(jenv, jarg1, _arg1);
    return _jresult;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1setdash(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1, jfloat jarg2)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;
    float  _arg2;

    p = *(PDF **)&jarg0;
    _arg1 = (float )jarg1;
    _arg2 = (float )jarg2;

    try {     PDF_setdash(p,_arg1,_arg2);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1setpolydash(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloatArray jarg1)
{
    PDF *p;
    EXC_STUFF;
    float carray[MAX_DASH_LENGTH];
    jfloat* jarray;
    int i;
    jsize length;

    p = *(PDF **) &jarg0;
    length = (*jenv)->GetArrayLength(jenv, jarg1);

    try {
        if (length > MAX_DASH_LENGTH)
            length = MAX_DASH_LENGTH;

        jarray = (*jenv)->GetFloatArrayElements(jenv, jarg1, 0);

        for(i=0; i < length; i++)
            carray[i] = (float) jarray[i];

        PDF_setpolydash(p, carray, length);
        (*jenv)->ReleaseFloatArrayElements(jenv, jarg1, jarray, 0);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1setflat(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;

    p = *(PDF **)&jarg0;
    _arg1 = (float )jarg1;

    try {     PDF_setflat(p,_arg1);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1setlinejoin(JNIEnv *jenv, jclass jcls, jlong jarg0, jint jarg1)
{
    PDF *p;
    EXC_STUFF;
    int  _arg1;

    p = *(PDF **)&jarg0;
    _arg1 = (int )jarg1;

    try {     PDF_setlinejoin(p,_arg1);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1setlinecap(JNIEnv *jenv, jclass jcls, jlong jarg0, jint jarg1)
{
    PDF *p;
    EXC_STUFF;
    int  _arg1;

    p = *(PDF **)&jarg0;
    _arg1 = (int )jarg1;

    try {     PDF_setlinecap(p,_arg1);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1setmiterlimit(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;

    p = *(PDF **)&jarg0;
    _arg1 = (float )jarg1;

    try {     PDF_setmiterlimit(p,_arg1);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1setlinewidth(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;

    p = *(PDF **)&jarg0;
    _arg1 = (float )jarg1;

    try {     PDF_setlinewidth(p,_arg1);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1save(JNIEnv *jenv, jclass jcls, jlong jarg0)
{
    PDF *p;
    EXC_STUFF;

    p = *(PDF **)&jarg0;

    try {     PDF_save(p);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1restore(JNIEnv *jenv, jclass jcls, jlong jarg0)
{
    PDF *p;
    EXC_STUFF;

    p = *(PDF **)&jarg0;

    try {     PDF_restore(p);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1translate(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1, jfloat jarg2)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;
    float  _arg2;

    p = *(PDF **)&jarg0;
    _arg1 = (float )jarg1;
    _arg2 = (float )jarg2;

    try {     PDF_translate(p,_arg1,_arg2);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1scale(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1, jfloat jarg2)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;
    float  _arg2;

    p = *(PDF **)&jarg0;
    _arg1 = (float )jarg1;
    _arg2 = (float )jarg2;

    try {     PDF_scale(p,_arg1,_arg2);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1rotate(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;

    p = *(PDF **)&jarg0;
    _arg1 = (float )jarg1;

    try {     PDF_rotate(p,_arg1);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1skew(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1, jfloat jarg2)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;
    float  _arg2;

    p = *(PDF **)&jarg0;
    _arg1 = (float )jarg1;
    _arg2 = (float )jarg2;

    try {     PDF_skew(p,_arg1,_arg2);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1concat(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat a, jfloat b, jfloat c, jfloat d, jfloat e, jfloat f) {
    PDF *p;
    EXC_STUFF;

    p = *(PDF **)&jarg0;

    try {     PDF_concat(p, (float) a, (float) b, (float) c, (float) d,
                                (float) e, (float) f);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1moveto(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1, jfloat jarg2)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;
    float  _arg2;

    p = *(PDF **)&jarg0;
    _arg1 = (float )jarg1;
    _arg2 = (float )jarg2;

    try {     PDF_moveto(p,_arg1,_arg2);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1lineto(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1, jfloat jarg2)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;
    float  _arg2;

    p = *(PDF **)&jarg0;
    _arg1 = (float )jarg1;
    _arg2 = (float )jarg2;

    try {     PDF_lineto(p,_arg1,_arg2);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1curveto(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1, jfloat jarg2, jfloat jarg3, jfloat jarg4, jfloat jarg5, jfloat jarg6)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    float  _arg5;
    float  _arg6;

    p = *(PDF **)&jarg0;
    _arg1 = (float )jarg1;
    _arg2 = (float )jarg2;
    _arg3 = (float )jarg3;
    _arg4 = (float )jarg4;
    _arg5 = (float )jarg5;
    _arg6 = (float )jarg6;

    try {     PDF_curveto(p,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1circle(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1, jfloat jarg2, jfloat jarg3)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;
    float  _arg2;
    float  _arg3;

    p = *(PDF **)&jarg0;
    _arg1 = (float )jarg1;
    _arg2 = (float )jarg2;
    _arg3 = (float )jarg3;

    try {     PDF_circle(p,_arg1,_arg2,_arg3);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1arc(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1, jfloat jarg2, jfloat jarg3, jfloat jarg4, jfloat jarg5)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    float  _arg5;

    p = *(PDF **)&jarg0;
    _arg1 = (float )jarg1;
    _arg2 = (float )jarg2;
    _arg3 = (float )jarg3;
    _arg4 = (float )jarg4;
    _arg5 = (float )jarg5;

    try {     PDF_arc(p,_arg1,_arg2,_arg3,_arg4,_arg5);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1rect(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1, jfloat jarg2, jfloat jarg3, jfloat jarg4)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;

    p = *(PDF **)&jarg0;
    _arg1 = (float )jarg1;
    _arg2 = (float )jarg2;
    _arg3 = (float )jarg3;
    _arg4 = (float )jarg4;

    try {     PDF_rect(p,_arg1,_arg2,_arg3,_arg4);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1closepath(JNIEnv *jenv, jclass jcls, jlong jarg0)
{
    PDF *p;
    EXC_STUFF;

    p = *(PDF **)&jarg0;

    try {     PDF_closepath(p);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1stroke(JNIEnv *jenv, jclass jcls, jlong jarg0)
{
    PDF *p;
    EXC_STUFF;

    p = *(PDF **)&jarg0;

    try {     PDF_stroke(p);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1closepath_1stroke(JNIEnv *jenv, jclass jcls, jlong jarg0)
{
    PDF *p;
    EXC_STUFF;

    p = *(PDF **)&jarg0;

    try {     PDF_closepath_stroke(p);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1fill(JNIEnv *jenv, jclass jcls, jlong jarg0)
{
    PDF *p;
    EXC_STUFF;

    p = *(PDF **)&jarg0;

    try {     PDF_fill(p);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1fill_1stroke(JNIEnv *jenv, jclass jcls, jlong jarg0)
{
    PDF *p;
    EXC_STUFF;

    p = *(PDF **)&jarg0;

    try {     PDF_fill_stroke(p);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1closepath_1fill_1stroke(JNIEnv *jenv, jclass jcls, jlong jarg0)
{
    PDF *p;
    EXC_STUFF;

    p = *(PDF **)&jarg0;

    try {     PDF_closepath_fill_stroke(p);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1endpath(JNIEnv *jenv, jclass jcls, jlong jarg0)
{
    PDF *p;
    EXC_STUFF;

    p = *(PDF **)&jarg0;

    try {     PDF_endpath(p);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1clip(JNIEnv *jenv, jclass jcls, jlong jarg0)
{
    PDF *p;
    EXC_STUFF;

    p = *(PDF **)&jarg0;

    try {     PDF_clip(p);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1setgray_1fill(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;

    p = *(PDF **)&jarg0;
    _arg1 = (float )jarg1;

    try {     PDF_setgray_fill(p,_arg1);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1setgray_1stroke(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;

    p = *(PDF **)&jarg0;
    _arg1 = (float )jarg1;

    try {     PDF_setgray_stroke(p,_arg1);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1setgray(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;

    p = *(PDF **)&jarg0;
    _arg1 = (float )jarg1;

    try {     PDF_setgray(p,_arg1);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1setrgbcolor_1fill(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1, jfloat jarg2, jfloat jarg3)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;
    float  _arg2;
    float  _arg3;

    p = *(PDF **)&jarg0;
    _arg1 = (float )jarg1;
    _arg2 = (float )jarg2;
    _arg3 = (float )jarg3;

    try {     PDF_setrgbcolor_fill(p,_arg1,_arg2,_arg3);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1setrgbcolor_1stroke(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1, jfloat jarg2, jfloat jarg3)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;
    float  _arg2;
    float  _arg3;

    p = *(PDF **)&jarg0;
    _arg1 = (float )jarg1;
    _arg2 = (float )jarg2;
    _arg3 = (float )jarg3;

    try {     PDF_setrgbcolor_stroke(p,_arg1,_arg2,_arg3);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1setrgbcolor(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1, jfloat jarg2, jfloat jarg3)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;
    float  _arg2;
    float  _arg3;

    p = *(PDF **)&jarg0;
    _arg1 = (float )jarg1;
    _arg2 = (float )jarg2;
    _arg3 = (float )jarg3;

    try {     PDF_setrgbcolor(p,_arg1,_arg2,_arg3);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1place_1image(JNIEnv *jenv, jclass jcls, jlong jarg0, jint jarg1, jfloat jarg2, jfloat jarg3, jfloat jarg4)
{
    PDF *p;
    EXC_STUFF;
    int  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;

    p = *(PDF **)&jarg0;
    _arg1 = (int )jarg1;
    _arg2 = (float )jarg2;
    _arg3 = (float )jarg3;
    _arg4 = (float )jarg4;

    try {     PDF_place_image(p,_arg1,_arg2,_arg3,_arg4);
    } catch;
}

JNIEXPORT jint JNICALL
Java_com_pdflib_pdflib_PDF_1open_1image(JNIEnv *jenv, jclass jcls, jlong jarg0, jstring jarg1, jstring jarg2, jbyteArray jarg3, jlong jarg4, jint jarg5, jint jarg6, jint jarg7, jint jarg8, jstring jarg9)
{
    jint _jresult = 0;
    int  _result;
    PDF *p;
    EXC_STUFF;
    char * _arg1;
    char * _arg2;
    char * _arg3;
    long  _arg4;
    int  _arg5;
    int  _arg6;
    int  _arg7;
    int  _arg8;
    char * _arg9;

    p = *(PDF **)&jarg0;

    try {
        _arg3 = (char *) (*jenv)->GetByteArrayElements(jenv, jarg3, (jboolean *) NULL);
        _arg1 = (char *)GetStringPDFChars(p, jenv, jarg1);
        _arg2 = (char *)GetStringPDFChars(p, jenv, jarg2);
#ifndef __ILEC400__
  #ifdef __IBMC__
        _arg4 = (long )jarg4.lo;
  #else
        _arg4 = (long )jarg4;
  #endif
#else
        /* deal with the AS/400's idea of a jlong... */
        JavaL2I(jarg4, _arg4);
#endif
        _arg5 = (int )jarg5;
        _arg6 = (int )jarg6;
        _arg7 = (int )jarg7;
        _arg8 = (int )jarg8;
        _arg9 = (char *)GetStringPDFChars(p, jenv, jarg9);

        _result = (int )PDF_open_image(p,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7,_arg8,_arg9);
    } catch;

    _jresult = (jint) _result;
    ReleaseStringPDFChars(jenv, jarg1, _arg1);
    ReleaseStringPDFChars(jenv, jarg2, _arg2);
    (*jenv)->ReleaseByteArrayElements(jenv, jarg3, (jbyte*) _arg3, JNI_ABORT);
    ReleaseStringPDFChars(jenv, jarg9, _arg9);

    return _jresult;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1close_1image(JNIEnv *jenv, jclass jcls, jlong jarg0, jint jarg1)
{
    PDF *p;
    EXC_STUFF;
    int  _arg1;

    p = *(PDF **)&jarg0;
    _arg1 = (int )jarg1;

    try {     PDF_close_image(p,_arg1);
    } catch;
}

JNIEXPORT jint JNICALL
Java_com_pdflib_pdflib_PDF_1open_1image_1file(JNIEnv *jenv, jclass jcls, jlong jarg0, jstring jarg1, jstring jarg2, jstring jarg3, jint jarg4)
{
    jint _jresult = 0;
    int  _result;
    PDF *p;
    EXC_STUFF;
    char * _arg1;
    char * _arg2;
    char * _arg3;
    int  _arg4;

    p = *(PDF **)&jarg0;

    try {
        _arg1 = (char *)GetStringPDFChars(p, jenv, jarg1);
        _arg2 = (char *)GetStringPDFChars(p, jenv, jarg2);
        _arg3 = (char *)GetStringPDFChars(p, jenv, jarg3);
        _arg4 = (int )jarg4;

        _result = (int )PDF_open_image_file(p,_arg1,_arg2,_arg3,_arg4);
    } catch;

    _jresult = (jint) _result;
    ReleaseStringPDFChars(jenv, jarg1, _arg1);
    ReleaseStringPDFChars(jenv, jarg2, _arg2);
    ReleaseStringPDFChars(jenv, jarg3, _arg3);
    return _jresult;
}

JNIEXPORT jint JNICALL
Java_com_pdflib_pdflib_PDF_1open_1CCITT(JNIEnv *jenv, jclass jcls, jlong jarg0, jstring jarg1, jint jarg2, jint jarg3, jint jarg4, jint jarg5, jint jarg6)
{
    jint _jresult = 0;
    int  _result;
    PDF *p;
    EXC_STUFF;
    char * _arg1;
    int  _arg2;
    int  _arg3;
    int  _arg4;
    int  _arg5;
    int  _arg6;

    p = *(PDF **)&jarg0;

    try {
        _arg1 = (char *)GetStringPDFChars(p, jenv, jarg1);
        _arg2 = (int )jarg2;
        _arg3 = (int )jarg3;
        _arg4 = (int )jarg4;
        _arg5 = (int )jarg5;
        _arg6 = (int )jarg6;

        _result = (int )PDF_open_CCITT(p,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6);
    } catch;

    _jresult = (jint) _result;
    ReleaseStringPDFChars(jenv, jarg1, _arg1);
    return _jresult;
}

JNIEXPORT jint JNICALL
Java_com_pdflib_pdflib_PDF_1add_1bookmark(JNIEnv *jenv, jclass jcls, jlong jarg0, jstring jarg1, jint jarg2, jint jarg3)
{
    jint _jresult = 0;
    int  _result;
    PDF *p;
    EXC_STUFF;
    char * _arg1;
    int  _arg2;
    int  _arg3;

    p = *(PDF **)&jarg0;

    try {
        _arg1 = (char *)GetStringUnicodePDFChars(p, jenv, jarg1, NULL);
        _arg2 = (int )jarg2;
        _arg3 = (int )jarg3;

        _result = (int )PDF_add_bookmark(p,_arg1,_arg2,_arg3);
    } catch;

    _jresult = (jint) _result;
    ReleaseStringPDFChars(jenv, jarg1, _arg1);
    return _jresult;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1set_1info(JNIEnv *jenv, jclass jcls, jlong jarg0, jstring jarg1, jstring jarg2)
{
    PDF *p;
    EXC_STUFF;
    char * _arg1;
    char * _arg2;

    p = *(PDF **)&jarg0;

    try {
        _arg1 = (char *)GetStringPDFChars(p, jenv, jarg1);
        _arg2 = (char *)GetStringUnicodePDFChars(p, jenv, jarg2, NULL);

        PDF_set_info(p,_arg1,_arg2);
    } catch;

    ReleaseStringPDFChars(jenv, jarg1, _arg1);
    ReleaseStringPDFChars(jenv, jarg2, _arg2);
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1attach_1file(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1, jfloat jarg2, jfloat jarg3, jfloat jarg4, jstring jarg5, jstring jarg6, jstring jarg7, jstring jarg8, jstring jarg9)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    char * _arg5;
    char * _arg6;
    char * _arg7;
    char * _arg8;
    char * _arg9;

    p = *(PDF **)&jarg0;

    try {
        _arg1 = (float )jarg1;
        _arg2 = (float )jarg2;
        _arg3 = (float )jarg3;
        _arg4 = (float )jarg4;
        _arg5 = (char *)GetStringPDFChars(p, jenv, jarg5);
        _arg6 = (char *)GetStringUnicodePDFChars(p, jenv, jarg6, NULL);
        _arg7 = (char *)GetStringUnicodePDFChars(p, jenv, jarg7, NULL);
        _arg8 = (char *)GetStringPDFChars(p, jenv, jarg8);
        _arg9 = (char *)GetStringPDFChars(p, jenv, jarg9);

        PDF_attach_file(p,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7,_arg8,_arg9);
    } catch;

    ReleaseStringPDFChars(jenv, jarg5, _arg5);
    ReleaseStringPDFChars(jenv, jarg6, _arg6);
    ReleaseStringPDFChars(jenv, jarg7, _arg7);
    ReleaseStringPDFChars(jenv, jarg8, _arg8);
    ReleaseStringPDFChars(jenv, jarg9, _arg9);
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1add_1note(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1, jfloat jarg2, jfloat jarg3, jfloat jarg4, jstring jarg5, jstring jarg6, jstring jarg7, jint jarg8)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    char * _arg5;
    char * _arg6;
    char * _arg7;
    int  _arg8;

    p = *(PDF **)&jarg0;

    try {
        _arg1 = (float )jarg1;
        _arg2 = (float )jarg2;
        _arg3 = (float )jarg3;
        _arg4 = (float )jarg4;
        _arg5 = (char *)GetStringUnicodePDFChars(p, jenv, jarg5, NULL);
        _arg6 = (char *)GetStringUnicodePDFChars(p, jenv, jarg6, NULL);
        _arg7 = (char *)GetStringPDFChars(p, jenv, jarg7);
        _arg8 = (int )jarg8;

        PDF_add_note(p,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7,_arg8);
    } catch;

    ReleaseStringPDFChars(jenv, jarg5, _arg5);
    ReleaseStringPDFChars(jenv, jarg6, _arg6);
    ReleaseStringPDFChars(jenv, jarg7, _arg7);
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1add_1pdflink(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1, jfloat jarg2, jfloat jarg3, jfloat jarg4, jstring jarg5, jint jarg6, jstring jarg7)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    char * _arg5;
    int  _arg6;
    char * _arg7;

    p = *(PDF **)&jarg0;

    try {
        _arg1 = (float )jarg1;
        _arg2 = (float )jarg2;
        _arg3 = (float )jarg3;
        _arg4 = (float )jarg4;
        _arg5 = (char *)GetStringPDFChars(p, jenv, jarg5);
        _arg6 = (int )jarg6;
        _arg7 = (char *)GetStringPDFChars(p, jenv, jarg7);

        PDF_add_pdflink(p,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7);
    } catch;

    ReleaseStringPDFChars(jenv, jarg5, _arg5);
    ReleaseStringPDFChars(jenv, jarg7, _arg7);
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1add_1launchlink(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1, jfloat jarg2, jfloat jarg3, jfloat jarg4, jstring jarg5)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    char * _arg5;

    p = *(PDF **)&jarg0;

    try {
        _arg1 = (float )jarg1;
        _arg2 = (float )jarg2;
        _arg3 = (float )jarg3;
        _arg4 = (float )jarg4;
        _arg5 = (char *)GetStringPDFChars(p, jenv, jarg5);

        PDF_add_launchlink(p,_arg1,_arg2,_arg3,_arg4,_arg5);
    } catch;

    ReleaseStringPDFChars(jenv, jarg5, _arg5);
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1add_1locallink(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1, jfloat jarg2, jfloat jarg3, jfloat jarg4, jint jarg5, jstring jarg6)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    int  _arg5;
    char * _arg6;

    p = *(PDF **)&jarg0;

    try {
        _arg1 = (float )jarg1;
        _arg2 = (float )jarg2;
        _arg3 = (float )jarg3;
        _arg4 = (float )jarg4;
        _arg5 = (int )jarg5;
        _arg6 = (char *)GetStringPDFChars(p, jenv, jarg6);

        PDF_add_locallink(p,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6);
    } catch;

    ReleaseStringPDFChars(jenv, jarg6, _arg6);
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1add_1weblink(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1, jfloat jarg2, jfloat jarg3, jfloat jarg4, jstring jarg5)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;
    float  _arg2;
    float  _arg3;
    float  _arg4;
    char * _arg5;

    p = *(PDF **)&jarg0;

    try {
        _arg1 = (float )jarg1;
        _arg2 = (float )jarg2;
        _arg3 = (float )jarg3;
        _arg4 = (float )jarg4;
        _arg5 = (char *)GetStringPDFChars(p, jenv, jarg5);

        PDF_add_weblink(p,_arg1,_arg2,_arg3,_arg4,_arg5);
    } catch;

    ReleaseStringPDFChars(jenv, jarg5, _arg5);
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1set_1border_1style(JNIEnv *jenv, jclass jcls, jlong jarg0, jstring jarg1, jfloat jarg2)
{
    PDF *p;
    EXC_STUFF;
    char * _arg1;
    float  _arg2;

    p = *(PDF **)&jarg0;

    try {
        _arg1 = (char *)GetStringPDFChars(p, jenv, jarg1);
        _arg2 = (float )jarg2;

        PDF_set_border_style(p,_arg1,_arg2);
    } catch;

    ReleaseStringPDFChars(jenv, jarg1, _arg1);
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1set_1border_1color(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1, jfloat jarg2, jfloat jarg3)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;
    float  _arg2;
    float  _arg3;

    p = *(PDF **)&jarg0;
    _arg1 = (float )jarg1;
    _arg2 = (float )jarg2;
    _arg3 = (float )jarg3;

    try {     PDF_set_border_color(p,_arg1,_arg2,_arg3);
    } catch;
}

JNIEXPORT void JNICALL
Java_com_pdflib_pdflib_PDF_1set_1border_1dash(JNIEnv *jenv, jclass jcls, jlong jarg0, jfloat jarg1, jfloat jarg2)
{
    PDF *p;
    EXC_STUFF;
    float  _arg1;
    float  _arg2;

    p = *(PDF **)&jarg0;
    _arg1 = (float )jarg1;
    _arg2 = (float )jarg2;

    try {     PDF_set_border_dash(p,_arg1,_arg2);
    } catch;
}
