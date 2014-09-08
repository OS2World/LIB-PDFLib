/*---------------------------------------------------------------------------*
 |        PDFlib - A library for dynamically generating PDF files            |
 +---------------------------------------------------------------------------+
 |        Copyright (c) 1997-2000 Thomas Merz. All rights reserved.          |
 +---------------------------------------------------------------------------+
 |    This software is not in the public domain.  It is subject to the       |
 |    "Aladdin Free Public License".  See the file license.pdf for details.  |
 |    This license grants you the right to use and redistribute PDFlib       |
 |    under certain conditions. Among other things, the license requires     |
 |    that the copyright notice and this notice be preserved on all copies.  |
 |    This requirement extends to ports to other programming languages.      |
 |                                                                           |
 |    In short, you are allowed to develop and use PDFlib-based software     |
 |    as long as you don't sell it. Commercial use of PDFlib requires a      |
 |    commercial license which can be obtained from the author of PDFlib.    |
 |    Contact information can be found in the accompanying PDFlib manual.    |
 |    PDFlib is distributed with no warranty of any kind. Commercial users,  |
 |    however, will receive warranty and support statements in writing.      |
 *---------------------------------------------------------------------------*/

// pdflib.hpp
//
// C++ wrapper for PDFlib
//
//

#ifndef PDFLIB_HPP
#define PDFLIB_HPP

// Add your compiler here if it doesn't support ANSI C++ string handling
#ifdef __WATCOMC__
    #define PDF_BROKEN_STRINGS
#endif

// Add your compiler here if it doesn't support proper bool handling
#ifdef __WATCOMC__
    #define PDF_BROKEN_BOOL
#endif

// Add your compiler here if it requires "using namespace std" for
// proper handling of ANSI C++ strings
#ifdef _MSC_VER
#define PDF_NEED_NAMESPACE
#endif

#ifdef PDF_BROKEN_BOOL
    #define bool        int
#endif

// If ANSI C++ strings don't work we revert to plain old C char *
#ifndef PDF_BROKEN_STRINGS
    #include <string>

    // VC++ needs this to recognize the string class, but Watcom 10.6
    // and gcc don't understand it...
    #ifdef PDF_NEED_NAMESPACE
        using namespace std;
    #endif

    #define pdfstring   string
#else
    #define pdfstring   const char *
#endif

// We use PDF as a C++ class name, therefore hide the actual C struct
// name for PDFlib usage with C++.

#define PDF PDF_c
#include "pdflib.h"
#undef PDF


// The C++ class wrapper for PDFlib

class PDF {
public:
    PDF(errorproc errorhandler = NULL,
        void* (*allocproc) (PDF_c *p, size_t size, const char *caller) = NULL,
        void* (*reallocproc)
                (PDF_c *p, void *mem, size_t size, const char *caller) = NULL,
        void  (*freeproc)(PDF_c *p, void *mem) = NULL,
        void *opaque = NULL);
    ~PDF();

    void * get_opaque();
    static int get_majorversion();
    static int get_minorversion();

    // Overloaded generic open and close methods
    int  open(pdfstring filename);
    int  open(FILE *fp);
    void open(size_t (*writeproc)(PDF_c *p, void *data, size_t size));

    void close();

    const char *get_buffer(long *size);

    void begin_page(float width, float height);
    void end_page();
    void set_parameter(pdfstring key, pdfstring value);
    pdfstring get_parameter(pdfstring key, float mod);
    float get_value(pdfstring key, float mod);
    void set_value(pdfstring key, float value);
    void show(pdfstring text);

// If ANSI C++ strings are not available, we need the old-style C
// functions for dealing with strings with embedded null characters
#ifdef PDF_BROKEN_STRINGS
    void show2(pdfstring text, int len);
    void show_xy2(pdfstring text, int len, float x, float y);
    void continue_text2(pdfstring text, int len);
    float stringwidth2(pdfstring text, int len, int font, float size);
#endif
    void show_xy(pdfstring text, float x, float y);
    void continue_text(pdfstring text);
    int show_boxed(pdfstring text, float left, float top, float width, float height, pdfstring hmode, pdfstring reserved);
    void set_text_pos(float x, float y);

    float stringwidth(pdfstring text, int font, float size);

    int  findfont(pdfstring fontname, pdfstring encoding, int embed);
    void setfont(int font, float fontsize);
    void save();
    void restore();
    void translate(float tx, float ty);
    void scale(float sx, float sy);
    void rotate(float phi);
    void skew(float alpha, float beta);
    void concat(float a, float b, float c, float d, float e, float f);
    void setdash(float b, float w);
    void setpolydash(float *darray, int length);
    void setflat(float flat);
    void setlinejoin(int join);
    void setlinecap(int cap);
    void setmiterlimit(float miter);
    void setlinewidth(float width);
    void moveto(float x, float y);
    void lineto(float x, float y);
    void curveto(float x1, float y1, float x2, float y2, float x3, float y3);
    void circle(float x, float y, float r);
    void arc(float x, float y, float r, float alpha1, float alpha2);
    void rect(float x, float y, float width, float height);
    void closepath();
    void stroke();
    void closepath_stroke();
    void fill();
    void fill_stroke();
    void closepath_fill_stroke();
    void endpath();
    void clip();
    void setgray_fill(float g);
    void setgray_stroke(float g);
    void setgray(float g);
    void setrgbcolor_fill(float red, float green, float blue);
    void setrgbcolor_stroke(float red, float green, float blue);
    void setrgbcolor(float red, float green, float blue);
    void place_image(int image, float x, float y, float scale);
    int open_image(pdfstring type, pdfstring source, const char *data, long len,
        int width, int height, int components, int bpc, pdfstring params);
    int open_image_file(pdfstring type, pdfstring filename,
        pdfstring stringparam, int intparam);
    void close_image(int image);
    int open_CCITT(pdfstring filename, int width, int height,
        bool BitReverse, int K, bool BlackIs1);
    int add_bookmark(pdfstring text, int parent, bool open);
    void set_info(pdfstring key, pdfstring value);
    void attach_file(float llx, float lly, float urx, float ury,
        pdfstring filename, pdfstring description, pdfstring author,
        pdfstring mimetype, pdfstring icon);
    void add_note(float llx, float lly, float urx, float ury,
        pdfstring contents, pdfstring title, pdfstring icon, bool open);
    void add_pdflink(float llx, float lly, float urx, float ury,
        pdfstring filename, int page, pdfstring dest);
    void add_launchlink(float llx, float lly, float urx, float ury,
        pdfstring filename);
    void add_locallink(float llx, float lly, float urx, float ury,
        int page, pdfstring dest);
    void add_weblink(float llx, float lly, float urx, float ury, pdfstring url);

    void set_border_style(pdfstring style, float width);
    void set_border_color(float red, float green, float blue);
    void set_border_dash(float d1, float d2);

private:
    PDF_c *p;
};

#endif  // PDFLIB_HPP
