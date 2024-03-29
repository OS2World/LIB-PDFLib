/* hello.java
 * Copyright (C) 1997-2000 Thomas Merz. All rights reserved.
 *
 * PDFlib client: pdfclock example in Java
 */

import java.io.*;
import java.text.*;		// SimpleDateFormat
import java.util.*;		// Date
import com.pdflib.pdflib;

public class pdfclock
{
    public static void main (String argv[]) throws
	OutOfMemoryError, IOException, IllegalArgumentException,
	IndexOutOfBoundsException, ClassCastException, ArithmeticException,
	RuntimeException, InternalError, UnknownError
    {
	pdflib p;
	int tm_hour, tm_min, tm_sec, alpha;
	float RADIUS = 200.0f;
	float MARGIN = 20.0f;
	SimpleDateFormat format;
	Date now = new Date();

	p = new pdflib();

	if (p.open_file("pdfclock_java.pdf") == -1) {
	    System.err.println("Couldn't open PDF file" +"pdfclock_java.pdf\n");
	    System.exit(1);
	}

	p.set_info("Creator", "pdfclock.java");
	p.set_info("Author", "Thomas Merz");
	p.set_info("Title", "PDF clock (Java)");

	p.begin_page(  (int) (2 * (RADIUS + MARGIN)),
				(int) (2 * (RADIUS + MARGIN)));

	p.translate(RADIUS + MARGIN, RADIUS + MARGIN);
	p.setrgbcolor(0.0f, 0.0f, 1.0f);
	p.save();

	// minute strokes 
	p.setlinewidth(2.0f);
	for (alpha = 0; alpha < 360; alpha += 6)
	{
	    p.rotate(6.0f);
	    p.moveto(RADIUS, 0.0f);
	    p.lineto(RADIUS-MARGIN/3, 0.0f);
	    p.stroke();
	}

	p.restore();
	p.save();

	// 5 minute strokes
	p.setlinewidth(3.0f);
	for (alpha = 0; alpha < 360; alpha += 30)
	{
	    p.rotate(30.0f);
	    p.moveto(RADIUS, 0.0f);
	    p.lineto(RADIUS-MARGIN, 0.0f);
	    p.stroke();
	}

	format = new SimpleDateFormat("hh");
	tm_hour= Integer.parseInt(format.format(now));
	format = new SimpleDateFormat("mm");
	tm_min = Integer.parseInt(format.format(now));
	format = new SimpleDateFormat("ss");
	tm_sec = Integer.parseInt(format.format(now));

	// draw hour hand 
	p.save();
	p.rotate((-((tm_min/60.0f) + tm_hour - 3.0f) * 30.0f));
	p.moveto(-RADIUS/10, -RADIUS/20);
	p.lineto(RADIUS/2, 0.0f);
	p.lineto(-RADIUS/10, RADIUS/20);
	p.closepath();
	p.fill();
	p.restore();

	// draw minute hand
	p.save();
	p.rotate((-((tm_sec/60.0f) + tm_min - 15.0f) * 6.0f));
	p.moveto(-RADIUS/10, -RADIUS/20);
	p.lineto(RADIUS * 0.8f, 0.0f);
	p.lineto(-RADIUS/10, RADIUS/20);
	p.closepath();
	p.fill();
	p.restore();

	// draw second hand
	p.setrgbcolor(1.0f, 0.0f, 0.0f);
	p.setlinewidth(2);
	p.save();
	p.rotate(-((tm_sec - 15.0f) * 6.0f));
	p.moveto(-RADIUS/5, 0.0f);
	p.lineto(RADIUS, 0.0f);
	p.stroke();
	p.restore();

	// draw little circle at center
	p.circle(0f, 0f, RADIUS/30);
	p.fill();

	p.restore();
	p.end_page();
	p.close();
    }
}
