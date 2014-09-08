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

/* compile_metrics.c
 *
 * Generate C header file with compiled font metrics from AFM or PFM
 *
 */

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#ifndef WIN32
#include <unistd.h>
#endif

#ifdef NeXT
#include <libc.h>	/* for getopt(), optind, optarg */
#endif

#ifdef __CYGWIN32__
#include <getopt.h>	/* for getopt(), optind, optarg */
#endif

#include "p_intern.h"
#include "p_font.h"

#ifdef WIN32
int getopt(int nargc, char **nargv, char *ostr);
char *optarg;
int optind;
#endif

#define HEADER "\
\n\
#ifdef %s\n\
\n\
/*\n\
 * Font metrics generated with compile_metrics utility\n\
 *\n\
 * Encoding used: %s\n\
 */\n"

void
add_entry(PDF *p, FILE *out, pdf_font font, pdf_encoding enc)
{
    int		i;

    fprintf(out, "{");
    fprintf(out, "\t\"%s\",\t\t\t/* FontName */\n", font.name);
    fprintf(out, "\t%s,\t\t\t/* Encoding */\n", p->encodings[enc]->apiname);
    fprintf(out, "\t0,\t\t\t\t/* used on current page */\n");
    fprintf(out, "\t0,\t\t\t\t/* embed flag */\n");
    fprintf(out, "\t(char *) NULL,\t\t\t/* Name of external font file */\n");
    fprintf(out, "\t0L,\t\t\t\t/* Object id */\n");
    fprintf(out, "\t%uL,\t\t\t/* Font flags */\n", font.flags);
    fprintf(out, "\tpdf_false,\t\t\t/* parsed from an AFM file */\n");
    fprintf(out, "\tpdf_false,\t\t\t/* Multiple Master font */\n");

    if (font.fullName)
	fprintf(out, "\t\"%s\",\t\t\t/* Full name */\n", font.fullName);
    else
	fprintf(out, "\tNULL,\t\t\t/* Full name */\n");

    fprintf(out, "\t\"%s\",\t\t/* Native encoding */\n", font.encodingScheme);

    if (font.familyName)
	fprintf(out, "\t\"%s\",\t\t\t/* Family name */\n", font.familyName);
    else
	fprintf(out, "\tNULL,\t\t\t/* Family name */\n");

    if (font.weight)
	fprintf(out, "\t\"%s\",\t\t\t\t/* Weight */\n", font.weight);
    else
	fprintf(out, "\tNULL,\t\t\t\t/* Weight */\n");

    fprintf(out, "\tcc_none,\t\t\t\t/* Character collection */\n");

    fprintf(out, "\t(float) %2.1f,\t\t\t/* ItalicAngle */\n", font.italicAngle);
    fprintf(out, "\t%d,\t\t\t\t/* isFixedPitch */\n", font.isFixedPitch);
    fprintf(out, "\t%d,\t\t\t\t/* llx */\n", (int) font.llx);
    fprintf(out, "\t%d,\t\t\t\t/* lly */\n", (int) font.lly);
    fprintf(out, "\t%d,\t\t\t\t/* urx */\n", (int) font.urx);
    fprintf(out, "\t%d,\t\t\t\t/* ury */\n", (int) font.ury);
    fprintf(out, "\t%d,\t\t\t\t/* UnderlinePosition */\n", font.underlinePosition);
    fprintf(out, "\t%d,\t\t\t\t/* UnderlineThickness */\n",font.underlineThickness);
    fprintf(out, "\t%d,\t\t\t\t/* CapHeight */\n", font.capHeight);
    fprintf(out, "\t%d,\t\t\t\t/* xHeight */\n", font.xHeight);
    fprintf(out, "\t%d,\t\t\t\t/* Ascender */\n", font.ascender);
    fprintf(out, "\t%d,\t\t\t\t/* Descender */\n", font.descender);
    fprintf(out, "\t%d,\t\t\t\t/* StdVW */\n", font.StdVW);
    fprintf(out, "\t%d,\t\t\t\t/* StdHW */\n", font.StdHW);

    fprintf(out, "\n\t0,\t\t\t\t/* numOfChars */\n");
    fprintf(out, "\t(CharMetricInfo *) NULL,\t/* cmi */\n");
    fprintf(out, "\t0,\t\t\t\t/* numOfTracks */\n");
    fprintf(out, "\t(TrackKernData *) NULL,\t\t/* tkd */\n");
    fprintf(out, "\t0,\t\t\t\t/* numOfPairs */\n");
    fprintf(out, "\t(PairKernData *) NULL,\t\t/* pkd */\n");
    fprintf(out, "\t0,\t\t\t\t/* numOfComps */\n");
    fprintf(out, "\t(CompCharData *) NULL,\t\t/* ccd */\n");

    fprintf(out, "\n/* Character metrics */\n", font.name);
    fprintf(out, "{");

    for (i=0; i < 256; i++) {
	fprintf(out, "\t%3d", font.widths[i]);
	if (i == 255)
	    fprintf(out, " ");
	else
	    fprintf(out, ",");

	if (i % 8 == 7)
	    fprintf(out, " /* 0x%02x */\n", i);
    }

    fprintf(out, "}\n");
    fprintf(out, "},\n");

    (void) pdf_cleanup_font_struct(p, &font);
}

