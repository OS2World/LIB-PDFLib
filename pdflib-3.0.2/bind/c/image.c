/* image.c
 * Copyright (C) 1997-2000 Thomas Merz. All rights reserved.
 * PDFlib client: image example in C
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "pdflib.h"

int
main(void)
{
    PDF *p;
    int image;
    float width, height;
    char *imagefile = "../../test/nesrin.jpg";

    p = PDF_new();

    /* open new PDF file */
    if (PDF_open_file(p, "image_c.pdf") == -1) {
	fprintf(stderr, "Error: couldn't open PDF file.\n");
	exit(2);
    }

    PDF_set_info(p, "Creator", "image.c");
    PDF_set_info(p, "Author", "Thomas Merz");
    PDF_set_info(p, "Title", "image sample (C)!");

    image = PDF_open_image_file(p, "jpeg", imagefile, "", 0);

    if (image == -1) {
	fprintf(stderr, "Error: couldn't open image file.\n");
	exit(3);
    }

    width = PDF_get_value(p, "imagewidth", image);
    height = PDF_get_value(p, "imageheight", image);

    /* We generate a page with the image's dimensions */
    PDF_begin_page(p, width, height);
    PDF_place_image(p, image, (float) 0.0, (float) 0.0, (float) 1.0);

    PDF_end_page(p);				/* close page		*/

    PDF_close(p);				/* close PDF document	*/

    PDF_delete(p);				/* delete the PDF object */

    exit(0);
}
