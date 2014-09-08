# Makefile for TIFF.3-5-5    tested with vac 3.6.5 fixpak 1 & os2 toolkit 4.5
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

CFLAGS  = /W1 /Ge- /Q+ /DTIF_PLATFORM_CONSOLE /I..\zlib.1-1-3
LDFLAGS = ..\..\zlib.1-1-3\zlib113.lib tiff355.def

OBJS    = \
        tif_aux.obj \
        tif_close.obj \
        tif_codec.obj \
        tif_compress.obj \
        tif_dir.obj \
        tif_dirinfo.obj \
        tif_dirread.obj \
        tif_dirwrite.obj \
        tif_dumpmode.obj \
        tif_error.obj \
        tif_fax3.obj \
        fax3sm_winnt.obj \
        tif_getimage.obj \
        tif_jpeg.obj \
        tif_flush.obj \
        tif_luv.obj \
        tif_lzw.obj \
        tif_next.obj \
        tif_open.obj \
        tif_packbits.obj \
        tif_pixarlog.obj \
        tif_predict.obj \
        tif_print.obj \
        tif_read.obj \
        tif_swab.obj \
        tif_strip.obj \
        tif_thunder.obj \
        tif_tile.obj \
        tif_os2.obj   \
        tif_version.obj \
        tif_warning.obj \
        tif_write.obj \
        tif_zip.obj

ALL : version.h tiff355.dll

CLEAN :
   -  del *.obj
   -  del *.exe
   -  del *.dll

tiff355.dll : $(OBJS)
     $(LD) $(LDFLAGS) /DLL $(OBJS) /out:tiff355.dll
     $(IM) tiff355.lib tiff355.dll

version.h:      $(VERSION) $(ALPHA) mkversion.c
     $(CC) /W1 /Ti  mkversion.c
  -  @del version.h
     .\mkversion -v ..\version -a ..\dist\tiff.alpha version.h

mkversion.obj : mkversion.c

tif_version.obj:        version.h
