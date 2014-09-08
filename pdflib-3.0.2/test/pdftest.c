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

/* pdftest.c
 *
 * Test bed and sample application for PDFlib
 */

/*
 * We would prefer to receive this from a central config file but
 * alas, CodeWarrior's support for predefined symbols is so clumsy...
 */

#if __POWERPC__ || __CFM68K__ || __MC68K__ || __OS2__
#define MAC

/* Undefine these to avoid warnings when testing without the libraries */
#define HAVE_LIBPNG
#define HAVE_LIBTIFF
#endif

#if defined(__INTEL__) && defined(__MWERKS__)
#define WIN32
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "pdflib.h"

/* Create a multi-threaded test program */
#if defined(WIN32) && defined(_MSC_VER) && defined(_MT)
#define PDF_WINTHREADS
#endif

/* Don't clutter output from many threads with messages */
#ifdef PDF_WINTHREADS

#define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <process.h>
#undef WIN32_LEAN_AND_MEAN

#define MESSAGE(m)              /* */
#define PDF_ENDTHREAD(val)      _endthreadex(val);

#else

#define MESSAGE(m)              fprintf(stderr, m)
#define PDF_ENDTHREAD(val)      /* */

#endif /* PDF_WINTHREADS */

/* ------------------------------------------------------------- */
static void
bookmarks(PDF *p)
{
#define UNICODEINFO    "\376\377\0001\000.\000A\0\0"
#define LEFT 50
#define FONTSIZE 24
#define LEAD ((int) (FONTSIZE * 1.5))

    unsigned char buf[64], tmp[64];
    int c, i, pos;
    float y=700;
    int level1, level2=0, level3=0, font;

    MESSAGE("Bookmark test...");

    font = PDF_findfont(p, "Helvetica", "host", 0);

    PDF_begin_page(p, a4_width, a4_height);

    PDF_setfont(p, font, FONTSIZE);

    PDF_show_xy(p, "The bookmarks for this page contain all", LEFT, y-=LEAD);
    PDF_show_xy(p, "Unicode characters.", LEFT, y-=LEAD);
    PDF_show_xy(p, "Depending on the available fonts,", LEFT, y-=LEAD);
    PDF_show_xy(p, "only a certain subset will be visible on", LEFT, y-=LEAD);
    PDF_show_xy(p, "your system.", LEFT, y-=LEAD);

    /* private Unicode info entry */
    PDF_set_info(p, "Revision", UNICODEINFO);

    /* Generate Unicode bookmarks */
    level1 = PDF_add_bookmark(p, "Unicode bookmarks", 0, 0);

    for (c = 0; c < 65535; c += 16) {
        if (c % 4096 == 0) {
            sprintf((char *) tmp, "U+%04X", c);
            level2 = PDF_add_bookmark(p, (char *) tmp, level1, 0);
        }
        if (c % 256 == 0) {
            sprintf((char *) tmp, "U+%04X", c);
            level3 = PDF_add_bookmark(p, (char *) tmp, level2, 0);
        }
        /* now comes the actual Unicode string with the BOM */
        sprintf((char *) tmp, "0x%04X: ", c);

        /* write the Unicode byte order mark */
        strcpy((char *) buf, "\376\377");

        for (pos = 0; tmp[pos/2]; /* */ ) {
            buf[2 + pos] = 0;
            pos++;
            buf[2 + pos] = tmp[pos/2];  /* construct Unicode string */
            pos++;
        }

        pos += 2;       /* account for the BOM */
        for (i = 0; i < 16; i++) {      /* continue filling buf with chars */
            buf[pos++] = (unsigned char) ((((c+i)) & 0xFF00) >> 8);
            buf[pos++] = (unsigned char) (((c+i)) & 0x00FF);
        }

        /* signal end of string with two null bytes */
        buf[pos++] = (unsigned char) 0;
        buf[pos] = (unsigned char) 0;

        (void) PDF_add_bookmark(p, (char *) buf, level3, 1);
    }

    PDF_end_page(p);

    MESSAGE("done\n");

#undef FONTSIZE
#undef LEFT
#undef UNICODEINFO
#undef LEAD
}

