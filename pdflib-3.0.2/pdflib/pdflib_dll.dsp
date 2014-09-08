# Microsoft Developer Studio Project File - Name="pdflib_dll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=pdflib_dll - Win32 no auxiliary libs
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "pdflib_dll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "pdflib_dll.mak" CFG="pdflib_dll - Win32 no auxiliary libs"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "pdflib_dll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "pdflib_dll - Win32 no auxiliary libs" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=xicl6.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "pdflib_dll - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "pdflib___Win32_Release"
# PROP BASE Intermediate_Dir "pdflib___Win32_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release_DLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PDFLIB_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MT /W3 /O2 /I "../../zlib" /I "../../libtiff" /I "../../libpng" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_MT" /D "PDFLIB_EXPORTS" /D "HAVE_LIBZ" /D "HAVE_LIBTIFF" /D "HAVE_LIBPNG" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /version:3.1 /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib zlib.lib libpng.lib libtiff.lib /nologo /base:"0x55300000" /version:3.1 /dll /pdb:none /machine:I386 /out:"pdflib.dll" /libpath:"..\..\zlib" /libpath:"..\..\libpng" /libpath:"..\..\libtiff"
# SUBTRACT LINK32 /debug /nodefaultlib

!ELSEIF  "$(CFG)" == "pdflib_dll - Win32 no auxiliary libs"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "pdflib_dll___Win32_no_auxiliary_libs"
# PROP BASE Intermediate_Dir "pdflib_dll___Win32_no_auxiliary_libs"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O2 /I "../../zlib" /I "../../libtiff" /I "../../libpng" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_MT" /D "PDFLIB_EXPORTS" /D "HAVE_LIBZ" /D "HAVE_LIBTIFF" /D "HAVE_LIBPNG" /YX /FD /c
# ADD CPP /nologo /MT /W3 /O2 /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_MT" /D "PDFLIB_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib zlib.lib libpng.lib libtiff.lib /nologo /base:"0x55300000" /version:3.1 /dll /pdb:none /machine:I386 /out:"pdflib.dll" /libpath:"..\..\zlib" /libpath:"..\..\libpng" /libpath:"..\..\libtiff"
# SUBTRACT BASE LINK32 /debug /nodefaultlib
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x55300000" /version:3.1 /dll /pdb:none /machine:I386 /out:"pdflib.dll" /libpath:"..\..\zlib" /libpath:"..\..\libpng" /libpath:"..\..\libtiff"
# SUBTRACT LINK32 /debug /nodefaultlib

!ENDIF 

# Begin Target

# Name "pdflib_dll - Win32 Release"
# Name "pdflib_dll - Win32 no auxiliary libs"
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
# Begin Source File

SOURCE=.\pdflib.rc
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
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
