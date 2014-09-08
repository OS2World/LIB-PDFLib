# Makefile for LPNG.1-0-8    tested with vac 3.6.5 fixpak 1 & os2 toolkit 4.5
# as you see we don"t use the pngos2.def anymore
# All the function are exported with the __export keyword
# All the variabled are exported with the _Export keyword
# see PNGCONF.H ifdef __IBMC__
# Ported by Serge Sterck 08/09/2000

CC = icc
LD = ilink
AR = ilib
RM = del
IM = implib

CFLAGS  = /W1 /Q+ /I..\pdflib
LDFLAGS =
LIBS    = ..\..\zlib.1-1-3\zlib113.lib ..\..\tiff.3-5-5\libtiff\tiff355.lib ..\..\lpng.1-0-8\lpng108.lib ..\pdflib\pdf302.lib

ALL : pdftest.exe

CLEAN :
   -  del *.obj
   -  del *.exe
   -  del *.dll


pdftest.exe : pdftest.obj
     $(LD) /EXE $(LIBS) pdftest.obj

pngtest.obj : pngtest.c
     $(CC) /W1 /c /I ..\zlib.1-1-3 pngtest.c