/* ------------------------------------------------------------- */
static void
gif_image(PDF *p)
{
    int image;

#define GIFFILE         "pdflib.gif"

    MESSAGE("GIF test...");

    if ((image = PDF_open_image_file(p, "gif", GIFFILE, "", 0)) == -1) {
        fprintf(stderr, "Error: Couldn't analyze GIF image %s.\n", GIFFILE);
        return;
    }

    PDF_begin_page(p, PDF_get_value(p, "imagewidth", (float) image),
                        PDF_get_value(p, "imageheight", (float) image));

    (void) PDF_add_bookmark(p, "GIF image", 0, 1);

    PDF_place_image(p, image, (float) 0.0, (float) 0.0, (float) 1.0);

    PDF_end_page(p);

    MESSAGE("done\n");

#undef GIFFILE
}

/* ------------------------------------------------------------- */
static void
png_image(PDF *p)
{
    int image;

#define PNGFILE         "pdflib.png"

    MESSAGE("PNG test...");

    if ((image = PDF_open_image_file(p, "png", PNGFILE, "", 0)) == -1) {
        fprintf(stderr, "Couldn't analyze PNG image %s.\n", PNGFILE);
        return;
    }

    PDF_begin_page(p, PDF_get_value(p, "imagewidth", (float) image),
                        PDF_get_value(p, "imageheight", (float) image));
    (void) PDF_add_bookmark(p, "PNG image", 0, 1);

    PDF_place_image(p, image, (float) 0.0, (float) 0.0, (float) 1.0);

    PDF_end_page(p);

    MESSAGE("done\n");

#undef PNGFILE
}

/* ------------------------------------------------------------- */
static void
tiff_image(PDF *p)
{
    int image;

#define TIFFFILE        "acroweb_j.tif"

    MESSAGE("TIFF test...");

    if ((image = PDF_open_image_file(p, "tiff", TIFFFILE, "", 0)) == -1) {
        fprintf(stderr, "Couldn't analyze TIFF image %s.\n", TIFFFILE);
        return;
    }

    PDF_begin_page(p, PDF_get_value(p, "imagewidth", (float) image),
                        PDF_get_value(p, "imageheight", (float) image));
    (void) PDF_add_bookmark(p, "TIFF image", 0, 1);

    PDF_place_image(p, image, (float) 0.0, (float) 0.0, (float) 1.0);

    PDF_end_page(p);

    MESSAGE("done\n");

#undef TIFFFILE
}

/* ------------------------------------------------------------- */
static void
jpeg_image(PDF *p)
{
    int image;
    float       scale;

#define JPEGFILE        "nesrin.jpg"

    MESSAGE("JPEG test...");

    if ((image = PDF_open_image_file(p, "jpeg", JPEGFILE, "", 0)) == -1) {
        fprintf(stderr, "Error: Couldn't analyze JPEG image %s.\n", JPEGFILE);
        return;
    }

    PDF_begin_page(p, a4_width, a4_height);
    (void) PDF_add_bookmark(p, "JPEG image", 0, 1);

    /* ----------------- first image ------------------- */
    /* fit image to page width */
    scale = (float) a4_width/PDF_get_value(p, "imagewidth", (float) image);
    PDF_place_image(p, image, (float) 0.0,
        a4_height - PDF_get_value(p, "imageheight", (float) image) * scale, scale);

    /* ----------------- second image ------------------- */
    scale = (float) 0.5;
    PDF_save(p);
    PDF_rotate(p, (float) 90.0);
    PDF_place_image(p, image, (float) 0.0, (float) (-1 * a4_width), scale);
    PDF_restore(p);

    /* ----------------- third image ------------------- */
    scale = (float) 0.25;
    PDF_save(p);
    PDF_rotate(p, (float) 45.0);
    PDF_place_image(p, image, (float) 200.0, (float) 0.0, scale);
    PDF_restore(p);

    PDF_end_page(p);

    MESSAGE("done\n");

#undef JPEGFILE
}

