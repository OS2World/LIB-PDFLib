// pdfclock.cpp
// Copyright (C) 1997-2000 Thomas Merz. All rights reserved.
// A little PDFlib application to draw an analog clock.
//
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#if !defined(WIN32) && !defined(MAC)
#include <unistd.h>
#endif

#include "pdflib.hpp"

#define READMODE	"rb"
#define RADIUS		200.0
#define MARGIN		20.0
#define BUFSIZE		512

#define FILENAME	"pdfclock_cpp.pdf"

int
main()
{
    PDF		*p;
    float	alpha;
    time_t	timer;
    struct tm	ltime;
    
    /* Create a new PDF object */
    p = new PDF();

    /* Open new PDF file */
    if (p->open(FILENAME) == -1) {
	fprintf(stderr, "pdfclock error: cannot open PDF file %s.\n",
		FILENAME);
    }

    p->set_info("Creator", "pdfclock.cpp");
    p->set_info("Author", "Thomas Merz");
    p->set_info("Title", "PDF clock (C++)");

    p->begin_page((unsigned int) (2 * (RADIUS + MARGIN)),
		      (unsigned int) (2 * (RADIUS + MARGIN)));
    
    p->translate(RADIUS + MARGIN, RADIUS + MARGIN);
    p->setrgbcolor(0.0, 0.0, 1.0);
    p->save();

    /* minute strokes */
    p->setlinewidth(2.0);
    for (alpha = 0; alpha < 360; alpha += 6)
    {
	p->rotate(6.0);
	p->moveto(RADIUS, 0.0);
	p->lineto((float) (RADIUS-MARGIN/3), 0.0);
	p->stroke();
    }

    p->restore();
    p->save();

    /* 5 minute strokes */
    p->setlinewidth(3.0);
    for (alpha = 0; alpha < 360; alpha += 30)
    {
	p->rotate(30.0);
	p->moveto(RADIUS, 0.0);
	p->lineto(RADIUS-MARGIN, 0.0);
	p->stroke();
    }

    time(&timer);
    ltime = *localtime(&timer);

    /* draw hour hand */
    p->save();
    p->rotate(
	    (float)(-((ltime.tm_min/60.0) + ltime.tm_hour - 3.0) * 30.0));
    p->moveto(-RADIUS/10, -RADIUS/20);
    p->lineto(RADIUS/2, 0.0);
    p->lineto(-RADIUS/10, RADIUS/20);
    p->closepath();
    p->fill();
    p->restore();

    /* draw minute hand */
    p->save();
    p->rotate((float) (-((ltime.tm_sec/60.0) + ltime.tm_min - 15.0) * 6.0));
    p->moveto(-RADIUS/10, -RADIUS/20);
    p->lineto(RADIUS * 0.8, 0.0);
    p->lineto(-RADIUS/10, RADIUS/20);
    p->closepath();
    p->fill();
    p->restore();

    /* draw second hand */
    p->setrgbcolor(1.0, 0.0, 0.0);
    p->setlinewidth(2);
    p->save();
    p->rotate((float) -((ltime.tm_sec - 15.0) * 6.0));
    p->moveto(-RADIUS/5, 0.0);
    p->lineto(RADIUS, 0.0);
    p->stroke();
    p->restore();

    /* draw little circle at center */
    p->circle(0, 0, (float) RADIUS/30);
    p->fill();

    p->restore();

    p->end_page();

    p->close();

    return(0);
}
