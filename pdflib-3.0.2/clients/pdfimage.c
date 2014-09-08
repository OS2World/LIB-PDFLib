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

/* pdfimage.c
 *
 * Convert PNG/TIFF/GIF/JPEG images to PDF
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#if !defined(WIN32) && !defined(MAC) && !defined(__OS2__)
#include <unistd.h>
#endif

#ifdef WIN32 || __OS2__
#include <process.h>
#endif

#ifdef NeXT
#include <libc.h>       /* for getopt(), optind, optarg */
#endif

#ifdef __CYGWIN32__
#include <getopt.h>     /* for getopt(), optind, optarg */
#endif

#ifdef __OS2__
        extern int optind;
        extern char* optarg;
#endif
#include "pdflib.h"

#if defined WIN32 || defined __DJGPP__ || \
    defined __OS2__ || defined __IBMC__ || defined __IBMCPP__ || \
    defined __POWERPC__ || defined __CFM68K__ || defined __MC68K__

#define READMODE        "rb"

#else

#define READMODE        "r"

#endif  /* Mac, Windows, and OS/2 platforms */

static void
usage(void)
{
    fprintf(stderr, "imagepdf: convert images to PDF. (C) Thomas Merz 1997-2000\n");
    fprintf(stderr, "usage: imagepdf [options] imagefile(s)\n");
    fprintf(stderr, "Available options:\n");
    fprintf(stderr, "-c         print caption\n");
    fprintf(stderr, "-o <file>  output file\n");

    exit(1);
}

/* Several magic numbers for image file formats */

#define GIF_MAGIC       "GIF"
#define TIFF_MAGIC_M    "MM"
#define TIFF_MAGIC_I    "II"
#define PNG_MAGIC       "\x89PNG"
#define JPEG_MAGIC      "\xFF\xD8"

#define MAGIC_LEN_MAX   10              /* maximum length of magic strings */

int
main(int argc, char *argv[])
{
    char        *pdffilename = NULL;
    FILE        *imagefile;
    PDF         *p;
    int         image;
    int         opt;
    int         caption = 0;
    float       scale_x, scale_y, dpi_x, dpi_y;
    char        header[MAGIC_LEN_MAX];

    while ((opt = getopt(argc, argv, "o:")) != -1)
        switch (opt) {
            case 'c':
                caption = 1;
                break;

            case 'o':
                pdffilename = optarg;
                break;
        }

    if (optind == argc) {
        fprintf(stderr, "Error: no image files given.\n");
        usage();
    }

    if (pdffilename == NULL) {
        fprintf(stderr, "Error: no output file given.\n");
        usage();
    }

    p = PDF_new();

    if (PDF_open_file(p, pdffilename) == -1) {
        fprintf(stderr, "Error: cannot open output file %s.\n", pdffilename);
        exit(1);
    }

    PDF_set_info(p, "Creator", "pdfimage");

    while (optind++ < argc) {
        fprintf(stderr, "Processing image file '%s'...\n", argv[optind-1]);

        if ((imagefile = fopen(argv[optind-1], READMODE)) == NULL) {
           fprintf(stderr, "Error: Couldn't open image file %s - skipped.\n",
                argv[optind-1]);
           continue;
        }

        if (fread(header, MAGIC_LEN_MAX, 1, imagefile) != 1)  {
           fprintf(stderr,
                "Error: Couldn't read from image file %s - skipped.\n",
                argv[optind-1]);
           fclose(imagefile);
           continue;
        }
        fclose(imagefile);

        if (!strncmp(header, GIF_MAGIC, strlen(GIF_MAGIC)))
           image = PDF_open_image_file(p, "gif", argv[optind-1], "", 0);

#ifdef HAVE_LIBTIFF
        else if (!strncmp(header, TIFF_MAGIC_I, strlen(TIFF_MAGIC_I)) ||
                 !strncmp(header, TIFF_MAGIC_M, strlen(TIFF_MAGIC_M)))
           image = PDF_open_image_file(p, "tiff", argv[optind-1], "", 0);
#endif

#ifdef HAVE_LIBPNG
        else if (!strncmp(header, PNG_MAGIC, strlen(PNG_MAGIC)))
           image = PDF_open_image_file(p, "png", argv[optind-1], "", 0);
#endif

        else if (!strncmp(header, JPEG_MAGIC, strlen(JPEG_MAGIC)))
           image = PDF_open_image_file(p, "jpeg", argv[optind-1], "", 0);

        else
           image = -1;  /* unknown file type */

        if (image == -1) {
            fprintf(stderr,"Error: Couldn't analyze image %s - skipped.\n",
                    argv[optind-1]);
            continue;
        }

        dpi_x = PDF_get_value(p, "resx", image);
        dpi_y = PDF_get_value(p, "resy", image);

        if (dpi_x > 0 && dpi_y > 0) {
            scale_x = ((float) 72.0) / dpi_x;
            scale_y = ((float) 72.0) / dpi_y;
        } else if (dpi_x < 0 && dpi_y < 0) {
            scale_x = (float) 1.0;
            scale_y = dpi_y / dpi_x;
        } else {
            scale_x = (float) 1.0;
            scale_y = (float) 1.0;
        }

        PDF_begin_page(p, PDF_get_value(p, "imagewidth", image) * scale_x,
                        PDF_get_value(p, "imageheight", image) * scale_y);

        /* define outline with filename */
        PDF_add_bookmark(p, argv[optind-1], 0, 0);

        PDF_scale(p, scale_x, scale_y);
        PDF_place_image(p, image, 0.0, 0.0, 1.0);

        PDF_end_page(p);
    }

    PDF_close(p);
    PDF_delete(p);
    exit(0);
}
