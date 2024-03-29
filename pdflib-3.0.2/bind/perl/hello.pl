#!/usr/bin/perl
# hello.pl
# Copyright (C) 1997-2000 Thomas Merz. All rights reserved.
#
# PDFlib client: hello example in Perl
#

use pdflib_pl 3.02;

$p = PDF_new();

die "Couldn't open PDF file" if (PDF_open_file($p, "hello_pl.pdf") == -1);

PDF_set_info($p, "Creator", "hello.pl");
PDF_set_info($p, "Author", "Thomas Merz");
PDF_set_info($p, "Title", "Hello world (Perl)");

PDF_begin_page($p, 595, 842);
$font = PDF_findfont($p, "Helvetica-Bold", "host", 0);

PDF_setfont($p, $font, 18.0);

PDF_set_text_pos($p, 50, 700);
PDF_show($p, "Hello world!");
PDF_continue_text($p, "(says Perl)");

PDF_end_page($p);
PDF_close($p);

PDF_delete($p);
