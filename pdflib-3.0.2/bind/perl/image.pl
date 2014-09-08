#!/usr/bin/perl
# image.pl
# Copyright (C) 1997-2000 Thomas Merz. All rights reserved.
#
# PDFlib client: image example in Perl
#

use pdflib_pl 3.02;

$p = PDF_new();

die "Couldn't open PDF file" if (PDF_open_file($p, "image_pl.pdf") == -1);

PDF_set_info($p, "Creator", "image.pl");
PDF_set_info($p, "Author", "Thomas Merz");
PDF_set_info($p, "Title", "image sample (Perl)");

$imagefile = "../../test/nesrin.jpg";

$image = PDF_open_image_file($p, "jpeg", $imagefile, "", 0);
die "Couldn't open image '$imagefile'" if ($image == -1);

# We generate a page with the image's dimensions
$width = PDF_get_value($p, "imagewidth", $image);
$height = PDF_get_value($p, "imageheight", $image);

PDF_begin_page($p, $width, $height);
PDF_place_image($p, $image, 0, 0, 1);
PDF_end_page($p);

PDF_close($p);

PDF_delete($p);
