# Makefile for ZLIB.1-1-3    tested with vac 3.6.5 fixpak 1 & os2 toolkit 4.5
# as you see we don"t use the zlib.def anymore
# All the function are exported with the __export keyword
# see ZCONF.H ifdef __IBMC__
#
# Ported by Serge Sterck 08/09/2000

CC = icc
LD = ilink
AR = ilib
RM = del
IM = implib

CFLAGS  = /W1 /Ge- /Q+ /DZLIB_DLL
LDFLAGS = /nologo

OBJS = adler32.obj compress.obj crc32.obj gzio.obj uncompr.obj deflate.obj trees.obj \
       zutil.obj inflate.obj infblock.obj inftrees.obj infcodes.obj infutil.obj inffast.obj

ALL : zlib113.dll example.exe minigzip.exe
CLEAN :
   -  del *.obj
   -  del *.exe
   -  del *.dll

zlib113.dll : $(OBJS)
     $(LD) $(LDFLAGS) /DLL $(OBJS) /out:zlib113.dll
     $(IM) /nologo zlib113.lib zlib113.dll

example.exe : example.obj
     $(LD) $(LDFLAGS) /EXE example.obj zlib113.lib

example.obj : example.c
     $(CC) /c /Q+ example.c

minigzip.exe : minigzip.obj
     $(LD) $(LDFLAGS) /EXE minigzip.obj zlib113.lib

minigzip.obj : minigzip.c
     $(CC) /c /Q+ minigzip.c