int
main(int argc, char *argv[])
{
    int   opt;
    char *filename, *outfilename = NULL;
    int enc;
    FILE *out;
    PDF  *p;
    pdf_font font;
    size_t len;

    /* This is only a dummy to provide a PDF* for the auxiliary functions */
    if ((p = PDF_new()) == NULL) {
	fprintf(stderr, "Couldn't generate internal PDF object - aborting\n");
	exit(99);
    }

    /* By default, generate compiled metrics for the current platform.
     * This may also be changed since the encoding is recorded in the
     * generated C structs.
     */
#ifdef EBCDIC
    enc = ebcdic;
#elif MAC
    enc = macroman;
#else
    enc = winansi;
#endif

    while ((opt = getopt(argc, argv, "e:o:")) != -1)
	switch (opt) {
	    case 'e':
		for (enc = 0; p->encodings[enc] != NULL; enc++) {
		    if (p->encodings[enc]->apiname &&
			!strcmp(optarg, p->encodings[enc]->apiname))
			break;
		}
		if (p->encodings[enc] == NULL) {
		    fprintf(stderr, "Encoding %s not supported!\n", optarg);
		    exit(88);
		}
		break;

	    case 'o':
	    	outfilename = optarg;
		if ((out = fopen(outfilename, "w")) == NULL) {
		    fprintf(stderr, "Couldn't open output file %s!\n",
		    	outfilename);
		    exit(99);
		}

		break;

	    default:
	    	break;
	}

    if (outfilename == NULL) {
	fprintf(stderr,
	    "%s: Compile AFM or PFM font metrics data to C code\n", argv[0]);
	fprintf(stderr, 
"Usage: %s -e [winansi|macroman|ebcdic] -o outfile metrics_files...\n",
	    argv[0]);
	exit(1);
    }

    fprintf(out,HEADER, p->encodings[enc]->apiname, p->encodings[enc]->apiname);

    while (optind < argc) {
	filename = argv[optind++];
	fprintf(stderr, "Reading %s\n", filename);

	len = strlen(filename);

	if (len >= 5 && (!strcmp(filename + len - 4, ".pfm") ||
	    !strcmp(filename + len - 4, ".PFM"))) {
	    /* parse PFM file */
	    if (!pdf_get_metrics_pfm(p, &font, NULL,  enc, filename)) {
		fprintf(stderr, "Error parsing PFM file '%s' - skipped!\n",
		filename);
	    } else {
		fprintf(out,
		"\n/* -------- Generated from metrics file %s -------- */\n",
		filename);
		add_entry(p, out, font, enc);
	    }
	} else if (len >= 5 && (!strcmp(filename + len - 4, ".afm") ||
		!strcmp(filename + len - 4, ".AFM"))) {
		/* parse AFM file */
		if (!pdf_get_metrics_afm(p, &font, NULL,  enc, filename)) {
		    fprintf(stderr, "Error parsing AFM file '%s' - skipped!\n",
		    filename);
		} else {
		    fprintf(out,
		"\n/* -------- Generated from metrics file %s -------- */\n",
		    filename);
		    add_entry(p, out, font, enc);
		}
	} else {
	    fprintf(stderr, "Unknown file name suffix in '%s' - skipped!\n",
		filename);
	}
    }

    fprintf(out, "\n#endif /* %s */\n\n", p->encodings[enc]->apiname);

    fclose(out);
    exit(0);
}