/* ------------------------------------------------------------- */
static void
character_table(PDF *p)
{
    char        text[50];
    int         i, j, font;
    float       x, y;

#define LEFT            50
#define TOP             700
#define FONTSIZE        16
#define FONTNAME        "Times-Roman"

#define ENCODING        "host"

    MESSAGE("Character encoding test...");

    font = PDF_findfont(p, FONTNAME, ENCODING, 1);

    PDF_begin_page(p, a4_width, a4_height);

    (void) PDF_add_bookmark(p, "Host Encoding", 0, 1);

    PDF_setfont(p, font, 2*FONTSIZE);
    PDF_show_xy(p, "Host Encoding", LEFT, TOP+2*FONTSIZE);
    PDF_show(p, " (");
    PDF_show(p, PDF_get_parameter(p, "fontencoding", (float) font));
    PDF_show(p, " )");

    PDF_setfont(p, font, FONTSIZE);
    text[1] = 0;

    y = TOP;
    for (i = 2; i < 16; i++) {
        y -=  2*FONTSIZE;
        x = LEFT;
        for (j = 0; j < 16; j++) {
            text[0] = (char) (i*16 + j);
            PDF_show_xy(p, text, x, y);
            x += 2*FONTSIZE;
        }
    }

    PDF_end_page(p);

    MESSAGE("done\n");

#undef LEFT
#undef TOP
#undef FONTSIZE
#undef FONTNAME
}

/* ------------------------------------------------------------- */
static void
grid(PDF *p)
{
#define STEP            10
#define FONTSIZE        ((float) 10.0)
#define THICK           ((float) 1.0)
#define THIN            ((float) 0.01)

    char buf[10];
    float i;
    float width = a4_width, height = a4_height;
    int font;

    MESSAGE("Grid test...");

    PDF_begin_page(p, width, height);
    (void) PDF_add_bookmark(p, "Grid", 0, 1);

    PDF_setlinewidth(p, THIN);
    PDF_setdash(p, (float) 1.0, (float) 2.0);

    /* draw vertical lines */
    for (i = 0; i < width; i += STEP) {
        PDF_save(p);
        if ((int) i % 100 == 0)
            PDF_setlinewidth(p, THICK);
        if ((int) i % 50 == 0)
            PDF_setdash(p, (float) 0.0, (float) 0.0);
        PDF_moveto(p, i, 0);
        PDF_lineto(p, i, (float) height);
        PDF_stroke(p);
        PDF_restore(p);
    }

    /* draw horizontal lines */
    for (i = 0; i < height; i += STEP) {
        PDF_save(p);
        if ((int) i % 50 == 0)
            PDF_setdash(p, (float) 0.0, (float) 0.0);
        if ((int) i % 100 == 0)
            PDF_setlinewidth(p, THICK);
        PDF_moveto(p, 0, i);
        PDF_lineto(p, (float) width, i);
        PDF_stroke(p);
        PDF_restore(p);
    }

#define FONTNAME        "Helvetica-Bold"
#define DELTA   ((float) 9)
#define RADIUS  ((float) 12.0)

    font = PDF_findfont(p, FONTNAME, "host", 0);

    PDF_setfont(p, font, FONTSIZE);

    /* print captions */
    for (i = 100; i < width; i+= 100) {
        PDF_save(p);
        PDF_setgray_fill(p, (float) 1.0);
        PDF_circle(p, i, (float) 20.0, RADIUS);
        PDF_fill(p);
        PDF_restore(p);
        sprintf(buf, "%d", (int) i);
        PDF_show_xy(p, buf, i - DELTA, (float) 20.0 - FONTSIZE/3);
    }

    for (i = 100; i < height; i+= 100) {
        PDF_save(p);
        PDF_setgray_fill(p, (float) 1.0);
        PDF_circle(p, (float) 40.0, i, RADIUS);
        PDF_fill(p);
        PDF_restore(p);
        sprintf(buf, "%d", (int) i);
        PDF_show_xy(p, buf, (float) 40.0 - DELTA, i - FONTSIZE/3);
    }

    PDF_end_page(p);

    MESSAGE("done\n");

#undef STEP
#undef FONTSIZE
#undef DELTA
#undef RADIUS
#undef FONTNAME
}


/* ------------------------------------------------------------- */
static void
shaded_circle(PDF *p)
{
    int i;
    float gray = (float) 0.1, r;

    MESSAGE("Shaded circle test...");

    /* generate perceptual linear color blend */
    r = (float) pow(1.0/gray, 1.0/255.0);

    PDF_begin_page(p, a4_width, a4_height);
    (void) PDF_add_bookmark(p, "Shaded circle", 0, 1);

    for (i = 255; i >= 0; i--) {
        PDF_setrgbcolor_fill(p, gray, gray, (float) 1.0);
        PDF_circle(p, (float) 300.0, (float) 400.0, (float) i);
        PDF_fill(p);
        gray *= r;
    }
    PDF_end_page(p);

    MESSAGE("done\n");
}


