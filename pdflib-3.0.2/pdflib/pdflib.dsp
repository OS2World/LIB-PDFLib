# Microsoft Developer Studio Project File - Name="pdflib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=pdflib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "pdflib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "pdflib.mak" CFG="pdflib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "pdflib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "pdflib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "pdflib - Win32 no auxiliary libs" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=xicl6.exe
RSC=rc.exe

!IF  "$(CFG)" == "pdflib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /O2 /I "..\..\libpng" /I "..\..\zlib" /I "..\..\libtiff" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "HAVE_LIBPNG" /D "HAVE_LIBZ" /D "HAVE_LIBTIFF" /D "_MT" /YX /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "pdflib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "pdflib___Win32_Debug"
# PROP BASE Intermediate_Dir "pdflib___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\libpng" /I "..\..\zlib" /I "..\..\libtiff" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "HAVE_LIBZ" /D "HAVE_LIBPNG" /D "HAVE_LIBTIFF" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "pdflib - Win32 no auxiliary libs"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "pdflib___Win32_no_auxiliary_libs"
# PROP BASE Intermediate_Dir "pdflib___Win32_no_auxiliary_libs"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O2 /I "..\..\libpng" /I "..\..\zlib" /I "..\..\libtiff" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "HAVE_LIBPNG" /D "HAVE_LIBZ" /D "HAVE_LIBTIFF" /D "_MT" /YX /FD /c
# ADD CPP /nologo /MT /W3 /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_MT" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "pdflib - Win32 Release"
# Name "pdflib - Win32 Debug"
# Name "pdflib - Win32 no auxiliary libs"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\p_afm.c
# End Source File
# Begin Source File

SOURCE=.\p_annots.c
# End Source File
# Begin Source File

SOURCE=.\p_basic.c
# End Source File
# Begin Source File

SOURCE=.\p_ccitt.c
# End Source File
# Begin Source File

SOURCE=.\p_color.c
# End Source File
# Begin Source File

SOURCE=.\p_draw.c
# End Source File
# Begin Source File

SOURCE=.\p_filter.c
# End Source File
# Begin Source File

SOURCE=.\p_font.c
# End Source File
# Begin Source File

SOURCE=.\p_gif.c
# End Source File
# Begin Source File

SOURCE=.\p_gstate.c
# End Source File
# Begin Source File

SOURCE=.\p_hyper.c
# End Source File
# Begin Source File

SOURCE=.\p_image.c
# End Source File
# Begin Source File

SOURCE=.\p_jpeg.c
# End Source File
# Begin Source File

SOURCE=.\p_pfm.c
# End Source File
# Begin Source File

SOURCE=.\p_png.c
# End Source File
# Begin Source File

SOURCE=.\p_stream.c
# End Source File
# Begin Source File

SOURCE=.\p_text.c
# End Source File
# Begin Source File

SOURCE=.\p_tiff.c
# End Source File
# Begin Source File

SOURCE=.\p_util.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\e_ansi.h
# End Source File
# Begin Source File

SOURCE=.\e_ebcdic.h
# End Source File
# Begin Source File

SOURCE=.\e_macroman.h
# End Source File
# Begin Source File

SOURCE=.\p_cid.h
# End Source File
# Begin Source File

SOURCE=.\p_config.h
# End Source File
# Begin Source File

SOURCE=.\p_font.h
# End Source File
# Begin Source File

SOURCE=.\p_intern.h
# End Source File
# Begin Source File

SOURCE=.\p_metrics.h
# End Source File
# Begin Source File

SOURCE=.\pdflib.h
# End Source File
# End Group
# End Target
# End Project
