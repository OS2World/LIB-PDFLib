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

// pdflib.cpp
//
// Implementation of C++ wrapper for PDFlib
//
//

#include "pdflib.hpp"

// If we don't have ANSI C++ strings we do it the old way.
// The Watcom also uses a cast operator instead of ANSI C++'s c_str() method
#ifdef PDF_BROKEN_STRINGS
    #define CHAR(s)	(char const *)(s)
#else
    #define CHAR(s)	s.c_str()
#endif

PDF::PDF(errorproc errorhandler,
    void* (*allocproc)(PDF_c *p, size_t size, const char *caller),
    void* (*reallocproc)(PDF_c *p, void *mem, size_t size, const char *caller),
    void  (*freeproc)(PDF_c *p, void *mem),
    void *opaque)
{
    PDF_boot();
    p = ::PDF_new2(errorhandler, allocproc, reallocproc, freeproc, opaque);

    if (p)
	PDF_set_parameter(p, "binding", "C++");
}

PDF::~PDF()
{
    ::PDF_delete(p);
    ::PDF_shutdown();
}

void *
PDF::get_opaque()
{
    return ::PDF_get_opaque(p);
}

int
PDF::get_majorversion()
{
    return ::PDF_get_majorversion();
}

int
PDF::get_minorversion()
{
    return ::PDF_get_minorversion();
}

int
PDF::open(pdfstring filename)
{
    return ::PDF_open_file(p, CHAR(filename));
}

int
PDF::open(FILE *fp)
{
    return ::PDF_open_fp(p, fp);
}

void
PDF::open(size_t (*writeproc)(PDF_c *p, void *data, size_t size))
{
    ::PDF_open_mem(p, writeproc);
}

void
PDF::close()
{
    ::PDF_close(p);
}

const char *
PDF::get_buffer(long *size)
{
    return ::PDF_get_buffer(p, size);
}

void
PDF::begin_page(float width, float height)
{
    ::PDF_begin_page(p, width, height);
}

void
PDF::end_page()
{
    ::PDF_end_page(p);
}

void
PDF::set_parameter(pdfstring key, pdfstring value)
{
    ::PDF_set_parameter(p, CHAR(key), CHAR(value));
}

pdfstring
PDF::get_parameter(pdfstring key, float mod)
{
    return ::PDF_get_parameter(p, CHAR(key), mod);
}

float
PDF::get_value(pdfstring key, float mod)
{
    return ::PDF_get_value(p, CHAR(key), mod);
}

void
PDF::set_value(pdfstring key, float value)
{
    ::PDF_set_value(p, CHAR(key), value);
}

#ifdef PDF_BROKEN_STRINGS

void
PDF::show2(pdfstring text, int len)
{
    ::PDF_show2(p, CHAR(text), len);
}

void
PDF::show_xy2(pdfstring text, int len, float x, float y)
{
    ::PDF_show_xy2(p, CHAR(text), len, x, y);
}

void
PDF::continue_text2(pdfstring text, int len)
{
    ::PDF_continue_text2(p, CHAR(text), len);
}

float
PDF::stringwidth2(pdfstring text, int len, int font, float size)
{
    return ::PDF_stringwidth2(p, CHAR(text), len, font, size);
}

#endif // PDF_BROKEN_STRINGS

void
PDF::show(pdfstring text)
{
#ifdef PDF_BROKEN_STRINGS
    ::PDF_show(p, CHAR(text));
#else
    ::PDF_show2(p, text.c_str(), (int) text.size());
#endif
}

void
PDF::show_xy(pdfstring text, float x, float y)
{
#ifdef PDF_BROKEN_STRINGS
    ::PDF_show_xy(p, CHAR(text), x, y);
#else
    ::PDF_show_xy2(p, text.c_str(), (int) text.size(), x, y);
#endif
}

