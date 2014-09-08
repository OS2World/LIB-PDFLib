#!/usr/bin/python
# image.py
# Copyright (C) 1997-2000 Thomas Merz. All rights reserved.
#
# PDFlib client: image example in Python
#

from sys import *
from pdflib_py import *

p = PDF_new()

if PDF_open_file(p, "image_py.pdf") == -1:
    print 'Couldn\'t open PDF file!', "image_py.pdf"
    exit(2);

PDF_set_info(p, "Author", "Thomas Merz")
PDF_set_info(p, "Creator", "image.py")
PDF_set_info(p, "Title", "image sample (Python)")

image = PDF_open_image_file(p, "jpeg", "../../test/nesrin.jpg", "", 0)

if image == -1:
    print 'Couldn\'t open image file!'
    exit(3);

width = PDF_get_value(p, "imagewidth", image)
height = PDF_get_value(p, "imageheight", image)

# We generate a page with the image's dimensions
PDF_begin_page(p, width, height)
PDF_place_image(p, image, 0, 0, 1);
PDF_end_page(p)

PDF_close(p)

PDF_delete(p);