/* ------------------------------------------------------------- */
static void
annotations(PDF *p)
{
#define FILENAME        "pdftest.c"
#define MIMETYPE        "text/plain"
#define NOTETEXT        "Hi! I'm a text annotation!"

/* Some greek and russian words for the unicode annotation test */
#define GREEKTEXT       "\376\377\003\233\003\237\003\223\003\237\003\243\0\0"
#define RUSSIANTEXT     "\376\377\004\037\004\036\004\024\004\040\004\043\004\023\004\020\0\0"

    MESSAGE("Annotation test...");

    PDF_begin_page(p, a4_width, a4_height);

    (void) PDF_add_bookmark(p, "Annotations", 0, 1);

    (void) PDF_attach_file(p, 100, 200, 150, 250, FILENAME,
                "C source file", "Thomas Merz", MIMETYPE, "graph");

    PDF_set_border_style(p, "dashed", (float) 3.0);
    PDF_set_border_color(p, (float) 0.0, (float) 0.0, (float) 1.0);
    PDF_set_border_dash(p, (float) 5.0, (float) 1.0);

    PDF_add_note(p, 200, 400, 300, 450, NOTETEXT, "Thomas Merz", "comment", 0);

    PDF_set_border_style(p, "dashed", (float) 4.0);
    PDF_set_border_color(p, (float) 1.0, (float) 0.0, (float) 0.0);
    PDF_set_border_dash(p, (float) 1.0, (float) 4.0);

    PDF_add_note(p, 200, 600, 550, 750, GREEKTEXT, "Greek annotation",
                "insert",1);

    PDF_add_note(p, 100, 500, 400, 650, RUSSIANTEXT,
                "Russian annotation", "paragraph", 1);

    PDF_add_launchlink(p, 300, 300, 400, 350, "../readme.txt");

    PDF_set_border_color(p, (float) 0.0, (float) 1.0, (float) 0.0);
    PDF_set_border_dash(p, (float) 3.0, (float) 2.0);

    PDF_add_pdflink(p, 400, 200, 500, 250, "../doc/PDFlib-manual.pdf", 2, "fitpage");

    PDF_set_border_color(p, (float) 1.0, (float) 1.0, (float) 0.0);
    PDF_set_border_dash(p, (float) 3.0, (float) 2.0);

    PDF_add_locallink(p, 500, 100, 550, 150, 8, "fitwidth");

    PDF_set_border_style(p, "solid", (float) 7.0);
    PDF_set_border_color(p, (float) 0.0, (float) 1.0, (float) 1.0);
    PDF_set_border_dash(p, (float) 0.0, (float) 0.0);

    PDF_add_weblink(p, 100, 150, 300, 250, "http://www.pdflib.com");

    PDF_end_page(p);

    MESSAGE("done\n");

#undef NOTETEXT
#undef FILENAME
#undef MIMETYPE
#undef RUSSIANTEXT
#undef GREEKTEXT
}