void
PDF::continue_text(pdfstring text)
{
#ifdef PDF_BROKEN_STRINGS
    ::PDF_continue_text(p, CHAR(text));
#else
    ::PDF_continue_text2(p, text.c_str(), (int) text.size());
#endif
}

int
PDF::show_boxed(pdfstring text, float left, float top, float width, float height, pdfstring hmode, pdfstring reserved)
{
    return ::PDF_show_boxed(p, CHAR(text), left, top, width, height, CHAR(hmode), CHAR(reserved));
}

void
PDF::set_text_pos(float x, float y)
{
    ::PDF_set_text_pos(p, x, y);
}

float
PDF::stringwidth(pdfstring text, int font, float size)
{
#ifdef PDF_BROKEN_STRINGS
    return ::PDF_stringwidth(p, CHAR(text), font, size);
#else
    return ::PDF_stringwidth2(p, text.c_str(), (int) text.size(), font, size);
#endif
}

int
PDF::findfont(pdfstring fontname, pdfstring encoding, int embed)
{
    return ::PDF_findfont(p, CHAR(fontname), CHAR(encoding), embed);
}

void
PDF::setfont(int font, float fontsize)
{
    ::PDF_setfont(p, font, fontsize);
}

void
PDF::save()
{
    ::PDF_save(p);
}

void
PDF::restore()
{
    ::PDF_restore(p);
}

void
PDF::translate(float tx, float ty)
{
    ::PDF_translate(p, tx, ty);
}

void
PDF::scale(float sx, float sy)
{
    ::PDF_scale(p, sx, sy);
}

void
PDF::rotate(float phi)
{
    ::PDF_rotate(p, phi);
}

void
PDF::skew(float alpha, float beta)
{
    ::PDF_skew(p, alpha, beta);
}

void
PDF::concat(float a, float b, float c, float d, float e, float f)
{
    ::PDF_concat(p, a, b, c, d, e, f);
}

void
PDF::setdash(float b, float w)
{
    ::PDF_setdash(p, b, w);
}

void
PDF::setpolydash(float *darray, int length)
{
    ::PDF_setpolydash(p, darray, length);
}

void
PDF::setflat(float flat)
{
    ::PDF_setflat(p, flat);
}

void
PDF::setlinejoin(int join)
{
    ::PDF_setlinejoin(p, join);
}

void
PDF::setlinecap(int cap)
{
    ::PDF_setlinecap(p, cap);
}

void
PDF::setmiterlimit(float miter)
{
    ::PDF_setmiterlimit(p, miter);
}

void
PDF::setlinewidth(float width)
{
    ::PDF_setlinewidth(p, width);
}

void
PDF::moveto(float x, float y)
{
    ::PDF_moveto(p, x, y);
}

void
PDF::lineto(float x, float y)
{
    ::PDF_lineto(p, x, y);
}

void
PDF::curveto(float x1, float y1, float x2, float y2, float x3, float y3)
{
    ::PDF_curveto(p, x1, y1, x2, y2, x3, y3);
}

void
PDF::circle(float x, float y, float r)
{
    ::PDF_circle(p, x, y, r);
}

void
PDF::arc(float x, float y, float r, float alpha1, float alpha2)
{
    ::PDF_arc(p, x, y, r, alpha1, alpha2);
}

void
PDF::rect(float x, float y, float width, float height)
{
    ::PDF_rect(p, x, y, width, height);
}

void
PDF::closepath()
{
    ::PDF_closepath(p);
}

void
PDF::stroke()
{
    ::PDF_stroke(p);
}

void
PDF::closepath_stroke()
{
    ::PDF_closepath_stroke(p);
}

void
PDF::fill()
{
    ::PDF_fill(p);
}

void
PDF::fill_stroke()
{
    ::PDF_fill_stroke(p);
}

void
PDF::closepath_fill_stroke()
{
    ::PDF_closepath_fill_stroke(p);
}

void
PDF::endpath()
{
    ::PDF_endpath(p);
}

void
PDF::clip()
{
    ::PDF_clip(p);
}

