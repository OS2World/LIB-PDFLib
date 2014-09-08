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

/* text2pdf.c
 *
 * Convert text files to PDF
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if !defined(WIN32) && !defined(MAC) && !defined(__OS2__)
#include <unistd.h>
#endif

#ifdef WIN32
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
#if defined WIN32 || defined __DJGPP__ || \
    defined __OS2__ || defined __IBMC__ || defined __IBMCPP__ || \
    defined __POWERPC__ || defined __CFM68K__ || defined __MC68K__ || \
    defined AS400 || defined __ILEC400__

#define READMODE        "rb"

#else

#define READMODE        "r"

#endif  /* Mac, Windows, and OS/2 platforms */

/* figure out whether or not we're running on an EBCDIC-based machine */
#define ASCII_A                 0x41
#define PLATFORM_A              'A'
#define EBCDIC_BRACKET          0x4A
#define PLATFORM_BRACKET        '['

#if (ASCII_A != PLATFORM_A && EBCDIC_BRACKET == PLATFORM_BRACKET)
#define PDFLIB_EBCDIC
#endif

#include "pdflib.h"

static void
usage(void)
{
    fprintf(stderr, "text2pdf - convert text files to PDF. (C) Thomas Merz 1997-2000\n");
    fprintf(stderr, "usage: text2pdf [options] [textfile]\n");
    fprintf(stderr, "Available options:\n");
    fprintf(stderr, "-e encoding   font encoding to use. Common encoding names:\n");
    fprintf(stderr, "              winansi, macroman, ebcdic, or user-defined\n");
    fprintf(stderr, "              host = default encoding of this platform\n");
    fprintf(stderr, "-f fontname   name of font to use\n");
    fprintf(stderr, "-h height     page height in points\n");
    fprintf(stderr, "-m margin     margin size in points\n");
    fprintf(stderr, "-o filename   PDF output file name\n");
    fprintf(stderr, "-s size       font size\n");
    fprintf(stderr, "-w width      page width in points\n");

    exit(1);
}

#define BUFLEN          512

int
main(int argc, char *argv[])
{
    char        buf[BUFLEN], *s;
    char        *pdffilename = NULL;
    FILE        *textfile = stdin;
    PDF         *p;
    int         opt;
    int         font;
    char        *fontname, *encoding;
    float       fontsize;
    float       x, y, width = a4_width, height = a4_height, margin = 20;
    char        ff, cr, nl;

    fontname    = "Courier";
    fontsize    = 12.0;
    encoding    = "host";
    nl          = '\n';
    ff          = '\f';

    while ((opt = getopt(argc, argv, "e:f:h:m:o:s:w:")) != -1)
        switch (opt) {
            case 'e':
                encoding = optarg;
                break;

            case 'f':
                fontname = optarg;
                break;

            case 'h':
                height = atoi(optarg);
                if (height < 0) {
                    fprintf(stderr, "Error: bad page height %f!\n", height);
                    usage();
                }
                break;

            case 'm':
                margin = atoi(optarg);
                if (margin < 0) {
                    fprintf(stderr, "Error: bad margin %f!\n", margin);
                    usage();
                }
                break;

            case 'o':
                pdffilename = optarg;
                break;

            case 's':
                fontsize = atoi(optarg);
                if (fontsize < 0) {
                    fprintf(stderr, "Error: bad font size %f!\n", fontsize);
                    usage();
                }
                break;

            case 'w':
                width = atoi(optarg);
                if (width < 0) {
                    fprintf(stderr, "Error: bad page width %f!\n", width);
                    usage();
                }
                break;

            case '?':
            default:
                usage();
        }

    if (!strcmp(encoding, "ebcdic")) {
        /* form feed is 0x0C in both ASCII and EBCDIC */
        nl = 0x15;
    }

    if (pdffilename == NULL)
        usage();

    if (optind < argc) {
        if ((textfile = fopen(argv[optind], READMODE)) == NULL) {
            fprintf(stderr, "Error: cannot open input file %s.\n",argv[optind]);
            exit(2);
        }
    } else
        textfile = stdin;

    p = PDF_new();
    if (p == NULL) {
        fprintf(stderr, "Error: cannot open output file %s.\n", pdffilename);
        exit(1);
    }

    PDF_open_file(p, pdffilename);

    PDF_set_info(p, "Title", "Converted text");
    PDF_set_info(p, "Creator", "text2pdf");

    x = margin;
    y = height - margin;

    while ((s = fgets(buf, BUFLEN, textfile)) != NULL) {
        if (s[0] == ff) {
            if (y == height - margin)
                PDF_begin_page(p, width, height);
            PDF_end_page(p);
            y = height - margin;
            continue;
        }

        if (s[0] != '\0' && s[strlen(s) - 1] == nl)
            s[strlen(s) - 1] = '\0';    /* remove newline character */

        if (y < margin) {               /* page break necessary? */
            y = height - margin;
            PDF_end_page(p);
        }

        if (y == height - margin) {
            PDF_begin_page(p, width, height);
            font = PDF_findfont(p, fontname, encoding, 0);
            PDF_setfont(p, font, fontsize);
            PDF_set_text_pos(p, x, y);
            y -= fontsize;
        }

        PDF_continue_text(p, s);
        y -= fontsize;

    }

    if (y != height - margin)
        PDF_end_page(p);

    PDF_close(p);
    PDF_delete(p);

    exit(0);
}
