// image.cpp
// Copyright (C) 1997-2000 Thomas Merz. All rights reserved.
// PDFlib client: image example in C++
//
//

#include <stdio.h>
#include <stdlib.h>

#include "pdflib.hpp"

int
main(void)
{
    PDF *p;			// pointer to the PDF class
    int image;
    float width, height;
    char *imagefile = "../../test/nesrin.jpg";

    p = new PDF();

    // Open new PDF file
    if (p->open("image_cpp.pdf") == -1) {
	fprintf(stderr, "Error: couldn't open PDF file image_cpp.pdf.\n");
	exit(2);
    }

    p->set_info("Creator", "image.cpp");
    p->set_info("Author", "Thomas Merz");
    p->set_info("Title", "image sample (C++)!");

    image = p->open_image_file("jpeg", imagefile, "", 0);

    if (image == -1) {
	fprintf(stderr, "Error: couldn't open image file.\n");
	exit(3);
    }

    width = p->get_value("imagewidth", image);
    height = p->get_value("imageheight", image);
    
    // We generate a page with the image's dimensions
    p->begin_page(width, height);
    p->place_image(image, (float) 0.0, (float) 0.0, (float) 1.0);
    p->end_page();				// finish page

    p->close();					// close PDF document
    delete p;

    return(0);
}
