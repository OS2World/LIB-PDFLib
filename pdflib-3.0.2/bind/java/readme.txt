Notes on the PDFlib Java binding:

- Older JDK versions up to 1.1.8 are known to have multiple bugs with
  respect to timezone interpretation. For this reason chances are that
  the pdfclock example will display a wrong time on your system.

- It doesn't seem to be possible to catch malloc errors since the JNI
  exits before the PDFlib error handler jumps in.


Using PDFlib with Borland/Inprise JBuilder
==========================================

- Having pdflib.java in the same directory as the project file
  seems to confuse JBuilder.

- On Windows pdf_java.dll must be placed in the \winnt\system32 directory,
  or some other directory contained in the PATH environment variable.
  On Linux libpdf_java.so should be placed in /usr/lib or a similar
  well-known (to the system) directory.

- Relative pathnames in PDFlib function calls are interpreted relative
  to the JBuilder35 bin directory, not the project directory. Absolute
  path names work. For this reason the output of the supplied PDFlib samples
  may end up in some strange place.


Using PDFlib with Allaire JRun
==============================

In order to use PDFlib with JRun the following is suggested:

- Copy PDFlib.jar and lib_java.dll to the .../JRun/servers/lib directory.

- Make sure that the lib directory is contained in the servlet.jnipath
  property. This property can be set from the management console, or
  specified in a local.properties file.


Using PDFlib with IBM WebSphere Application Server
==================================================

Servlets are loaded with a custom class loader. For this reason,
the pdflib.jar file has to be located in the Application Server's
classpath rather than the web app's classpath.  

To locate the pdflib.jar file in the AS's classpath, place the jar 
file in the \<WAS app-server's path>\lib directory and edit the 
admin.conf file.  In the admin.conf file add the path to the jar file 
to the setting labeled:

com.ibm.ejs.sm.adminserver.classpath

The DLL or .so must be located somewhere on the machine's path.
The winnt\system32 directory works for Windows, the bin directory
of WAS on Solaris.
