/* image.java
 * Copyright (C) 1997-2000 Thomas Merz. All rights reserved.
 *
 * PDFlib client: image example in Java
 */

import java.io.*;
import com.pdflib.pdflib;

public class image
{
    public static void main (String argv[]) throws
	OutOfMemoryError, IOException, IllegalArgumentException,
	IndexOutOfBoundsException, ClassCastException, ArithmeticException,
	RuntimeException, InternalError, UnknownError
    {
	int image;
	float width, height;
	pdflib p;
	String imagefile = "../../test/nesrin.jpg";

	p = new pdflib();

	if (p.open_file("image_java.pdf") == -1) {
	    System.err.println("Couldn't open PDF file image_java.pdf\n");
	    System.exit(1);
	}

	p.set_info("Creator", "image.java");
	p.set_info("Author", "Thomas Merz");
	p.set_info("Title", "image sample (Java)");

	image = p.open_image_file("jpeg", imagefile, "", 0);

	if (image == -1) {
	    System.err.println("Couldn't open image file.\n");
	    System.exit(1);
	}

	width = p.get_value("imagewidth", image);
	height = p.get_value("imageheight", image);

	// We generate a page with the image's dimensions
	p.begin_page(width, height);
	p.place_image(image, (float) 0.0, (float) 0.0, (float) 1.0);
	p.end_page();

	p.close();
    }
}
