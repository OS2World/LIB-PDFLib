#!/bin/sh
# image.tcl
# Copyright (C) 1997-2000 Thomas Merz. All rights reserved.
#
# PDFlib client: image example in Tcl
#

# Hide the exec to Tcl but not to the shell by appending a backslash\
exec tclsh "$0" ${1+"$@"}

# The lappend line is unnecessary if PDFlib has been installed
# in the Tcl package directory
lappend auto_path .

package require pdflib 3.02

set p [PDF_new]

if {[PDF_open_file $p "image_tcl.pdf"] == -1} {
    puts stderr "Couldn't open PDF file!"
    exit
}

PDF_set_info $p "Creator" "image.tcl"
PDF_set_info $p "Author" "Thomas Merz"
PDF_set_info $p "Title" "image sample (Tcl)"

set image [PDF_open_image_file $p "jpeg" "../../test/nesrin.jpg" "" 0]

if {$image == -1} {
    puts stderr "Couldn't open image file!"
    exit
}

set width [PDF_get_value $p "imagewidth" image]
set height [PDF_get_value $p "imageheight" image]

PDF_begin_page $p $width $height
PDF_place_image $p $image 0 0 1
PDF_end_page $p
PDF_close $p

PDF_delete $p
