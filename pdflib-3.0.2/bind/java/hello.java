/* hello.java
 * Copyright (C) 1997-2000 Thomas Merz. All rights reserved.
 *
 * PDFlib client: hello example in Java
 */

import java.io.*;
import com.pdflib.pdflib;

public class hello
{
    public static void main (String argv[]) throws
	OutOfMemoryError, IOException, IllegalArgumentException,
	IndexOutOfBoundsException, ClassCastException, ArithmeticException,
	RuntimeException, InternalError, UnknownError
    {
	int font;
	pdflib p;

	p = new pdflib();

	if (p.open_file("hello_java.pdf") == -1) {
	    System.err.println("Couldn't open PDF file hello_java.pdf\n");
	    System.exit(1);
	}

	p.set_info("Creator", "hello.java");
	p.set_info("Author", "Thomas Merz");
	p.set_info("Title", "Hello world (Java)");

	p.begin_page(595, 842);

	font = p.findfont("Helvetica-Bold", "host", 0);

	p.setfont(font, 18);

	p.set_text_pos(50, 700);
	p.show("Hello world!");
	p.continue_text("(says Java)");
	p.end_page();

	p.close();
    }
}
