#!/usr/bin/python
# pdfclock.py
# Copyright (C) 1997-2000 Thomas Merz. All rights reserved.
#
# PDFlib client: pdfclock example in Python
#

from sys import *
from time import *
from pdflib_py import *

RADIUS = 200.0;
MARGIN = 20.0;

p = PDF_new()

if PDF_open_file(p, "pdfclock_py.pdf") == -1:
    print 'Couldn\'t open PDF file!', "pdfclock_py.pdf"
    exit(2);

PDF_set_info(p, "Author", "Thomas Merz")
PDF_set_info(p, "Creator", "pdfclock.py")
PDF_set_info(p, "Title", "PDF clock (Python)")

PDF_begin_page(p, 2 * (RADIUS + MARGIN), 2 * (RADIUS + MARGIN));

PDF_translate(p, RADIUS + MARGIN, RADIUS + MARGIN);
PDF_setrgbcolor(p, 0.0, 0.0, 1.0);
PDF_save(p);

# minute strokes 
PDF_setlinewidth(p, 2.0);
for alpha in range(0, 360, 6):
    PDF_rotate(p, 6.0);
    PDF_moveto(p, RADIUS, 0.0);
    PDF_lineto(p, RADIUS-MARGIN/3, 0.0);
    PDF_stroke(p);

PDF_restore(p);
PDF_save(p);

# 5 minute strokes
PDF_setlinewidth(p, 3.0);
for alpha in range(0, 360, 30):
    PDF_rotate(p, 30.0);
    PDF_moveto(p, RADIUS, 0.0);
    PDF_lineto(p, RADIUS-MARGIN, 0.0);
    PDF_stroke(p);

(tm_year, tm_year, tm_day,
tm_hour, tm_min, tm_sec, 
tm_weekday, tm_julian, tm_ds) = localtime(time());

# draw hour hand 
PDF_save(p);
PDF_rotate(p, (-((tm_min/60.0) + tm_hour - 3.0) * 30.0));
PDF_moveto(p, -RADIUS/10, -RADIUS/20);
PDF_lineto(p, RADIUS/2, 0.0);
PDF_lineto(p, -RADIUS/10, RADIUS/20);
PDF_closepath(p);
PDF_fill(p);
PDF_restore(p);

# draw minute hand
PDF_save(p);
PDF_rotate(p, (-((tm_sec/60.0) + tm_min - 15.0) * 6.0));
PDF_moveto(p, -RADIUS/10, -RADIUS/20);
PDF_lineto(p, RADIUS * 0.8, 0.0);
PDF_lineto(p, -RADIUS/10, RADIUS/20);
PDF_closepath(p);
PDF_fill(p);
PDF_restore(p);

# draw second hand
PDF_setrgbcolor(p, 1.0, 0.0, 0.0);
PDF_setlinewidth(p, 2);
PDF_save(p);
PDF_rotate(p, -((tm_sec - 15.0) * 6.0));
PDF_moveto(p, -RADIUS/5, 0.0);
PDF_lineto(p, RADIUS, 0.0);
PDF_stroke(p);
PDF_restore(p);

# draw little circle at center
PDF_circle(p, 0, 0, RADIUS/30);
PDF_fill(p);

PDF_restore(p);
PDF_end_page(p);

PDF_close(p);
PDF_delete(p);