/* ------------------------------------------------------------- */
static void
centered_text(PDF *p)
{
    float       y;
    char        **cp;
    int         textfont, titlefont;
    char        *title = "Der Zauberlehrling";
    char        *subtitle ="Johann Wolfgang von Goethe";
#ifndef MAC
    /* Note that this sample won't display correctly on EBCDIC systems */
    char        *poem[] = {
                    "Hat der alte Hexenmeister",
                    "Sich doch einmal wegbegeben!",
                    "Und nun sollen seine Geister",
                    "Auch nach meinem Willen leben.",
                    "Seine Wort\222 und Werke",
                    "Merkt ich und den Brauch,",
                    "Und mit Geistesst\344rke",
                    "Tu ich Wunder auch.",
                    "Walle! walle",
                    "Manche Strecke,",
                    "Da\337, zum Zwecke,",
                    "Wasser flie\337e",
                    "Und mit reichem, vollem Schwalle",
                    "Zu dem Bade sich ergie\337e.",
                    NULL };
#else
/* We don't want to use metrics files for the sample, and therefore
 * stick to host encoding. For this reason the sample text (which
 * uses German special characters) must be platform-specific.
 * Both flavors only differ in the special characters.
 */
    char        *poem[] = {
                    "Hat der alte Hexenmeister",
                    "Sich doch einmal wegbegeben!",
                    "Und nun sollen seine Geister",
                    "Auch nach meinem Willen leben.",
                    "Seine Wort\325 und Werke",
                    "Merkt ich und den Brauch,",
                    "Und mit Geistesst\212rke",
                    "Tu ich Wunder auch.",
                    "Walle! walle",
                    "Manche Strecke,",
                    "Da\247, zum Zwecke,",
                    "Wasser flie\247e",
                    "Und mit reichem, vollem Schwalle",
                    "Zu dem Bade sich ergie\247e.",
                    NULL };
#endif

#define FONTSIZE        24
#define CENTER          290
#define TOP             750

    MESSAGE("Centered text test...");

    PDF_begin_page(p, a4_width, a4_height);
    (void) PDF_add_bookmark(p, "Centered text", 0, 1);

    textfont = PDF_findfont(p, "Times-Roman", ENCODING, 1);

    y = TOP;

    titlefont = PDF_findfont(p, "Helvetica-Bold", ENCODING, 1);
    PDF_setfont(p, titlefont, (float) (1.5 * FONTSIZE));
    PDF_show_boxed(p, title, CENTER, y, 0, 0, "center", "");
    y -= (float) (1.5 * FONTSIZE);

    PDF_setfont(p, titlefont, FONTSIZE/2);
    PDF_show_boxed(p, subtitle, CENTER, y, 0, 0, "center", "");
    y -= (float) (3.0 * FONTSIZE);

    PDF_setfont(p, textfont, FONTSIZE);

    for (cp = poem; *cp; cp++) {
        PDF_show_boxed(p, *cp, CENTER, y, 0, 0, "center", "");
        y -= (float) (1.5 * FONTSIZE);
    }

    PDF_end_page(p);

    MESSAGE("done\n");

#undef FONTSIZE
#undef RIGHT
#undef TOP
}

/* ------------------------------------------------------------- */
static void
memory_image(PDF *p)
{
    char        *buf, *bp;
    int         width, height, components;
    int         bpc;
    int         image;
    int         i, j;
    float       y, sx, sy;

#define LEFT            ((float) 50.0)

    width       = 256;
    height      = 1;
    bpc         = 8;
    components  = 3;

    MESSAGE("Memory image test...");

    sx = (float) 3.0;           /* desired horizontal scaling factor */
    sy = (float) 128.0;         /* desired height of one color band */

    buf = (char *) calloc((size_t) (width * height * components), 1);

    if (buf == NULL) {
        fprintf(stderr, "Not enough memory for memory image!\n");
        return;
    }

    /* now fill the buffer with fake image data (simple color ramp) */
    for (bp = buf, i=0; i<height; i++) {
        for (j=0; j<width; j++) {
            *bp++ = (char) (j % 256);   /* red blend */
            *bp++ = 0;
            *bp++ = 0;
        }
    }

    /*
     * In positioning the images below, we will have to compensate
     * for the scaling.
     */
    y = LEFT;

    image = PDF_open_image(p, "raw", "memory", buf, width*height*components,
        width, height, components, bpc, "");
    if (image == -1) {
        fprintf(stderr, "Not enough memory for memory image!\n");
        free(buf);
        return;
    }

    PDF_begin_page(p, 900, 600);
    (void) PDF_add_bookmark(p, "Memory image", 0, 1);

    /*
     * Since the image interface doesn't support non-proportional
     * scaling, we will use PDF_scale() instead to stretch the image.
     */
    PDF_scale(p, sx, sy);               /* stretch image */

    PDF_place_image(p, image, LEFT/sx, y/sy, (float) 1.0);

    for (bp = buf, i=0; i<height; i++) {
        for (j=0; j<width; j++) {
            *bp++ = 0;
            *bp++ = (char) (j % 256);   /* green blend */
            *bp++ = 0;
        }
    }

    y += height * sy;           /* position the image */

    image = PDF_open_image(p, "raw", "memory", buf, width*height*components,
        width, height, components, bpc, "");
    if (image == -1) {
        fprintf(stderr, "Not enough memory for memory image!\n");
        free(buf);
        PDF_end_page(p);
        return;
    }

    PDF_place_image(p, image, LEFT/sx, y/sy, (float) 1.0);

    for (bp = buf, i=0; i<height; i++) {
        for (j=0; j<width; j++) {
            *bp++ = 0;
            *bp++ = 0;
            *bp++ = (char) (j % 256);   /* blue blend */
        }
    }

    y += height * sy;           /* position the image */

    image = PDF_open_image(p, "raw", "memory", buf, width*height*components,
        width, height, components, bpc, "");
    if (image == -1) {
        fprintf(stderr, "Not enough memory for memory image!\n");
        free(buf);
        PDF_end_page(p);
        return;
    }

    PDF_place_image(p, image, LEFT/sx, y/sy, (float) 1.0);

    for (bp = buf, i=0; i<height; i++) {
        for (j=0; j<width; j++) {
            *bp++ = (char) (j % 256);   /* gray blend */
        }
    }

    y += height * sy;           /* position the image */

    components  = 1;            /* now a single component image */
    image = PDF_open_image(p, "raw", "memory", buf, width*height*components,
        width, height, components, bpc, "");
    if (image == -1) {
        fprintf(stderr, "Not enough memory for memory image!\n");
        free(buf);
        PDF_end_page(p);
        return;
    }

    PDF_place_image(p, image, LEFT/sx, y/sy, (float) 1.0);

    free(buf);
    PDF_end_page(p);

    MESSAGE("done\n");

#undef LEFT
}

