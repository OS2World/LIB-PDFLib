# Makefile for TIFF.3-5-5 TOOLS tested with vac 3.6.5 fixpak 1 & os2 toolkit 4.5
# Ported by Serge Sterck 08/09/2000

CC = icc
LD = ilink
AR = ilib
RM = del
IM = implib

CFLAGS  = /W1 /Q+ /I..\libtiff  ..\libtiff\tiff355.lib
LDFLAGS =
LIBS    = port.lib

TARGETS: port.lib \
         tiffcp.exe tiffinfo.exe tiffdump.exe fax2tiff.exe \
         fax2ps.exe gif2tiff.exe pal2rgb.exe ppm2tiff.exe \
         rgb2ycbcr.exe thumbnail.exe ras2tiff.exe tiff2bw.exe \
         tiff2rgba.exe tiff2ps.exe tiffcmp.exe tiffdither.exe \
         tiffmedian.exe tiffsplit.exe

port.lib:..\port\strcasecmp.c ..\port\getopt.c
         $(CC) /c ..\port\strcasecmp.c
         $(CC) /c ..\port\getopt.c
         $(AR) /nofreeformat port.lib +strcasecmp.obj +getopt.obj;
.c.exe:
        $(CC) $(CFLAGS) $*.c $(LIBS)

CLEAN :
   -  del *.obj
   -  del *.exe
   -  del *.dll

