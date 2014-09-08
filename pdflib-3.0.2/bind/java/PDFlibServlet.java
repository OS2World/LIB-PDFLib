/* PDFlibServlet.java
 * Copyright (C) 1997-2000 Thomas Merz. All rights reserved.
 *
 * PDFlib client: hello servlet example in Java
 */

import java.io.*;
import javax.servlet.*;

import com.pdflib.pdflib;

public class PDFlibServlet extends GenericServlet
{
    public void service(ServletRequest request, ServletResponse response)
    throws ServletException,
	OutOfMemoryError, IOException, IllegalArgumentException,
	IndexOutOfBoundsException, ClassCastException, ArithmeticException,
	RuntimeException, InternalError, UnknownError
    {
	int font;
	pdflib p;
	byte[] buf;
	ServletOutputStream out;

	p = new pdflib();
	p.open_file("");

	p.set_info("Creator", "PDFlibServlet.java");
	p.set_info("Author", "Thomas Merz");
	p.set_info("Title", "Hello world (Java servlet)");

	p.begin_page(595, 842);

	font = p.findfont("Helvetica-Bold", "host", 0);

	p.setfont(font, 18);

	p.set_text_pos(50, 700);
	p.show("Hello world!");
	p.continue_text("(says Java servlet)");
	p.end_page();

	p.close();

	buf = p.get_buffer();

	response.setContentType("application/pdf");
	response.setContentLength(buf.length);

	out = response.getOutputStream();
	out.write(buf);
	out.close();
    }
}