/* ------------------------------------------------------------- */
static void
radial_structure(PDF *p)
{
    float alpha;

    MESSAGE("Radial structure test...");

    PDF_begin_page(p, a4_width, a4_height);
    (void) PDF_add_bookmark(p, "Radial structure", 0, 1);

    PDF_translate(p, (float) 300.0, (float) 400.0);
    PDF_setlinewidth(p, (float) 0.1);

    /* better solution: don't accumulate rounding errors */
    for (alpha = 0; alpha < 360; alpha++) {
        PDF_save(p);
        PDF_rotate(p, alpha);
        PDF_moveto(p, (float) 0.0, (float) 0.0);
        PDF_lineto(p, (float) 250.0, (float) 0.0);
        PDF_stroke(p);
        PDF_restore(p);
    }

    PDF_end_page(p);

    MESSAGE("done\n");
}

/* ------------------------------------------------------------- */
static void
random_data_graph(PDF *p)
{
    float x;

    MESSAGE("Random graph test...");

    PDF_begin_page(p, a4_width, a4_height);
    (void) PDF_add_bookmark(p, "Random graph", 0, 1);

#define STEP    ((float) 10.0)
#define MARGIN  ((float) 50.0)
#define RIGHT   ((float) 500.0)
#define TOP     ((float) 800.0)
    PDF_setlinewidth(p, 2);
    PDF_moveto(p, RIGHT, MARGIN);
    PDF_lineto(p, MARGIN, MARGIN);
    PDF_lineto(p, MARGIN, TOP);
    PDF_setgray_stroke(p, 0);           /* black */
    PDF_stroke(p);

    PDF_setlinewidth(p, 1);
    PDF_moveto(p, MARGIN, MARGIN);

    /* construct some random graph data */
    PDF_setrgbcolor_stroke(p, (float) 1.0, (float) 0, (float) 0);/* red */
    for (x=MARGIN; x<RIGHT; x+=STEP)
        PDF_lineto(p, x, x + (TOP-MARGIN)/(float)2.0*rand()/(RAND_MAX+(float)1.0));

    PDF_stroke(p);

    PDF_setrgbcolor_stroke(p, (float) 0, (float) 1.0, (float) 0);/* green */
    PDF_moveto(p, MARGIN, MARGIN);
    for (x=MARGIN; x<RIGHT; x+=STEP)
        PDF_lineto(p, x,
            MARGIN + TOP*(x-MARGIN)*(x-MARGIN)/((RIGHT-MARGIN)*(RIGHT-MARGIN)));

    PDF_stroke(p);

    PDF_setrgbcolor_stroke(p, (float) 0, (float) 0, (float) 1.0);/* blue */
    PDF_moveto(p, MARGIN, MARGIN);
    for (x=MARGIN; x<RIGHT; x+=STEP)
        PDF_lineto(p, x, MARGIN + x + MARGIN*rand()/(RAND_MAX+(float)1.0));

    PDF_stroke(p);

    PDF_end_page(p);

    MESSAGE("done\n");

#undef STEP
#undef MARGIN
#undef RIGHT
#undef TOP
}