void
PDF::setgray_fill(float g)
{
    ::PDF_setgray_fill(p, g);
}

void
PDF::setgray_stroke(float g)
{
    ::PDF_setgray_stroke(p, g);
}

void
PDF::setgray(float g)
{
    ::PDF_setgray(p, g);
}

void
PDF::setrgbcolor_fill(float red, float green, float blue)
{
    ::PDF_setrgbcolor_fill(p, red, green, blue);
}

void
PDF::setrgbcolor_stroke(float red, float green, float blue)
{
    ::PDF_setrgbcolor_stroke(p, red, green, blue);
}

void
PDF::setrgbcolor(float red, float green, float blue)
{
    ::PDF_setrgbcolor(p, red, green, blue);
}

void
PDF::place_image(int image, float x, float y, float scale)
{
    ::PDF_place_image(p, image, x, y, scale);
}

int
PDF::open_image(pdfstring type, pdfstring source, const char *data,
    long len, int width, int height, int components, int bpc, pdfstring params)
{
    return ::PDF_open_image(p, CHAR(type), CHAR(source), data, len,
    	width, height, components, bpc, CHAR(params));
}

void
PDF::close_image(int image)
{
    ::PDF_close_image(p, image);
}

int
PDF::open_image_file(pdfstring type, pdfstring filename,
    pdfstring stringparam, int intparam)
{
    return ::PDF_open_image_file(p, CHAR(type), CHAR(filename),
    	CHAR(stringparam), intparam);
}

int
PDF::open_CCITT(pdfstring filename, int width, int height, bool BitReverse, int K, bool BlackIs1)
{
    return ::PDF_open_CCITT(p, CHAR(filename), width, height, BitReverse, K, BlackIs1);
}

int
PDF::add_bookmark(pdfstring text, int parent, bool open)
{
    return ::PDF_add_bookmark(p, CHAR(text), parent, open);
}

void
PDF::set_info(pdfstring key, pdfstring value)
{
    ::PDF_set_info(p, CHAR(key), CHAR(value));
}

void
PDF::attach_file(float llx, float lly, float urx, float ury, pdfstring filename, pdfstring description, pdfstring author, pdfstring mimetype, pdfstring icon)
{
    ::PDF_attach_file(p, llx, lly, urx, ury, CHAR(filename), CHAR(description),
    		CHAR(author), CHAR(mimetype), CHAR(icon));
}

void
PDF::add_note(float llx, float lly, float urx, float ury, pdfstring contents, pdfstring title, pdfstring icon, bool open)
{
    ::PDF_add_note(p, llx, lly, urx, ury, CHAR(contents),
    	CHAR(title), CHAR(icon), open);
}

void
PDF::add_pdflink(float llx, float lly, float urx, float ury, pdfstring filename, int page, pdfstring dest)
{
    ::PDF_add_pdflink(p, llx, lly, urx, ury, CHAR(filename), page, CHAR(dest));
}

void
PDF::add_launchlink(float llx, float lly, float urx, float ury, pdfstring filename)
{
    ::PDF_add_launchlink(p, llx, lly, urx, ury, CHAR(filename));
}

void
PDF::add_locallink(float llx, float lly, float urx, float ury, int page, pdfstring dest)
{
    ::PDF_add_locallink(p, llx, lly, urx, ury, page, CHAR(dest));
}

void
PDF::add_weblink(float llx, float lly, float urx, float ury, pdfstring url)
{
    ::PDF_add_weblink(p, llx, lly, urx, ury, CHAR(url));
}

void
PDF::set_border_style(pdfstring style, float width)
{
    ::PDF_set_border_style(p, CHAR(style), width);
}

void
PDF::set_border_color(float red, float green, float blue)
{
    ::PDF_set_border_color(p, red, green, blue);
}

void
PDF::set_border_dash(float d1, float d2)
{
    ::PDF_set_border_dash(p, d1, d2);
}
