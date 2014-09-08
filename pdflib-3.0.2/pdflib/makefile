# Makefile for PDFLIB.3.0.2 tested with vac 3.6.5 fixpak 1 & os2 toolkit 4.5
# Ported by Serge Sterck 08/09/2000

CC = icc
LD = ilink
AR = ilib
RM = del
IM = implib

CFLAGS1 = /W1 /Ge- /Q+ /DPDFLIB_EXPORTS /I..\..\zlib.1-1-3 /I..\..\tiff.3-5-5\libtiff /I..\..\lpng.1-0-8
CFLAGS2 = /DHAVE_LIBTIFF /DHAVE_LIBZ /DHAVE_LIBPNG
CFLAGS  = $(CFLAGS1) $(CFLAGS2)
LDFLAGS =
LIBS    = ..\..\zlib.1-1-3\zlib113.lib ..\..\tiff.3-5-5\libtiff\tiff355.lib ..\..\lpng.1-0-8\lpng108.lib

OBJS = p_afm.obj       \
       p_annots.obj    \
       p_basic.obj     \
       p_ccitt.obj     \
       p_color.obj     \
       p_draw.obj      \
       p_filter.obj    \
       p_font.obj      \
       p_gif.obj       \
       p_gstate.obj    \
       p_hyper.obj     \
       p_image.obj     \
       p_jpeg.obj      \
       p_pfm.obj       \
       p_png.obj       \
       p_stream.obj    \
       p_text.obj      \
       p_tiff.obj      \
       p_util.obj
ALL : pdf302.dll

CLEAN :
   -  del *.obj
   -  del *.exe
   -  del *.dll

pdf302.dll : $(OBJS)
   - $(LD) $(LDFLAGS) $(LIBS) /DLL $(OBJS) /out:pdf302.dll
     $(IM) pdf302.lib pdf302.dll