/* ------------------------------------------------------------- */
static void
many_pages(PDF *p)
{
    char buf[128];
    int i, font;

#define FONTSIZE        24
#define FONTNAME        "Times-Roman"
#define PAGECOUNT       1000

    MESSAGE("Many pages test...");

    for (i = 1; i <= PAGECOUNT; i++) {
        PDF_begin_page(p, a4_width, a4_height);

        if (i == 1)
            (void) PDF_add_bookmark(p, "Many pages", 0, 1);

        font = PDF_findfont(p, FONTNAME, "host", 1);
        PDF_setfont(p, font, FONTSIZE);

        sprintf(buf, "Page %d", i);
        PDF_show_xy(p, buf, 200, 600);

        PDF_end_page(p);
    }

    MESSAGE("done\n");
}

static unsigned
#ifdef PDF_WINTHREADS
__stdcall               /* needed for threads */
#endif
Do_PDFlib_tests(void *arg)
{
    char        filename[50];
    PDF         *p;

#ifdef PDF_WINTHREADS
    sprintf(filename, "pdftest%03ld.pdf", (long) arg);
#else
    (void) arg; /* avoid compiler warning "unreferenced parameter" */
    strcpy(filename, "pdftest.pdf");
#endif

    fprintf(stderr, "Creating PDFlib test file '%s'!\n", filename);

    PDF_boot();

    p = PDF_new();

    if (PDF_open_file(p, filename) == -1) {
        printf("Couldn't open PDF file '%s'!\n", filename);
        PDF_ENDTHREAD(1);
        return 1;
    }

    PDF_set_info(p, "Keywords", "image graphics text hypertext");
    PDF_set_info(p, "Subject", "Check many PDFlib function calls");
    PDF_set_info(p, "Title", "PDFlib test program");
    PDF_set_info(p, "Creator", "PDFtest");
    PDF_set_info(p, "Author", "Thomas Merz");

#ifdef HAVE_LIBPNG
    png_image(p);
#endif
    gif_image(p);
    jpeg_image(p);
#ifdef HAVE_LIBTIFF
    tiff_image(p);
#endif
    centered_text(p);
    character_table(p);
    annotations(p);
    bookmarks(p);
    grid(p);
    shaded_circle(p);
    memory_image(p);
    radial_structure(p);
    random_data_graph(p);
    many_pages(p);

    PDF_close(p);
    PDF_delete(p);

    PDF_shutdown();

    fprintf(stderr, "\nPDFlib test file '%s' finished!\n", filename);

    PDF_ENDTHREAD(0);

    return 0;
}

#define PDF_MAXTHREADS 4

int
main(int argc, char *argv[])
{
#ifdef PDF_WINTHREADS
    unsigned t, w, thread_id;
    HANDLE hThread[PDF_MAXTHREADS];

    /* do a multi-threaded test */
    for (t=0; t < PDF_MAXTHREADS; t++) {
        hThread[t] =(HANDLE) _beginthreadex(NULL, 0, &Do_PDFlib_tests,
                                        (void *)t, 0, &thread_id );
        if (hThread[t] == 0) {
            printf("Couldn't create thread %ud\n", t);
            break;
        }
    }

    for (w=0; w < t; w++) {
        WaitForSingleObject(hThread[w], INFINITE);
        CloseHandle(hThread[w]);
    }
#else /* !PDF_WINTHREADS */

    /* do a single test */
    Do_PDFlib_tests((void *) 0L);

#endif /* !PDF_WINTHREADS */

    (void) argc;        /* avoid compiler warning "unreferenced parameter" */
    (void) argv;        /* avoid compiler warning "unreferenced parameter" */

    exit(0);

    return 0;   /* shut up compiler warnings */
}
