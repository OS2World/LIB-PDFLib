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

CFLAGS  = /W1 /Ge- /Q+ /DPNG_BUILD_DLL /I..\zlib.1-1-3
LDFLAGS = ..\zlib.1-1-3\zlib113.lib

OBJS = png.obj pngset.obj pngget.obj pngrutil.obj pngtrans.obj pngwutil.obj \
        pngread.obj pngrio.obj pngwio.obj pngwrite.obj pngrtran.obj \
        pngwtran.obj pngmem.obj pngerror.obj pngpread.obj

ALL : lpng108.dll pngtest.exe

CLEAN :
   -  del *.obj
   -  del *.exe
   -  del *.dll

lpng108.dll : $(OBJS)
     $(LD) $(LDFLAGS) /DLL $(OBJS) /out:lpng108.dll
     $(IM) lpng108.lib lpng108.dll

pngtest.exe : pngtest.obj
     $(LD) /EXE pngtest.obj ..\zlib.1-1-3\zlib113.lib lpng108.lib

pngtest.obj : pngtest.c
     $(CC) /W1 /c /I ..\zlib.1-1-3 pngtest.c
